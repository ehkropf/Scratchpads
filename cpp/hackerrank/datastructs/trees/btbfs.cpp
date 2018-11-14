#include <iostream>

#include "btree.hpp"

int main()
{
    int N;
    std::cin >> N;

    Btree T = Btree::from_cin(N);

    OUT("-----\n" << T << "\n-----");
    T.levelTraversal();

    return 0;
}
