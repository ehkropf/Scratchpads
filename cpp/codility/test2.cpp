#include <iostream>

int solution(int N, int K)
{
    if (N < 0 || K < 0 || N < K)
        return -1;

    // Because B(N, K) = B(N, N-K).
    if (K > N-K)
        K = N-K;

//    if (K == 1 && N < 1e9)
//        return N;

    // I use this method in a "Schwarz problem" solver.
    // I think complexity is just K.
    long result = 1;
    for (int i = 0; i < K; ++i)
    {
        result = (long)(result*(long)(N - i))/(long)(i + 1);
//        result *= N - i;
//        result /= i + 1; //K - i;

        if (result > 1e9)
            return -1;
    }

    return result;
}

int main()
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j <= i; ++j)
            std::cout << solution(i, j) << " ";
        std::cout << std::endl;
    }

    std::cout << solution(40, 20) << std::endl;
    std::cout << solution(3, 5) << std::endl;
    std::cout << solution(2147483647, 2147483647) << std::endl;
    std::cout << solution(2147483647, 1) << std::endl;
    std::cout << solution(2147483647, 0) << std::endl;

    return 0;
}
