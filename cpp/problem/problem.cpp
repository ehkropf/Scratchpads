#include <iostream>
#include <memory>

////////////////////////////////////////////////////////////////////////
/*!
 * Interface specification for solution eval().
 */
class Solution
{
public:
    virtual ~Solution() = default;

    virtual int eval(int) = 0;
};

typedef  std::unique_ptr<Solution> SolutionPtr;


////////////////////////////////////////////////////////////////////////
/*!
 * Takes given data on construction and adds it to input for evaluation.
 */
class ASolution : public Solution
{
    int _data;

protected:
    std::string _name;

public:
    ASolution(int data, std::string name = "ASolution") : _data(data), _name(name)
    {
        std::cout << _name << this << " built with data " << _data << std::endl;
    }
    ~ASolution()
    {
        std::cout << _name << this << " destroyed" << std::endl;
    }

    int getData() const { return _data; }

    virtual int eval(int input) { return input + _data; }
};

/*!
 * Basically rename ASolution.
 */
class BSolution : public ASolution
{
public:
    BSolution(int data) :  ASolution(data, "BSolution") {}
};


////////////////////////////////////////////////////////////////////////
class Problem;

/*!
 * Abstract solve() specification for Solver interface.
 */
class Solver
{
public:
    virtual ~Solver() = default;

    virtual Solver& newCopy() = 0;
    virtual SolutionPtr solve(const Problem&) = 0;
};

typedef std::unique_ptr<Solver> SolverPtr;


////////////////////////////////////////////////////////////////////////
/*!
 * Solver method takes input data from problem and adds 10 to pass to
 * solution.
 */
class ASolver : public Solver
{
public:
    ASolver() { std::cout << "ASolver " << this << " built" << std::endl; }
    ~ASolver() { std::cout << "ASolver " << this << " destroyed" << std::endl; }

    virtual Solver& newCopy() { return *(new ASolver); }
    virtual SolutionPtr solve(const Problem&);
};

/*!
 * Constructs solution wtih data value of `15` regardless of problem
 * data.
 */
class BSolver: public Solver
{
public:
    BSolver() { std::cout << "BSolver " << this << " built" << std::endl; }
    ~BSolver() { std::cout << "BSolver " << this << " destroyed" << std::endl; }

    virtual Solver& newCopy() { return *(new BSolver); }
    virtual SolutionPtr solve(const Problem&) { return SolutionPtr(new BSolution(15)); }
};


////////////////////////////////////////////////////////////////////////
/*!
 * Default solver is `ASolver`
 */
class Problem
{
    int _data;
    SolverPtr _solver;

public:
    Problem(int data) : _data(data), _solver(new ASolver)
    {
        std::cout << "Problem built with data " << _data << std::endl;
    }
    ~Problem()
    {
        std::cout << "Problem destroyed" << std::endl;
    }

    int data() const { return _data; }

    SolutionPtr solve();
    const Solver& solver() const { return *_solver; }
    void setSolver(Solver* solver) { _solver.reset(solver); }
};


////////////////////////////////////////////////////////////////////////
// Problem class definitions.

SolutionPtr
Problem::solve()
{
    return _solver->solve(*this);
}


////////////////////////////////////////////////////////////////////////
// Solver definitions.

SolutionPtr
ASolver::solve(const Problem& problem)
{
    return SolutionPtr(new ASolution(problem.data() + 10));
}


////////////////////////////////////////////////////////////////////////
int main()
{
    std::cout << "Start" << std::endl;

    Problem problem(7);
    SolutionPtr first = problem.solve();

    std::cout << "Solution A at 3 has value " << first->eval(3) << std::endl;

    problem.setSolver(new BSolver);
    SolutionPtr second = problem.solve();

    std::cout << "Solution B at 3 has value " << second->eval(3) << std::endl;

    std::cout << "End" << std::endl;

    return 0;
}
