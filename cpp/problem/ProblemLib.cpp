#include "Problem.hpp"
#include "ASolver.hpp"

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
