#include "btree.hpp"

int main()
{
    int N;
    std::cin >> N;
    Btree tree = Btree::unFromCin(N);

    OUT(tree);
    OUT("In order: ");
    tree.inOrder();

    int T;
    std::cin >> T;
    while (T--)
    {
        int K;
        std::cin >> K;
        OUT("Swap K=" << K);
        tree.KSwap(K);
        OUT(tree);
        tree.inOrder();
    }

    return 0;
}
