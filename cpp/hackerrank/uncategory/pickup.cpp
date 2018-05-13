#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

static std::string input(
"5\n"
"17\n"
"3 3 6 1 3 9 4 2 13 5 5 1 3 11 2 1 0\n"
"18\n"
"2 1 9 0 5 8 1 4 1 1 2 0 0 10 2 5 1 5\n"
"18\n"
"2 9 3 2 5 2 1 0 11 2 1 7 0 8 6 2 0 4\n"
"18\n"
"6 1 0 2 1 6 9 11 11 8 5 11 1 6 3 12 11 3\n"
"20\n"
"0 0 2 11 0 2 8 1 5 4 10 4 7 4 13 1 10 2 8 11\n"
);

static std::string expected(
"759833446\n"
"709820289\n"
"802116046\n"
"343692793\n"
"704373542\n"
);

int main()
{
    unsigned M = 1e9 + 7;
    std::stringstream ss;
    ss.str(input);

    unsigned T;
    ss >> T;

    while (T--)
    {
        unsigned N;
        ss >> N;

        std::vector<unsigned> count(N+1, 0);
        for (unsigned i = 0; i < N; ++i)
        {
            unsigned ci;
            ss >> ci;
            ++count[ci];
        }

        unsigned i = 0, left = 0, picked = 0;
        unsigned long ways = 1;
        while (picked < N)
        {
            assert(i < N);
            left += count[i++];
            if (!left)
            {
                ways = 0;
                break;
            }
            ways = (ways*left)%M;
            --left;
            ++picked;
        }

        std::cout << ways << std::endl;
    }

    return 0;
}
