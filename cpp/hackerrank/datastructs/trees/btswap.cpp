#include "btree.hpp"

int main()
{
    int N;
    std::cin >> N;
    Btree T = Btree::unFromCin(N);

    OUT(T);

    return 0;
}
