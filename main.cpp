#include <string>
#include <map>
#include <iostream>
#include <memory>
#include <functional>

class Type
{
public:
    virtual ~Type() {}
    virtual void fun() = 0;
};

class ConcreteTypeA : public Type
{
public:
    void fun() override { std::cout << "A" << std::endl;}
};

class ConcreteTypeB : public Type
{
public:
    ConcreteTypeB(int);
    void fun() override { std::cout << "B" << std::endl;}
};

template <typename T>
class SomeClass
{
    using CreateType = std::function<std::unique_ptr<Type>()>;
    
    std::map<std::string, CreateType> storage_;
public:
    template <typename F>
    void someMethod(const std::string& id, F type) 
    {
        // storage_.emplace(id, type);
    }

    template<typename... Args>
    std::unique_ptr<T> call(std::string id, Args&&... args)
    {
        auto& st = storage_.at(id);
        return st(std::forward<Args>(args)...);
    }

};

std::unique_ptr<Type> fun_create_A()
{
    return std::make_unique<ConcreteTypeA>();
}

template <typename... Args>
std::unique_ptr<Type> fun_create_B(Args&&... args)
{
    return std::make_unique<ConcreteTypeB>(std::forward<Args>(args)...);
}

struct caller_helper {
  template<class...Args>
  auto operator()(Args&&...args)
  -> decltype( func_create_B(std::declval<Args>()...) )
  {
    return func_create_B(std::forward<Args>(args)...);
  }
};

template<typename Fn, Fn fn, typename... Args>
typename std::result_of<Fn(Args...)>::type
wrapper(Args&&... args) {
    return fn(std::forward<Args>(args)...);
}

#define WRAPPER(FUNC) wrapper<decltype(&FUNC), &FUNC>

// std::function<std::unique_ptr<Type>()> createFn = []{ return fun_create_B(); };

int main()
{
    SomeClass<Type> rt;
    rt.someMethod("A", &fun_create_A);
    auto a = rt.call("A");
    a->fun();

    WRAPPER(fun_create_A)();

    // rt.someMethod("B", caller_helper());
    // auto b = rt.call("B", 4);
    // b->call();

    return 0;
}
