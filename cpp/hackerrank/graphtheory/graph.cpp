#include "graph.h"

//------------------------------------------------------------------------------
//Vertex::Vertex()

//------------------------------------------------------------------------------
Edge::Edge(Vertex::id_type a, Vertex::id_type b)
    : a(a), b(b)
{}


//------------------------------------------------------------------------------
//bool operator==(const Vertex& a, const Vertex& b)
//{
//    return &a == &b ? true : false;
//}

//------------------------------------------------------------------------------
Graph::Graph(Vertices v, Edges e)
    : m_v(v), m_e(e)
{}


//------------------------------------------------------------------------------
const Vertices&
Graph::vertices() const
{
    return m_v;
}

void
Graph::set_vertices(Vertices v)
{
    m_v = v;
}

void
Graph::add_vertex(const Vertex& v)
{
    m_v.push_back(v);
}

void
Graph::add_vertex(Vertex&& v)
{
    m_v.push_back(v);
}


//------------------------------------------------------------------------------
const Edges&
Graph::edges() const
{
    return m_e;
}

void
Graph::set_edges(Edges e)
{
    m_e = e;
}

void
Graph::add_edge(const Edge& e)
{
    m_e.push_back(e);
}

void
Graph::add_edge(Edge&& e)
{
    m_e.push_back(e);
}
