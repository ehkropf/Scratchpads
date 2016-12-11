#include <iostream>
#include <vector>
#include <unordered_set>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& A)
{
    for (int n : A)
        os << " " << n;
    return os;
}

int solution(std::vector<int>& A)
{
    int result = A[0];
    for (auto i = A.begin() + 1, ae = A.end(); i < ae; ++i)
        result ^= *i;

    return result;
}

int main()
{
    std::vector<int> input{9, 3, 9, 3, 9, 7, 9};

    std::cout << input << std::endl
        << "result: " << solution(input) << std::endl;

    return 0;
}
