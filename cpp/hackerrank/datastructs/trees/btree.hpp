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
    using RawPtrQueue = std::queue<const Bnode*>;

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
    static Bnode::shared_ptr bstAdd(Bnode::shared_ptr root, int d)
    {
        if (!root) return Bnode::make_shared(d);

        Bnode::shared_ptr cur;
        if (d <= root->data)
        {
            cur = bstAdd(root->left, d);
            root->left = cur;
        }
        else
        {
            cur = bstAdd(root->right, d);
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

    //! Find node with data value in unordered btree using pre-order DFS.
    Bnode::shared_ptr unFind(int v, Bnode::shared_ptr& root)
    {
        OUT("Find " << v << " now at " << root->data);
        if (root->data == v) return root;

        Bnode::shared_ptr res;
        if (root->left) res = unFind(v, root->left);

        if (res->data != v && root->right) return unFind(v, root->right);

        return res;
    }

public:
    //! Ordered tree insert.
    void bstAdd(int d)
    {
        if (!m_root)
            m_root = Bnode::make_shared(d);
        else
            bstAdd(m_root, d);
    }

    //! Unordered tree insert.
    void addAt(int v, int l, int r)
    {
        if (!m_root) m_root = Bnode::make_shared(1);
        Bnode::shared_ptr node = unFind(v, m_root);
        assert(!(node->left || node->right));
        if (l > 0) node->left = Bnode::make_shared(l);
        if (r > 0) node->right = Bnode::make_shared(r);
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
        Bnode::RawPtrQueue q;
        q.push(m_root.get());

        while (q.size())
        {
            const Bnode* n = q.front();
            if (n->left) q.push(n->left.get());
            if (n->right) q.push(n->right.get());
            std::cout << n->data << " ";
            q.pop();
        }
        std::cout << std::endl;
    }

    //! Create BST from stdin.
    static Btree bstFromCin(int N)
    {
        Btree T;
        while (N--)
        {
            int d;
            std::cin >> d;
            T.bstAdd(d);
        }
        return T;
    }

    //! Create unordered btree from stdin.
    static Btree unFromCin(int N)
    {
        Btree T;
        int v = 0;
        while (N--)
        {
            int l, r;
            std::cin >> l >> r;
            T.addAt(++v, l, r);
        }

        return T;
    }

    friend std::ostream& operator<<(std::ostream&, const Btree&);
};

//! Output sideways btree.
std::ostream& operator<<(std::ostream& os, const Btree& T)
{
    return os << *T.m_root;
}
