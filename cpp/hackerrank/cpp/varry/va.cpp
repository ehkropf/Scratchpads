#include <iostream>
#include <vector>

//------------------------------------------------------------------------------
using intv = std::vector<int>;
using intvv = std::vector<intv>;

//------------------------------------------------------------------------------
int main()
{
    // First line: n q
    int n, q;
    std::cin >> n >> q;

    // n lines of arrays to add.
    intvv a;
    a.reserve(n);

    while (n--)
    {
        // Each line: k k0 k1 k2 ... k(k-1)
        int k;
        std::cin >> k;

        a.emplace_back(intv());
        a.back().reserve(k);

        for (int j = 0; j < k; ++j)
        {
            int x;
            std::cin >> x;
            a.back().emplace_back(x);
        }
    }

    while (q--)
    {
        int i, j;
        std::cin >> i >> j;
        std::cout << a[i][j] << std::endl;
    }

    return 0;
}
