#include <iostream>
#include <utility>
#include <list>

using upair = std::pair<unsigned, unsigned>;
using upvec = std::list<upair>;

unsigned combo(unsigned n, unsigned A, unsigned B, unsigned C, unsigned D)
{
    // Check with 1 and 2 first.
    upvec abs;
    for (unsigned a = 0; a <= A; ++a)
    {
        for (unsigned b = 0; b <= B; ++b)
        {
            if (a == 0 && b == 0) continue;
            if ((n - 2*b - a) % 5 == 0)
            {
                abs.emplace_back(upair(a, b));
                std::cout << "(a, b) = " << abs.back().first << " " << abs.back().second << std::endl;
            }
        }
    }

    // Then check with 5 and 10.
    upvec cds;
    for (unsigned c = 0; c <= C; ++c)
    {
        for (unsigned d = 0; d <= D; ++d)
        {
            if (c == 0 && d == 0) continue;
            unsigned ncd = n - 5*c - 10*d;
            if (ncd == 0)
            {
                cds.emplace_back(upair(c, d));
                std::cout << "(c, d) = " << cds.back().first << " " << cds.back().second << std::endl;
            }
            if (ncd <= 0) continue;
            for (auto ab = abs.begin(); ab != abs.end(); )
            {
                unsigned a = ab->first;
                unsigned b = ab->second;
                if ((ncd - a - 2*b) == 0)
                {
                    ab = abs.erase(ab);
                    cds.emplace_back(upair(c, d));
                    std::cout << "(c, d) = " << cds.back().first << " " << cds.back().second << std::endl;
                    break;
                }
                else ++ab;
            }
        }
    }

    return abs.size() + cds.size();
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
//    unsigned T;
//    std::cin >> T;
//
//    for (unsigned i = 0; i < T; ++i)
//    {
//        unsigned n, A, B, C, D;
//        std::cin >> n;
//        std::cin >> A >> B >> C >> D;
//        std::cout << combo(n, A, B, C, D);
//    }

    //unsigned n = 15, A = 2, B = 3, C = 1, D = 1;
    //unsigned n = 12, A = 2, B = 3, C = 1, D = 1;
    unsigned n = 1000, A = 10000, B = 1000, C = 1000, D = 1000;
    std::cout << combo(n, A, B, C, D) << std::endl;

    return 0;
}
