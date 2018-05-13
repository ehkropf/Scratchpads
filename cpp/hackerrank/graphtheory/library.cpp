#include <iostream>
#include <vector>
#include <queue>

#define COUT(S) std::cout << S << std::endl

struct Node
{
    int last_edge;
    bool visited;

    Node() : last_edge(-1), visited(false) {}
};

struct Edge
{
    int to_node;
    int next_edge;
};

using Edges = std::vector<Edge>;
using Nodes = std::vector<Node>;

void print(int n, int m, int c_lib, int c_road)
{
    COUT("n = " << n << ", m = " << m);
    COUT("c_lib = " << c_lib << ", c_road = " << c_road);
}

//int roadsAndLibraries(int n, int c_lib, int c_road, vector < vector<int> > cities) {
long roadsAndLibraries(int n, int c_lib, int c_road, const Edges& E, Nodes& N)
{
    long cost = 0;

    // BFS from each node.
    std::queue<int> Q;
    for (int i = 0; i < n; ++i)
    {
        if (N[i].visited) continue;
        N[i].visited = true;
        cost += c_lib;
        COUT("\nVisited " << i);
        Q.push(i);
        while (Q.size())
        {
            for (int k = N[Q.front()].last_edge; k != -1; k = E[k].next_edge)
            {
                int j = E[k].to_node;
                if (N[j].visited) continue;
                N[j].visited = true;
                cost += c_road;
                COUT("Visited " << j);
                Q.push(j);
            }
            Q.pop();
        }
    }

    return cost;
}

int main() {
    int q; // 1 <= q <= 10
    std::cin >> q;

    for(int a0 = 0; a0 < q; a0++)
    {
        COUT("+++++=== q = " << a0 << " ===+++++");
        // 1 <= n <= 1e5
        // 0 <= m <= min(1e5, n*(n-1)/2)
        // 1 <= c_lib <= 1e5
        // 1 <= c_road <= 1e5
        int n, m, c_lib, c_road;
        std::cin >> n >> m >> c_lib >> c_road;
        print(n, m, c_lib, c_road);

        if (c_lib < c_road || m == 0)
        {
            // Could just ignore for speed, but whatever.
            for (int i = 0; i < m; ++i)
            {
                int u, v;
                std::cin >> u >> v;
            }

            std::cout << std::endl << long(n)*long(c_lib) << std::endl;
            continue;
        }

        // Input is 1-indexed; graph in memory is 0-indexed.
        Nodes N(n);
        Edges E(2*m);
        for (int i = 0; i < m; ++i)
        {
            int u, v; // 1 <= u,v <= n
            std::cin >> u >> v;

            // Outgoing edge
            E[i].to_node = v-1;
            E[i].next_edge = N[u-1].last_edge;
            N[u-1].last_edge = i;

            // Incoming edge
            E[m+i].to_node = u-1;
            E[m+i].next_edge = N[v-1].last_edge;
            N[v-1].last_edge = m+i;
        }

        unsigned result = roadsAndLibraries(n, c_lib, c_road, E, N);

        std::cout << std::endl << result << std::endl;
    }

    return 0;
}
