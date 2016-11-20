#ifndef BSOLUTION_HPP
#define BSOLUTION_HPP

#include "ASolution.hpp"

/*!
 * Basically rename ASolution.
 */
class BSolution : public ASolution
{
public:
    BSolution(int data) :  ASolution(data, "BSolution") {}
};

#endif // BSOLUTION_HPP
