#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <memory>
#include "Solution.hpp"

class Problem;

/*!
 * Abstract solve() specification for Solver interface.
 */
class Solver
{
public:
    virtual ~Solver() = default;

    virtual Solver& newCopy() = 0;
    virtual SolutionUPtr solve(const Problem&) = 0;
    virtual SolutionUPtr solve(const Problem&, Solution&) = 0;
};

typedef std::unique_ptr<Solver> SolverUPtr;

#endif // SOLVER_HPP
