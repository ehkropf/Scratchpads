#include <iostream>
#include <vector>

using uvec = std::vector<unsigned>;

int main()
{
    //unsigned n = 15, A = 2, B = 3, C = 1, D = 1;
    unsigned n = 1000, A = 10000, B = 1000, C = 1000, D = 1000;

    unsigned long long ans = 0;
    std::vector<unsigned> ways(n+1, 0);

    for (unsigned b = 0; b <= B && 2*b <= n; ++b)
        for (unsigned a = 0; a <= A && a + 2*b <= n; ++a)
            ways[a + 2*b]++;

    for (unsigned c = 0; c <= C && 5*c <= n; ++c)
        for (unsigned d = 0; d <= D && 10*d + 5*c <= n; ++d)
            ans += ways[n - 5*c - 10*d];

    std::cout << ans << std::endl;

    return 0;
}
