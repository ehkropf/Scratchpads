#ifndef ASOLUTION_HPP
#define ASOLUTION_HPP

#include <iostream>
#include <string>

#include "Solution.hpp"
#include "ASolverData.hpp"

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

#endif // ASOLUTION_HPP
