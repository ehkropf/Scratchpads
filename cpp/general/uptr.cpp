#include <iostream>
#include <memory>

/*
 * What is the best way to store a runtime polymorphic object in a class
 * when we want the class to manage the lifetime of the object?
 */

////////////////////////////////////////////////////////////////////////
/*!
 * Base.
 */
struct Base
{
    Base() { std::cout << "Built a Base\n"; }
    virtual ~Base() { std::cout << "Destroyed a Base\n"; }

    virtual std::string name() const { return "Base"; }
};

////////////////////////////////////////////////////////////////////////
/*!
 * Derived.
 */
struct Derived : Base
{
    Derived() { std::cout << "Built a Derived\n"; }
    ~Derived() { std::cout << "Destroyed a Derived\n"; }

    virtual std::string name() const { return "Derived"; }
};

////////////////////////////////////////////////////////////////////////
/*!
 * User of things of Base type.
 */
class User
{
    std::unique_ptr<Base> _thing;

public:
    User() {}
    User(Base* thing) : _thing(thing) {}
    // User(std::unique_ptr<Base> uptr) : _thing(std::move(uptr)) {}
    User(std::unique_ptr<Base>&& uptr) : _thing(std::move(uptr)) {}

    friend std::ostream& operator<<(std::ostream& os, const User& user)
    {
        os << "User thing claims to be a " << user._thing->name();
        return os;
    }
};

////////////////////////////////////////////////////////////////////////
int main()
{
    {
        Base* b = new Derived;
        User u(b);

        std::cout << u << std::endl;
    }

    std::cout << std::endl;

    {
        // User u(new Derived);

        // std::unique_ptr<Base> b(new Derived);
        // User u(std::move(b));

        User u(std::unique_ptr<Base>(new Derived));

        std::cout << u << std::endl;
    }

    return 0;
};
