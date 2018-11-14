#pragma once

#include "out.h"

#include <iostream>
#include <vector>
#include <queue>
#include <sstream>

#ifndef OUT
#define OUT(S)
#endif

//------------------------------------------------------------------------------
using intv = std::vector<int>;

int sum(const intv& nv)
{
    int sum = 0;
    for (int i : nv) sum += i;
    return sum;
}

std::ostream& operator<<(std::ostream& os, const intv& v)
{
    intv::const_iterator vi(v.begin()), end(v.end());
    while (vi != end)
    {
       os << *vi;
       if (++vi != end) os << " ";
    }
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
