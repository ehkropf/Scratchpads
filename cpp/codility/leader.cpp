#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& A)
{
    for (int n : A)
        os << n << " ";
    return os;
}

bool allSame(const std::vector<int>& A)
{
    int ref = A[0];
    for (auto i = A.begin()+1, e = A.end(); i != e; ++i)
        if (*i != ref)
            return false;
    return true;
}

int solution(std::vector<int>& A)
{
    int N = A.size();

    // Edge cases.
    if (N < 2)
        return -1;
    else if(N == 2)
    {
        if (A[0] == A[1])
            return 0;
        else
            return -1;
    }

    while (A.size() > 1 && !allSame(A))
    {
        std::cout << A << std::endl;
        for (auto i = A.begin()+1, e = A.end(); i != e; ++i)
        {
            if (*(i-1) != *i)
            {
                A.erase(i-1, i+1);
                // iterator invalid.
                break;
            }
        }
    }

    std::cout << A << std::endl;

//    int poss = -1;
//    int odd = N % 2;
//
//    // Each pair of non-matching numbers marked off list leaves
//    // possible leaader still in the list.
//    for(int i = 0; i < N-odd; i += 2)
//    {
//        if (A[i] == A[i+1]) poss = i;
//    }
//    if (poss == -1 && odd)
//        poss = N-1;
//    else
//        return poss;

    // Still have to count to check?
    return -1;
}

int main()
{
    using intv = std::vector<int>;

    intv A{3, 4, 3, 2, 3, -1, 3, 3};
    //intv A{3, 4, 3, 2, 3, -1, 2};
    //intv A{4, 4, 3, 3, 3};
    //intv A{4, 4, 3, 3, 3, 4, 4};
    //intv A{4, 4, 4, 3, 3};

    int ans = solution(A);
    std::cout << "answer: " << ans << std::endl;

    return 0;
}
