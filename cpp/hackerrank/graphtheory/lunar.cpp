#include <iostream>
#include <queue>
#include <vector>
#include <cassert>

#define OUT(S) std::cout << S << std::endl;
//#define OUT(S)

//--------------------------------------------------------------------------
using intv = std::vector<int>;

int sum(const intv& nv)
{
    int sum = 0;
    for (int i : nv) sum += i;
    return sum;
}

std::ostream& operator<<(std::ostream& os, const intv& v)
{
    for (int i : v) os << i << " ";
    return os;
}

//--------------------------------------------------------------------------
struct Node
{
    bool visited;
    int last_edge;
    int id;

    Node() : visited(false), last_edge(-1), id(-1) {}
};

using Nodes = std::vector<Node>;

Nodes init_nodes(int size)
{
    Nodes nv(size);
    int i = 0;
    // Vector elements are guaranteed contiguous.
    for (Node& n : nv)
    {
        n.id = i++;
    }

    return nv;
}

std::ostream& operator<<(std::ostream& os, const Nodes& nv)
{
    os << "\n i | last" << std::endl;;
    os <<   "---+-----" << std::endl;
    for (const Node& n : nv)
    {
        os << " " << n.id << " |  " << n.last_edge << std::endl;
    }

    return os;
}

//--------------------------------------------------------------------------
struct Edge
{
    int to_node;
    int next_edge;
    int id;

    Edge() : to_node(-1), next_edge(-1), id(-1) {}
};

using Edges = std::vector<Edge>;
std::ostream& operator<<(std::ostream& os, const Edges& ev)
{
    os << "\n i | to | next" << std::endl;
    os <<   "---+----+-----" << std::endl;
    int i = 0;
    for (const Edge& e : ev)
    {
        os << " " << i++ << " |  " << e.to_node << " |  " << e.next_edge << std::endl;
    }

    return os;
}

//--------------------------------------------------------------------------
class Graph
{
    Nodes m_nodes;
    Edges m_edges;

protected:
    void reset_visited()
    {
        for (Node& n : m_nodes) n.visited = false;
    }

public:
    Graph(int nn) : m_nodes(init_nodes(nn)) {}

    const Nodes& nodes() const { return m_nodes; }
    const Edges& edges() const { return m_edges; }

    void add_edge(int from, int to)
    {
        Nodes& N = m_nodes;

        m_edges.emplace_back();
        int ei = m_edges.size() - 1;
        Edge& e = m_edges.back();

        //OUT("Edge " << ei << " from " << from << " to " << to);

        e.to_node = to;
        e.next_edge = N[from].last_edge;
        N[from].last_edge = ei;
        e.id = ei;
    }

    intv connected_node_count()
    {
        intv nc;
        Nodes& N = m_nodes;
        Edges& E = m_edges;

        for (Node& ni : m_nodes)
        {
            int c = 0;
            std::queue<Node*> q;

            if (ni.visited) continue;
            ni.visited = true;
            ++c;
            q.push(&ni);

            // bfs over each component; count entries
            while (q.size())
            {
                for (int e = q.front()->last_edge; e != -1; e = E[e].next_edge)
                {
                    //OUT("Along edge " << e);
                    int n = E[e].to_node;
                    assert(n == N[n].id);
                    if (N[n].visited) continue;
                    N[n].visited = true;
                    ++c;
                    q.push(&N[n]);
                }
                q.pop();
            }
            //OUT("Counted " << c);
            nc.push_back(c);
        }

        return nc;
    }

    bool all_visited()
    {
        for (const Node& ni : m_nodes)
            if (!ni.visited)
            {
                OUT("Missed node " << ni.id);
                return false;
            }
        return true;
    }
};

//--------------------------------------------------------------------------
long calculate_pairs(const intv& nc)
{
    OUT("Components: " << nc.size());
    if (nc.size() < 2) return 0;

    intv csum(nc.size() - 1);
    csum[0] = nc[0];
    long res = long(csum[0]) * long(nc[1]);
    for (int i = 1; i < nc.size()-1; ++i)
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
