#include <iostream>
#include <vector>
#include <cassert>

// #define OUT(S) std::cout << S << std::endl
#define OUT(S)

using Grid = std::vector<std::string>;

struct Coord
{
    int x;
    int y;

    Coord() : x(-1), y(-1) {}
    bool notset() { return x == -1 || y == -1; }
};

std::ostream& operator<<(std::ostream& os, const Coord& c)
{
    std::cout << "x=" << c.x << " y=" << c.y;
    return os;
}

void displayPathtoPrincess(int n, Grid grid){
    Coord m, p;
    int y = 0;
    for (auto& s : grid)
    {
        OUT("gridline: " << s);
        int x;
        if (m.notset())
        {
            x = s.find_first_of('m');
            if (x != std::string::npos)
            {
                m.y = y;
                m.x = x;
                OUT("found m : " << m);
            }
        }
        if (p.notset())
        {
            x = s.find_first_of('p');
            if (x != std::string::npos)
            {
                p.y = y;
                p.x = x;
                OUT("found p : " << p);
            }
        }
        ++y;
    }

    assert(!(m.notset() && p.notset()));

    int dx = p.x - m.x;
    OUT("dx = " << dx);
    if (dx)
    {
        std::string move(dx > 0 ? "RIGHT" : "LEFT");
        dx = dx > 0 ? dx : -dx;
        for (int i = 0; i < dx; ++i) std::cout << move << "\n";
    }
    int dy = p.y - m.y;
    OUT("dy = " << dy);
    if (dy)
    {
        std::string move(dy > 0 ? "DOWN" : "UP");
        dy = dy > 0 ? dy : -dy;
        for (int i = 0; i < dy; ++i) std::cout << move << "\n";
    }
}

int main(void) {

    int m;
    Grid grid;

    std::cin >> m;

    for(int i=0; i<m; ++i) {
        std::string s; std::cin >> s;
        grid.push_back(s);
    }

    displayPathtoPrincess(m, grid);

    return 0;
}
