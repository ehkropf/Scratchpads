#include <iostream>
#include <string>
#include <memory>

#include "Problem.hpp"


////////////////////////////////////////////////////////////////////////
// Problem class definitions.

SolutionUPtr
Problem::solve()
{
    return _solver->solve(*this);
}

SolutionUPtr
Problem::solve(Solution& prevSolution)
{
    return _solver->solve(*this, prevSolution);
}


////////////////////////////////////////////////////////////////////////
// Solver definitions.

SolutionUPtr
ASolver::solve(const Problem& problem)
{
    if (!_solverData.get())
    {
        std::cout << "Creating new solver data" << std::endl;
        _solverData = std::make_shared<ASolverData>();
    }
    return SolutionUPtr(new ASolution(problem.data() + 10, _solverData));
}

SolutionUPtr
ASolver::solve(const Problem& problem, Solution& prevSolution)
{
    std::cout << "ASolver given previous solution data" << std::endl;
    _solverData = dynamic_cast<ASolution*>(&prevSolution)->getSolverData();
    return solve(problem);
}


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
