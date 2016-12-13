#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

int solution(int n)
{
    long a = 0, b = 1;
    for (int i = 0; i < n; ++i)
    {
        long tmp = a;
        a = b;
        b = b + tmp;
    }

    std::cout << a << std::endl;
    return (int)(a % (long)1e6);

//    std::stringstream s;
//    s << std::setfill('0') << std::setw(6) << a;
//    std::cout << s.str() << std::endl;
//    int ans;
//    s >> ans;
//    return ans;
}

int main()
{
    //for (int i = 0; i < 10; ++i)
    //    std::cout << solution(i) << std::endl;

    for (int i : {0, 1, 36, 2147483647})
        std::cout << solution(i) << std::endl;

    return 0;
}
