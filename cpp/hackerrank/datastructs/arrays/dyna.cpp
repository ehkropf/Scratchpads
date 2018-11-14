#include <iostream>
#include <vector>

#define OUT(S) // std::cout << S << std::endl

using Seq = std::vector<int>;
using SeqList = std::vector<Seq>;

//--------------------------------------------------------------------------
struct ProblemData
{
    int N;
    SeqList seqList;
    int lastAnswer;

    ProblemData(int N) : N(N), seqList(N), lastAnswer(0) {}

    int query(int qt, int x, int y)
    {
        switch (qt)
        {
            case 1:
                queryOne(x, y);
                break;

            case 2:
                queryTwo(x, y);
                break;

            default:
                std::cerr << "Unknown query type " << qt << std::endl;
                return -1;
        }

        return 0;
    }

    int xindex(int x) const { return (x ^ lastAnswer) % N; }

    void queryOne(int x, int y)
    {
        int i = xindex(x);
        seqList[i].push_back(y);
        OUT("Q1: push back " << y << " at " << i);
    }

    void queryTwo(int x, int y)
    {
        int i = xindex(x);
        lastAnswer = seqList[i][y % seqList[i].size()];
        OUT("Q2: assign to lastAnswer from [" << i << "][" << y % seqList[i].size() << "]");
        std::cout << lastAnswer << std::endl;
    }
};

std::ostream& operator<<(std::ostream& os, const ProblemData& d)
{
    std::cout << "Problem:\nN = " << d.N << "\nlastAnswer = " << d.lastAnswer;

    return os;
}

//--------------------------------------------------------------------------
int main()
{
    int N, Q;
    std::cin >> N >> Q;
    ProblemData data(N);
    OUT(data);

    for (int q = 0; q < Q; ++q)
    {
        int qt, x, y;
        std::cin >> qt >> x >> y;
        if (data.query(qt, x, y)) return -1;
    }

    return 0;
}
