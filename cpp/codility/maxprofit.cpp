#include <iostream>
#include <vector>

int solution(std::vector<int>& A)
{
    int N = A.size();
    if (N < 2) return 0;

    int curr = 0, profit = 0;
    for (auto i = A.begin()+1, end = A.end(); i != end; ++i)
    {
        curr = std::max(0, curr + *i - *(i-1));
        profit = std::max(profit, curr);
    }
//    for (auto i = A.begin(), e = A.end(); i != e-1; ++i)
//        for (auto j = i+1; j != e; ++j)
//            profit = std::max(profit, *j - *i);

    return profit;
}

int main()
{
    using intv = std::vector<int>;

    intv A{1};
    //intv A{2, 4};
    //intv A{23171, 21011, 21123, 21366, 21013, 21367};

    int ans = solution(A);
    std::cout << "answer: " << ans << std::endl;

    return 0;
}
