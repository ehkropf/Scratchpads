#include "graph.hpp"

//--------------------------------------------------------------------------
long calculate_pairs(const intv& nc)
{
    OUT("Components: " << nc.size());
    if (nc.size() < 2) return 0;

    intv csum(nc.size() - 1);
    csum[0] = nc[0];
    long res = long(csum[0]) * long(nc[1]);
    for (int i = 1; (unsigned long)i < nc.size()-1; ++i)
    {
        csum[i] = csum[i-1] + nc[i];
        res += long(csum[i]) * long(nc[i+1]);
    }
    OUT("csum is " << csum);

    return res;
}

//------------------------------------------------------------------------------

int main()
{
    // main input here
    int n, p;

    std::cin >> n >> p;
    OUT("Have " << n << " astronauts and given " << p << " pairs.");

    Graph G(n);
    for (int i = 0; i < p; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        G.add_edge(u, v);
        G.add_edge(v, u);
    }

//    OUT(G.nodes());
//    OUT(G.edges());

    // count number of partitions, with number of nodes in each
    // represent by vector of ints
    intv nc = G.connected_node_count();
    OUT("nc is " << nc);
    OUT("nc sum is " << sum(nc));

//    // calculate number of pairs
    long ans = calculate_pairs(nc);

    OUT("Answer: " << ans);

    OUT("All visited: " << (G.all_visited() ? "yes" : "no"));

    return 0;
}
