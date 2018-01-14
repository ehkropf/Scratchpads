#include <iostream>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// From cppreference. Note this basically makes a _second_ definition. There
// is a compile error if generic member access is attempted in the
// specialization.

template< typename T>
struct A {
    struct B {};  // member class
    template<class U> struct C { }; // member class template
};

struct C {};

template<> // specialization
struct A<int> {
    void f(int); // member function of a specialization
};

// template<> not used for a member of a specialization
void A<int>::f(int)
{
    // B = C; // This is an error!
    std::cout << "A<inf>::f(int) special function\n";
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// If we declare the function (and provide an arguably neccessary default
// implementation), then the specialized function can access the member
// variable.
template <typename T>
struct X
{
    T x;
    struct B {};

    void specfun(T);
};

template <typename T>
void X<T>::specfun(T y)
{
    x = y;
    std::cout << "specfun() default implementation\n";
}

template<>
void X<int>::specfun(int y)
{
    x = y + 25; // _Now_ the generic member can be accessed.
    std::cout << "specfun() called for X<int> instance\n";
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main()
{
    X<int> asdf;
    asdf.x = 14;
    asdf.specfun(15);

    return 0;
};
