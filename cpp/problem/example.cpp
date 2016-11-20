#include <iostream>
#include <string>
#include <memory>

#include "Problem.hpp"


////////////////////////////////////////////////////////////////////////
int main()
{
    SolutionUPtr first;
    SolutionUPtr second;

    std::cout << "Start" << std::endl;

    {
        Problem problem(7);
        first = problem.solve();
    }

    std::cout << "Solution A at 3 has value " << first->eval(3) << std::endl;

    {
        Problem problem(13);
        second = problem.solve(*first);
    }

//    problem.setSolver(new BSolver);
//    SolutionUPtr second = problem.solve();
//
//    std::cout << "Solution B at 3 has value " << second->eval(3) << std::endl;

    std::cout << "End" << std::endl;

    return 0;
}
