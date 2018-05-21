#pragma once

/*!
 *  Btree.hpp
 *
 *  A binary tree implementation for hackerrank challenges.
 */

#include <iostream>
#include <memory>
#include <queue>
#include <cassert>

#ifndef OUT
#ifndef NOOUT
#define OUT(S) std::cout << S << std::endl
#else
#define OUT(S)
#endif
#endif

//--------------------------------------------------------------------------
struct Bnode
{
    using shared_ptr = std::shared_ptr<Bnode>;
    static shared_ptr make_shared(int d) { return std::make_shared<Bnode>(d); }

    int data;
    shared_ptr left;
    shared_ptr right;

    Bnode(int d) : data(d) {}

    friend std::ostream& operator<<(std::ostream& os, const Bnode& n);

    using PtrQueue = std::queue<Bnode::shared_ptr>;
    using PtrRefQueue = std::queue<Bnode::shared_ptr&>;

protected:
    //! Worker function for operator<<().
    std::ostream& print(std::ostream& os, std::string prefix, bool isTail) const
    {
        if (right) right->print(os, prefix + (isTail ? "│  " : "   "), false);
        os << prefix << (isTail ? "└─ " : "┌─ ") << data << std::endl;
        if (left) left->print(os, prefix + (isTail ? "   " : "│  "), true);
        return os;
    }
};

//! Descend from node to send "sideways formatted" binary tree to stdout.
std::ostream& operator<<(std::ostream& os, const Bnode& n)
{
    return n.print(os, "", true);
}

//--------------------------------------------------------------------------
class Btree
{
    Bnode::shared_ptr m_root;

protected:
    //! Recursive order insert.
    static Bnode::shared_ptr insert(Bnode::shared_ptr root, int d)
    {
        if (!root) return Bnode::make_shared(d);

        Bnode::shared_ptr cur;
        if (d <= root->data)
        {
            cur = insert(root->left, d);
            root->left = cur;
        }
        else
        {
            cur = insert(root->right, d);
            root->right = cur;
        }

        return root;
    }

    //! Work function for preOrder() traversal.
    static void preOrder(Bnode::shared_ptr root)
    {
        std::cout << root->data << " ";
        if (root->left) preOrder(root->left);
        if (root->right) preOrder(root->right);
    }

    //! Helper function for topView().
    static void topLeft(Bnode::shared_ptr n, std::string& s)
    {
        if(n->left) topLeft(n->left, s);
        s += std::to_string(n->data) + " ";
    }

    //! Helper function for topView().
    static void topRight(Bnode::shared_ptr n, std::string& s)
    {
        s += " " + std::to_string(n->data);
        if(n->right) topRight(n->right, s);
    }

public:
    //! Create from stdin.
    static Btree from_cin(int N)
    {
        Btree T;
        while (N--)
        {
            int d;
            std::cin >> d;
            T.insert(d);
        }
        return T;
    }

    //! Ordered tree insert.
    void insert(int d)
    {
        if (!m_root)
            m_root = Bnode::make_shared(d);
        else
            insert(m_root, d);
    }

    //! Send pre-order traversal to stdout.
    void preOrder() const { preOrder(m_root); std::cout << std::endl; }

    //! Shows "top" view of tree from left to right.
    std::string topView() const
    {
        std::string s;
        if (m_root->left) topLeft(m_root->left, s);
        s += std::to_string(m_root->data);
        if (m_root->right) topRight(m_root->right, s);

        return s;
    }

    //! BFS level traversal (L->R) to stdout.
    void levelTraversal() const
    {
        Bnode::PtrQueue q;
        q.push(m_root);

        while (q.size())
        {
            Bnode::shared_ptr& n = q.front();
            if (n->left) q.push(n->left);
            if (n->right) q.push(n->right);
            std::cout << n->data << " ";
            q.pop();
        }
        std::cout << std::endl;
    }

    friend std::ostream& operator<<(std::ostream&, const Btree&);
};

//! Output sideways btree.
std::ostream& operator<<(std::ostream& os, const Btree& T)
{
    return os << *T.m_root;
}
