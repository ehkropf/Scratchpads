#include <iostream>
#include <vector>

int solution(std::vector<int>& A)
{
    int T = 0;
    int c = 0;
    for (int a : A)
    {
        if (a)
            T += c;
        else
            ++c;

        if (T > 1e6) return -1;
    }

    return T;
}

int main()
{
    using intv = std::vector<int>;

    intv A(1e3, 0);
    intv A1(1e3, 1);
    A.insert(A.end(), A1.begin(), A1.end());
//    intv A{0, 1, 0, 1, 1};

    int ans = solution(A);
    std::cout << "answer: " << ans << std::endl;

    return 0;
}
