#include <iostream>
#include <string>
#include <locale>

int countCamel(const std::string& S)
{
    int count = 1;
    for (auto i = S.begin(), end = S.end(); i != end; ++i)
        if (std::isupper(*i)) ++count;

    return count;
}

int main()
{
    // std::string S("saveChangesInTheEditor");
    std::string S = "zZ" + std::string(1e5-2, 'z');

    int ans = countCamel(S);
    std::cout << "\nwords counted: " << ans << "\n" << std::endl;

    return 0;
}
