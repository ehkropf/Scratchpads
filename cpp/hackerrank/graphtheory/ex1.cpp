#include <iostream>

#include "graph.h"

#define ENDL std::endl;
#define COUT(S) std::cout << S << ENDL

//------------------------------------------------------------------------------
int main()
{
    unsigned n_v = 10;
    Graph g;

    {
        Vertices v;
        for (unsigned i = 0; i < n_v; ++i)
        {
            v.emplace_back();
        }
        COUT("Size of v before set: " << v.size());
        g.set_vertices(std::move(v));
        COUT("Size of v after set: " << v.size());
    }

    COUT("Graph g has " << g.vertices().size() << " vertices.");

    return 0;
}
