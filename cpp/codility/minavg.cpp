#include <vector>
#include <iostream>

float sliceAvg(const std::vector<int>& A, int p, int q)
{
    int sum = 0;
    int N = q - p + 1;
    for (; p < q+1; ++p)
        sum += A[p];
    return (float)sum/(float)N;
}

int solution(std::vector<int> &A) {
    // write your code in C++14 (g++ 6.2.0)

    std::cout << "Size A = " << A.size() << std::endl;

    int pm = 0;
    int N = A.size();
    float avmin = std::numeric_limits<float>::max();

    // Check size 3, then size 2 slices.
    for (int qp : {3, 2})
    {
        if (A.size() < qp) continue;

        for(int p = 0, q = qp-1; q < N; ++p, ++q)
        {
            std::cout << "check: p=" << p
                << ", q=" << q
                << ", qp=" << qp << std::endl;
            float sa = sliceAvg(A, p, q);
            if (sa < avmin)
            {
                pm = p;
                avmin = sa;
                std::cout << "newmin = " << avmin << ", index: " << pm << std::endl;
            }
        }
    }

    return pm;
}

int main()
{
    std::vector<int> input{9, 8, 7, 6, 5, 4, 3, 2, 1};
    // std::vector<int> input{2, 4};
    // std::vector<int> input{4, 2, 2, 5, 1, 5, 8};

    int ans = solution(input);
    std::cout << "answer: " << ans << std::endl;

    return 0;
}
