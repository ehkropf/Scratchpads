#include <iostream>
#include <fstream>
#include <deque>
#include <iomanip>

#define OUT(S) std::cout << S << std::endl;

//--------------------------------------------------------------------------
using intv = std::deque<int>;

std::ostream& operator<<(std::ostream& os, const intv& v)
{
    intv::const_iterator vi = v.begin(), end = v.end();
    while (vi != end)
    {
        os << *vi;
        if (++vi != end) os << " ";
    }
    return os;
}

//--------------------------------------------------------------------------
class Heap
{
    //! Heap array (dequeue).
    intv m_arr;

protected:
    //! Protected array access.
    intv& get_arr() { return m_arr; }

    //! Parent index.
    int parent(int i) { return (i - 1)/2; }

    //! Left child index.
    int left(int i) { return 2*i + 1; }

    //! Right child index.
    int right(int i) { return 2*i + 2; }

    //! Abstract sift-up from index i.
    virtual void sift_up(int i) = 0;

    //! Abstract heapify from index i.
    virtual void heapify(int i) = 0;

    //! Heap check work function.
    virtual bool _heap_check(int i) = 0;

public:
    //! Insert into heap. O(log n)
    void insert(int n)
    {
        m_arr.emplace_back(n);
        sift_up(m_arr.size() - 1);
    }

    //! Remove root element from heap.
    int pop()
    {
        int root = m_arr.front();
        std::swap(m_arr.front(), m_arr.back());
        m_arr.pop_back();
        heapify(0);

        return root;
    }

    //! Peek at root element.
    int root() const { return m_arr[0]; }

    //! Heap size -- number of elements.
    size_t size() const { return m_arr.size(); }

    //! Heap check.
    bool heap_check() { return _heap_check(0); }

    friend std::ostream& operator<<(std::ostream&, const Heap&);
};

std::ostream& operator<<(std::ostream& os, const Heap& h)
{
    os << h.m_arr;
    return os;
}

//--------------------------------------------------------------------------
class MinHeap : public Heap
{
protected:
    //! Sift-up. Relies on i being valid index.
    virtual void sift_up(int i)
    {
        intv& arr = get_arr();
        while (i && arr[parent(i)] > arr[i])
        {
            std::swap(arr[parent(i)], arr[i]);
            i = parent(i);
        }
    }

    //! Recursive down-heapify. Relies on subtree satisfying heap property.
    virtual void heapify(int i)
    {
        intv& arr = get_arr();
        int n = arr.size();
        int imin = i;
        int l = left(i);
        int r = right(i);

        if (l < n && arr[l] < arr[imin]) imin = l;
        if (r < n && arr[r] < arr[imin]) imin = r;
        if (imin != i)
        {
            std::swap(arr[i], arr[imin]);
            heapify(imin);
        }
    }

    virtual bool _heap_check(int i)
    {
        intv& v = get_arr();
        int n = v.size();
        int l = left(i);
        int r = right(i);

        if (l < n)
        {
            if (v[l] < v[i])
                return false;
            else
                _heap_check(l);
        }
        if (r < n)
        {
            if (v[r] < v[i])
                return false;
            else
                _heap_check(r);
        }
        return true;
    }
};

//--------------------------------------------------------------------------
class MaxHeap : public Heap
{
protected:
    //! Sift-up. Relies on i being valid index.
    virtual void sift_up(int i)
    {
        intv& arr = get_arr();
        while (i && arr[parent(i)] < arr[i])
        {
            std::swap(arr[parent(i)], arr[i]);
            i = parent(i);
        }
    }

    //! Recursive down-heapify. Relies on subtree satisfying heap property.
    virtual void heapify(int i)
    {
        intv& arr = get_arr();
        int n = arr.size();
        int imax = i;
        int l = left(i);
        int r = right(i);

        if (l < n && arr[imax] < arr[l]) imax = l;
        if (r < n && arr[imax] < arr[r]) imax = r;
        if (imax != i)
        {
            std::swap(arr[i], arr[imax]);
            heapify(imax);
        }
    }

    virtual bool _heap_check(int i)
    {
        intv& v = get_arr();
        int n = v.size();
        int l = left(i);
        int r = right(i);

        if (l < n)
        {
            if (v[l] > v[i])
                return false;
            else
                _heap_check(l);
        }
        if (r < n)
        {
            if (v[r] > v[i])
                return false;
            else
                _heap_check(r);
        }
        return true;
    }
};

//--------------------------------------------------------------------------
struct Problem
{
    MaxHeap xh;
    MinHeap nh;

    void add(int v)
    {
        if (!nh.size() || nh.root() < v)
            nh.insert(v);
        else
            xh.insert(v);

        // Balance.
        int dh = nh.size() - xh.size();
        if (dh == 2)
            xh.insert(nh.pop());
        else if (dh == -2)
            nh.insert(xh.pop());
    }

    float mean() const
    {
        size_t ne = nh.size(), xe = xh.size();
        if (ne > xe)
            return float(nh.root());
        else if (xe > ne)
            return float(xh.root());
        else
            return float(nh.root() + xh.root())/2.;
    }

    size_t size() const { return xh.size() + nh.size(); }
};

std::ostream& operator<<(std::ostream& os, Problem& P)
{
    auto tf = [](bool tf) -> char { return tf ? '/' : 'X'; };
    os << "  minheap (" << tf(P.nh.heap_check()) << ")(" << P.nh.size()
        << ") -- " << P.nh << std::endl;
    os << "  maxheap (" << tf(P.xh.heap_check()) << ")(" << P.xh.size()
        << ") -- " << P.xh << std::endl;
    return os;
}

//--------------------------------------------------------------------------
int main(int, char** argv)
{
    std::ifstream ins(argv[1]);

    int n;
    ins >> n;

    Problem P;
    while (n--)
    {
        int v;
        ins >> v;

        P.add(v);
        std::cout << std::setprecision(1) << std::fixed << P.mean() << std::endl;
    }

    ins.close();
    return 0;
}
