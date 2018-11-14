#include <iostream>
#include <fstream>
#include <deque>
#include <iomanip>
#include <string>

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

        OUT("heapify at " << i << ":");
        if (l < n && arr[l] < arr[imin])
        {
            OUT("  at l < at imin");
            imin = l;
        }
        if (r < n && arr[r] < arr[imin])
        {
            OUT("  at r < at imin");
            imin = r;
        }
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

public:
    static MinHeap given_array(const intv& v)
    {
        MinHeap h;
        intv&  arr= h.get_arr();
        arr = v;
        return h;
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

public:
    static MaxHeap given_array(const intv& v)
    {
        MaxHeap h;
        intv& arr= h.get_arr();
        arr = v;
        return h;
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
int main()
{
    auto hout = [](std::string label, Heap* h)
    {
        auto tf = [](bool v) -> char { return v ? '/' : 'X'; };
        OUT("  " << label << " (" << tf(h->heap_check()) << ")("
                << h->size() << ") -- " << *h);
    };

    int add = 78775;
    intv minh_in{ 47620, 71415, 51310, 56623, 48240, 69067, 59597, 57256, 60326, 57064, 90403, 74518, 51299, 53125, 70521, 63131, 55461, 60757, 60445, 73634, 71859, 67599, 94063, 77519, 58281, 68921, 83478, 73952, 81530, 53649, 69230, 63772, 71057, 74030, 67294, 61382, 62995, 74879, 78563, 94807, 75425, 98296, 92661, 87127, 98074, 89952, 71709, 98041, 77196, 83312, 78164, 96066, 95912, 93793, 92211, 88179, 89659, 86940, 93644, 86287, 97446, 92041, 91127, 93624, 74725, 76966, 94455, 85116, 96402, 81638, 88540, 98764, 77763, 88830, 55170, 70076, 82531 };
    intv maxh_in{ 47560, 47005, 46981, 42707, 40776, 28659, 38816, 45843, 45484, 35597, 44001, 45170, 26019, 26464, 31306, 39001, 41510, 17073, 43245, 15897, 19486, 24394, 16843, 30118, 40447, 31718, 13751, 30050, 22986, 23833, 23759, 36827, 17502, 15237, 22120, 37105, 16598, 20535, 21499, 22934, 10914, 1013, 6823, 13837, 35032, 10945, 13401, 13449, 31598, 1817, 12766, 22016, 31450, 30409, 148, 2973, 1481, 4738, 5799, 11113, 1711, 10115, 20555, 13343, 17368, 18784, 29941, 7920, 7084, 3033, 2698, 16571, 19709, 22462, 5415, 17738 };

    MinHeap nh = MinHeap::given_array(minh_in);
    MaxHeap xh = MaxHeap::given_array(maxh_in);

    OUT("\nAdd " << add);
    nh.insert(add);
    hout("minheap", &nh);

    OUT("\nRemove " << nh.root());
    int remove = nh.pop();
    hout("minheap", &nh);

    OUT("\nAdd " << remove);
    xh.insert(remove);
    hout("maxheap", &xh);

    return 0;
}
