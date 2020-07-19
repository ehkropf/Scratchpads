#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>

// Complete the matchingStrings function below.
std::vector<int> matchingStrings(std::vector<std::string> strings, std::vector<std::string> queries) {
    std::map<std::string, int> sparse;
    std::vector<int> out;
    
    // Create sparse "hash".
    for (const auto s : strings)
    {
        auto pos = sparse.find(s);
        if (pos != sparse.end())
        {
            pos->second += 1;
        }
        else
        {
            sparse.insert(std::pair<std::string, int>{s, 1});
        }
    }

    // Check against queries.
    for (const auto s : queries)
    {
        auto pos = sparse.find(s);
        if (pos != sparse.end())
        {
            out.push_back(pos->second);
        }
        else
        {
            out.push_back(0);
        }
    }

    return out;
}

int main()
{
    std::ofstream fout{std::getenv("OUTPUT_PATH")};

    int strings_count;
    std::cin >> strings_count;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<std::string> strings(strings_count);

    for (int i = 0; i < strings_count; i++) {
        std::string strings_item;
        getline(std::cin, strings_item);

        strings[i] = strings_item;
    }

    int queries_count;
    std::cin >> queries_count;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<std::string> queries(queries_count);

    for (int i = 0; i < queries_count; i++) {
        std::string queries_item;
        getline(std::cin, queries_item);

        queries[i] = queries_item;
    }

    std::vector<int> res = matchingStrings(strings, queries);

    for (int i = 0; i < res.size(); i++) {
        fout << res[i];

        if (i != res.size() - 1) {
            fout << "\n";
        }
    }

    fout << "\n";

    fout.close();

    return 0;
}
