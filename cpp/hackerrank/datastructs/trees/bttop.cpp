#include <iostream>

#include "btree.hpp"

//--------------------------------------------------------------------------
int main()
{
    int N;
    std::cin >> N;

    Btree T = Btree::from_cin(N);

    OUT("-----");
    OUT(T);

    OUT("-----");
    OUT("Top view: " << T.topView());

    return 0;
}
