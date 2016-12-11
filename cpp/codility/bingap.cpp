#include <iostream>
#include <vector>
#include <string>

int bingap(int n)
{
    std::string bs;
    int g = 0, curr = 0;

    // Find least significant bit.
    int lsb = 0;
    while (!((n >> lsb) & 1) && lsb < 31)
    {
        ++lsb;
        bs.insert(0, "0");
    }
//    for (; lsb < 32; ++lsb)
//        if ((n >> lsb ) & 1) break;

    // Count.
    for (int i = lsb; i < 31; ++i)
    {
        if (n < (1 << i)) break;

        bool bi = (n >> i) & 1;
        if (bi)
        {
            bs.insert(0, "1");
            g = std::max(g, curr);
            curr = 0;
        }
        else
        {
            bs.insert(0, "0");
            curr += 1;
        }
    }
    std::cout << bs << std::endl;
    return g;
}


int main()
{
    // static input
    std::vector<int> input{1, 4, 11, 16, 1041, (int)(0x7fffffff)};

    for (int n : input)
    {
        int g = bingap(n);
        std::cout << n << " -> " << g << std::endl;
    }

    return 0;
}
