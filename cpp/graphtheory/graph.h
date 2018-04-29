#pragma once

#include <memory>
#include <vector>

//------------------------------------------------------------------------------
//! Vertex (1d) class.

class Vertex
{
public:
    using id_type = unsigned;
    using shared_ptr = std::shared_ptr<Vertex>;
};

using Vertices = std::vector<Vertex>;


//------------------------------------------------------------------------------
//! Edge class.
//  An edge connects two vertices.

class Edge
{
    Vertex::id_type a;
    Vertex::id_type b;

public:
    Edge(Vertex::id_type, Vertex::id_type);
};

using Edges = std::vector<Edge>;


//------------------------------------------------------------------------------
//! Graph class.
//  A graph is a collection of vertices and edges.

class Graph
{
    Vertices m_v;
    Edges m_e;

public:
    Graph() = default;
    Graph(Vertices, Edges);

    const Vertices& vertices() const;
    void set_vertices(Vertices);
    void add_vertex(const Vertex&);
    void add_vertex(Vertex&&);

    const Edges& edges() const;
    void set_edges(Edges);
    void add_edge(const Edge&);
    void add_edge(Edge&&);
};
