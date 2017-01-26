#include <iostream>
#include <vector>
#include <utility>

using uvec = std::vector<unsigned>;
using upair = std::pair<unsigned, unsigned>;
using puvec = std::vector<upair>;

int main()
{
    puvec pairs;

    pairs.emplace_back(upair(2, 3));
    pairs.emplace_back(upair(7, 2));

    std::cout << pairs[0].first << "," << pairs[0].second << std::endl;
    std::cout << pairs[1].first << "," << pairs[1].second << std::endl;

    return 0;
}
