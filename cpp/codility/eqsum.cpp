#include <iostream>
#include <vector>

int solution(std::vector<int>& A)
{
    int lsum = 0, rsum = 0;
    for (auto i = A.begin()+1, ie = A.end(); i != ie; ++i)
        rsum += *i;
    if (rsum == 0) return 0;

    for (auto i = A.begin()+1, ie = A.end(); i != ie; ++i)
    {
        lsum += *(i-1);
        rsum -= *i;
        std::cout << "lsum = " << lsum << ", rsum = " << rsum << std::endl;
        if (lsum == rsum) return std::distance(A.begin(), i);
    }

    return -1;
}

int main()
{
    std::vector<int> input{1, -1, 2, 3};
    // std::vector<int> input{1, -1, 0};
    // std::vector<int> input{0, -1, 1};
    // std::vector<int> input{1, 1, 1, 1, 3};
    // std::vector<int> input{-1, 3, -4, 5, 1, -6, 2, 1};

    std::cout << solution(input) << std::endl;

    return 0;
}
