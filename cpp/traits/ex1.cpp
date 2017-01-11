#include <iostream>

template<typename T>
struct supports_optimised_implementation
{
    static const bool value = false;
};

template<bool b>
struct algorithm_selector
{
    template<typename T>
    static void implementation(T&) // object)
    {
        std::cout << "I'm the default implementation." << std::endl;
    }
};

template<>
struct algorithm_selector<true>
{
    template<typename T>
    static void implementation(T& object)
    {
        object.optimised_implementation();
    }
};

template<typename T>
void algorithm(T& object)
{
    algorithm_selector<supports_optimised_implementation<T>::value>::implementation(object);
}

class ObjectA {};

class ObjectB
{
public:
    void optimised_implementation()
    {
        std::cout << "Optimised implementon here." << std::endl;
    }
};

template<>
struct supports_optimised_implementation<ObjectB>
{
    static const bool value = true;
};

int main()
{
    ObjectA a;
    algorithm(a);

    ObjectB b;
    algorithm(b);

    return 0;
}
