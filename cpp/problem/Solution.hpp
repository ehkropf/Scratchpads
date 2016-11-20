#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <memory>

/*!
 * Interface specification for Solution.
 *
 * Requires evaluation behaviour eval() defined.
 */
class Solution
{
public:
    virtual ~Solution() = default;

    virtual int eval(int) = 0;
};

typedef  std::unique_ptr<Solution> SolutionUPtr;

#endif // SOLUTION_HPP
