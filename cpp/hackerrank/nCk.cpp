#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

using ulong = unsigned long;
using uvec = std::vector<ulong>;
using uvvec = std::vector<uvec>;

ulong binom(unsigned n, unsigned k, unsigned mod = 1e9)
{
    static uvec table;

    if (k > n-k) k = n - k;
    if (k == 0) return 1;
    if (k == 1) return n;

    unsigned nT = n - 4;
    if (table.size() < n-3)
    {
        // Add row to table and calculate.
        table.emplace_back(uvvec());

        if (nT == 0) { table[nT].emplace_back(6); }
        else { table[nT].emplace_back(n-1 + table[nT-1][0]); }

        unsigned szTA = (nT - (int)(!(nT & 1)))/2 + 1;
        for (unsigned i = 1; i < szTA; ++i)
        {
            if (i+1 == szTA && nT & 1) break;
            unsigned tmp = table[nT-1][i-1] + table[nT-1][i];
            table[nT].emplace_back(tmp < mod ? tmp : tmp % mod);
        }
        if (nT & 1) table[nT][szTA-1] = 2*table[nT-1][szTA-2];
    }

    return table[nT][k-2];
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    unsigned T;
    std::cin >> T;

    //std::cout << "80_C_40: " << binom(80, 40) << std::endl;

    for (unsigned i = 0; i < T; ++i)
    {
        unsigned n;
        std::cin >> n;
        if (n == 1)
        {
            std::cout << "1" << std::endl;
            continue;
        }
        std::cout << "1 " << n;
        for (unsigned k = 2; k <= n; ++k)
        {
            std::cout << " " << binom(n, k);
        }
        std::cout << std::endl;
    }

    return 0;
}
