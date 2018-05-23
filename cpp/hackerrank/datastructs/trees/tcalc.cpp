#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

#define OUT(S) std::cout << S << std::endl

//--------------------------------------------------------------------------
class Tree
{
public:
    struct Node;
    using Nodes = std::vector<std::shared_ptr<Node>>;

    struct Node
    {
        using shared_ptr = std::shared_ptr<Node>;
        static shared_ptr make_shared(int k) { return std::make_shared<Node>(k); }

        int key;
        Nodes nodes;

        Node(int k) : key(k) {}
    };

private:
    Node::shared_ptr m_root;

protected:
    std::ostream& print(std::ostream& os, Node::shared_ptr root, std::string prefix, bool isTail) const
    {
        os << prefix << (isTail ? "└─ " : "├─ ") << root->key << std::endl;
        prefix += isTail ? "   " : "│  ";

        Nodes::const_iterator ni = root->nodes.begin(), end = root->nodes.end();
        while (ni != end)
            print(os, *ni, prefix, ++ni == end);
        return os;
    }

public:
    //! Add key k at node n.
    void addAt(int k, int n)
    {
        if (!m_root) m_root = Node::make_shared(1);
        Node::shared_ptr at = find(n);
        assert(at != nullptr);
        at->nodes.push_back(Node::make_shared(k));
    }

    //! Find node with key k.
    Node::shared_ptr find(int k, Node::shared_ptr root = nullptr) const
    {
        if (!root) root = m_root;
        if (root->key == k) return root;
        for (auto node : root->nodes)
            if (Node::shared_ptr res = find(k, node)) return res;
        return nullptr;
    }

    //! Depth of v from root.
    int depth(int v, Node::shared_ptr root = nullptr, int level = 0) const
    {
        if (!root) root = m_root;
        if (root->key == v) return level;
        for (auto node : root->nodes)
        {
            int res = depth(v, node, level+1);
            if (res > -1) return res;
        }
        return -1;
    }

    //! Lowest common ancestor of v1, v2.
    Node::shared_ptr lca(int v1, int v2, Node::shared_ptr root = nullptr) const
    {
        if (!root) root = m_root;
        if (root->key == v1 || root->key == v2) return root;

        Node::shared_ptr res = nullptr;
        for (auto n : root->nodes)
        {
            Node::shared_ptr tmp = lca(v1, v2, n);
            if (res && tmp) return root;
            if (tmp) res = tmp;
        }
        return res;
    }

    //! Distance between two nodes.
    int dist(int v1, int v2) const
    {
        Node::shared_ptr n = lca(v1, v2);
        return depth(v1, n) + depth(v2, n);
    }

    //! Create tree from std::cin.
    static Tree fromCin(int n)
    {
        Tree T;
        while (--n)
        {
            int v, k;
            std::cin >> v >> k;
            T.addAt(k, v);
        }
        return T;
    }

    friend std::ostream& operator<<(std::ostream&, const Tree&);
};

std::ostream& operator<<(std::ostream& os, const Tree& T)
{
    return T.print(os, T.m_root, "", true);
}

//--------------------------------------------------------------------------
using intv = std::vector<int>;

std::ostream& operator<<(std::ostream& os, intv& v)
{
    intv::const_iterator vi = v.begin(), end = v.end();
    while (vi != end)
        os << *vi << (++vi == end ? "" : " ");
    return os;
}

//--------------------------------------------------------------------------
int main()
{
    int n, q;
    std::cin >> n >> q;
    Tree T = Tree::fromCin(n);

    OUT(T);

    for (int i = 0; i < q; ++i)
    {
        int k;
        std::cin >> k;
        //OUT("Use k=" << k << " values for pairs");
        intv pv;
        for (int j = 0; j < k; ++j)
        {
            int p;
            std::cin >> p;
            pv.push_back(p);
        }
        //OUT("values: " << pv);

        int ans = 0;
        if (pv.size())
        {
            intv::const_iterator pi = pv.begin(), end = pv.end();
            while (pv.size() && pi+1 != end)
            {
                intv::const_iterator p2 = pi + 1;
                while (p2 != end)
                {
                    int v1 = *pi, v2 = *p2++;
                    //OUT("{" << v1 << "," << v2 << "}");

                    int s = T.dist(v1, v2);
                    //OUT("Dist: " << T.dist(v1, v2));

                    ans += v1*v2*s;
                }
                ++pi;
            }
            ans %= int(1e9 + 7);

            std::cout << ans << std::endl;
        }
    }

    return 0;
}
