#include <iostream>
#include <vector>

using namespace std;

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

//int roadsAndLibraries(int n, int c_lib, int c_road, vector < vector<int> > cities) {
int roadsAndLibraries(int n, int c_lib, int c_road, const Edges& E, const Nodes& N)
{
    COUT("n = " << n);
    COUT("c_lib = " << c_lib << ", c_road = " << c_road);

    // Adjacency list:
    COUT("\nEdges\n========");
    for (int i = 0; i < E.size(); ++i)
    {
        COUT(E[i].to_node << " " << E[i].next_edge);
    }

    COUT("\nNodes\n========");
    for (int i = 0; i < N.size(); ++i)
    {
        COUT(N[i].last_edge);
    }

    return -2;
}

int main() {
    int q; // 1 <= q <= 10
    cin >> q;

    for(int a0 = 0; a0 < q; a0++){
        COUT("+++++=====+++++");
        int n; // 1 <= n <= 1e5
        int m; // 0 <= m <= min(1e5, n*(n-1)/2)
        int c_lib; // 1 <= c_lib <= 1e5
        int c_road; // 1 <= c_road <= 1e5
        cin >> n >> m >> c_lib >> c_road;

        if (c_lib < c_road)
        {
            cout << n*c_lib << endl;
            continue;
        }

        Nodes N(n);
        Edges E(2*m);
        for (int i = 0; i < m; ++i)
        {
            int u, v; // 1 <= u,v <= n
            cin >> u >> v;

            // Outgoing edge
            E[i].to_node = v-1;
            E[i].next_edge = N[v-1].last_edge;
            N[v-1].last_edge = i;

            // Incoming edge
            E[m+i].to_node = u-1;
            E[m+i].next_edge = N[u-1].last_edge;
            N[u-1].last_edge = m+i;
        }

        int result = roadsAndLibraries(n, c_lib, c_road, E, N);

//        vector< vector<int> > cities(m,vector<int>(2));
//        for(int cities_i = 0;cities_i < m;cities_i++){
//           for(int cities_j = 0;cities_j < 2;cities_j++){
//              cin >> cities[cities_i][cities_j];
//           }
//        }
//        int result = roadsAndLibraries(n, c_lib, c_road, cities);
        cout << result << endl;
    }

    return 0;
}
