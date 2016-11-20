#ifndef ASOLVER_HPP
#define ASOLVER_HPP

#include <iostream>
#include "Solver.hpp"
#include "ASolution.hpp"

/*!
 * Solver method takes input data from problem and adds 10 to pass to
 * solution.
 */
class ASolver : public Solver
{
    ASolverDataSPtr _solverData;

public:
    ASolver() : _solverData(nullptr)
    {
        std::cout << "ASolver " << this << " built" << std::endl;
    }
    ~ASolver() { std::cout << "ASolver " << this << " destroyed" << std::endl; }

    virtual Solver& newCopy() { return *(new ASolver); }
    virtual SolutionUPtr solve(const Problem&);
    virtual SolutionUPtr solve(const Problem&, Solution&);
};

#endif // ASOLVER_HPP
