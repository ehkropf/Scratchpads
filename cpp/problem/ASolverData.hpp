#ifndef ASOLVERDATA_HPP
#define ASOLVERDATA_HPP

#include <iostream>
#include <memory>
#include <string>

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

#endif // ASOLVERDATA_HPP
