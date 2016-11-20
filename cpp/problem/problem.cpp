#include <iostream>
#include <string>
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

typedef  std::unique_ptr<Solution> SolutionUPtr;


////////////////////////////////////////////////////////////////////////
/*!
 * Solver data store.
 */
class ASolverData
{
    std::string _domainData;

public:
    ASolverData(std::string data = "A-method cached") : _domainData(data)
    {
        std::cout << "ASolverData created" << std::endl;
    }
    ~ASolverData()
    {
        std::cout << "ASolverData destroyed" << std::endl;
    }

    ASolverData(const ASolverData&) = default;
    ASolverData(ASolverData&&) = default;

    ASolverData& operator=(const ASolverData&) = default;
    ASolverData& operator=(ASolverData&&) = default;
};

typedef std::shared_ptr<ASolverData> ASolverDataSPtr;

/*!
 * Takes given data on construction and adds it to input for evaluation.
 */
class ASolution : public Solution
{
    int _data;
    ASolverDataSPtr _solverData;

protected:
    std::string _name;

public:
#define SOLUTION_NAME_STR "ASolution"
    ASolution(int data, std::string name = SOLUTION_NAME_STR) : _data(data), _name(name)
    {
        std::cout << _name << this << " built with data " << _data << std::endl;
    }
    ASolution(int data, ASolverDataSPtr solverData, std::string name = SOLUTION_NAME_STR)
        : _data(data), _solverData(solverData), _name(name)
    {
        std::cout << _name << this << " built with solver data and data " << _data << std::endl;
    }
    ~ASolution()
    {
        std::cout << _name << this << " destroyed" << std::endl;
    }

    int getData() const { return _data; }
    ASolverDataSPtr getSolverData() { return _solverData; }

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
    virtual SolutionUPtr solve(const Problem&) = 0;
    virtual SolutionUPtr solve(const Problem&, Solution&) = 0;
};

typedef std::unique_ptr<Solver> SolverPtr;


////////////////////////////////////////////////////////////////////////
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
    virtual SolutionUPtr solve(const Problem&) { return SolutionUPtr(new BSolution(15)); }
    virtual SolutionUPtr solve(const Problem& problem, Solution&) { return solve(problem); }
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
