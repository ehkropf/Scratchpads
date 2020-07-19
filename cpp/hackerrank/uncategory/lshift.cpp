#include <iostream>
#include <vector>
#include <string>

template <class FwdIter>
void printArray(FwdIter start, FwdIter end)
{
    while (start != end)
    {
        std::cout << *start++ << " ";
    }
    std::cout << "\n";
}

std::vector<std::string> split_string(std::string);

int main()
{
    std::string nd_temp;
    std::getline(std::cin, nd_temp);

    std::vector<std::string> nd = split_string(nd_temp);

    int n = stoi(nd[0]);

    int d = stoi(nd[1]);

    std::string a_temp_temp;
    std::getline(std::cin, a_temp_temp);

    std::vector<std::string> a_temp = split_string(a_temp_temp);

    std::vector<int> a(n);

    for (int i = 0; i < n; i++) {
        int a_item = stoi(a_temp[i]);

        a[i] = a_item;
    }

    for (int i = 0, j = d % n ; i < n; ++i)
    {
        std::cout << a[j++] << " ";
        if (j >= n) j %= n;
    }
    std::cout << "\n";

    return 0;
}

std::vector<std::string> split_string(std::string input_string) {
    std::string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    std::vector<std::string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != std::string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, std::min(pos, input_string.length()) - i + 1));

    return splits;
}

