#ifndef PROBLEM_HPP
#define PROBLEM_HPP

// #include "Solution.hpp"
#include "ASolver.hpp"

/*!
 * Default solver is `ASolver`
 */
class Problem
{
    int _data;
    SolverUPtr _solver;

public:
    Problem(int data, Solver* solver = new ASolver) : _data(data), _solver(solver)
    {
        std::cout << "Problem built with data " << _data << std::endl;
    }
    ~Problem()
    {
        std::cout << "Problem destroyed" << std::endl;
    }

    int data() const { return _data; }

    SolutionUPtr solve();
    SolutionUPtr solve(Solution& prevSolution);
    const Solver& solver() const { return *_solver; }
    void setSolver(Solver* solver) { _solver.reset(solver); }
};

#endif // PROBLEM_HPP
