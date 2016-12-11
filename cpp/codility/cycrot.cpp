#include <iostream>
#include <vector>

std::vector<int> solution(std::vector<int> &A, int K)
{
    int N = A.size();
    if (N == 0) return A;

    int rot = K % N;
    if (K == 0) return A;

    std::vector<int> B;
    B.reserve(N);

    if (rot == 0) return A;

    auto ri = A.end() - rot;
    for (auto i = ri; i < A.end(); ++i)
        B.emplace_back(*i);
    for (auto i = A.begin(); i < ri; ++i)
        B.emplace_back(*i);

    return B;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int> &A)
{
    for (int n : A)
        os << " " << n;

    return os;
}

int main()
{
    using intv = std::vector<int>;
    using inlist = std::vector<intv>;

    inlist input{ intv(), {-1, 2, -3, 4, -5} };
//    std::vector<int> input{-1, 2, -3, 4, -5};
    std::vector<int> Kv{0, 1, 2, 5, 6, 9, 10};

    for (intv A : input)
    {
        for (int K : Kv)
        {
            std::cout << "K = " << K << std::endl;
            std::cout << solution(A, K) << std::endl;
        }
    }

    return 0;
}
