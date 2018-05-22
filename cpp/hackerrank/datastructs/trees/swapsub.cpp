#include <iostream>
#include <memory>

//--------------------------------------------------------------------------
struct Bnode
{
    using shared_ptr = std::shared_ptr<Bnode>;
    static shared_ptr make_shared(int d) { return std::make_shared<Bnode>(d); }

    int data;
    shared_ptr left;
    shared_ptr right;

    Bnode(int d) : data(d) {}
};

//--------------------------------------------------------------------------
class Btree
{
    Bnode::shared_ptr m_root;

protected:
    //! Work function for inOrder() traversal.
    static void inOrder(const Bnode::shared_ptr& root)
    {
        if(root->left) inOrder(root->left);
        std::cout << root->data << " ";
        if(root->right) inOrder(root->right);
    }

    //! Work function for preOrder() traversal.
    static void preOrder(Bnode::shared_ptr root)
    {
        std::cout << root->data << " ";
        if (root->left) preOrder(root->left);
        if (root->right) preOrder(root->right);
    }

    //! Find node with data value in unordered btree using pre-order DFS.
    Bnode::shared_ptr unFind(int v, Bnode::shared_ptr& root)
    {
        if (root->data == v) return root;

        if (root->left)
            if (Bnode::shared_ptr result = unFind(v, root->left)) return result;
        if (root->right)
            if (Bnode::shared_ptr result = unFind(v, root->right)) return result;

        return nullptr;
    }

    //! Recursive K-swap.
    void KSwap(int K, int level, Bnode::shared_ptr& root)
    {
        // Descend tree pre-order. At levels K, 2*K, 3*K, ..., swap child nodes L<->R.
        if (level % K == 0) std::swap(root->left, root->right);
        if (root->left) KSwap(K, level+1, root->left);
        if (root->right) KSwap(K, level+1, root->right);
    }

public:
    //! Unordered tree insert.
    void addAt(int v, int l, int r)
    {
        if (!m_root) m_root = Bnode::make_shared(1);
        Bnode::shared_ptr node = unFind(v, m_root);
        assert(node && !(node->left || node->right));
        if (l > 0) node->left = Bnode::make_shared(l);
        if (r > 0) node->right = Bnode::make_shared(r);
    }

    void KSwap(int K) { KSwap(K, 1, m_root); }

    //! In-order traversal to stdout.
    void inOrder() const { inOrder(m_root); std::cout << std::endl; }

    //! Create unordered btree from stdin.
    static Btree unFromCin(int N)
    {
        Btree T;
        for (int v = 1; v <= N; ++v)
        {
            int l, r;
            std::cin >> l >> r;
            T.addAt(v, l, r);
        }

        return T;
    }

    friend std::ostream& operator<<(std::ostream&, const Btree&);
};

//--------------------------------------------------------------------------
int main()
{
    int n;
    std::cin >> n;

    Btree tr = Btree::unFromCin(n);

    int T;
    std::cin >> T;
    while (T--)
    {
        int K;
        std::cin >> K;
        tr.KSwap(K);
        tr.inOrder();
    }

    return 0;
}
