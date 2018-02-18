#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <armadillo>

#define DEBUG

//------------------------------------------------------------------------------
//! Thread safe cout. Prevents cout collision.
struct coutTS : std::ostringstream
{
    coutTS() = default;
    ~coutTS()
    {
        std::lock_guard<std::mutex> _(_mutex);
        std::cout << this->str();
    }
private:
    static std::mutex _mutex;
};
std::mutex coutTS::_mutex;

#define STDOUT(S) coutTS() << S << std::endl

#ifdef DEBUG
#define DOUT(S) coutTS() << "<debug> " << S << std::endl
#else
#define DOUT(S)
#endif

//------------------------------------------------------------------------------
struct TimeThis
{
    using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

    TimeThis(std::function<void()> f)
    {
        auto start = std::chrono::high_resolution_clock::now();
        f();
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = stop - start;
        seconds = elapsed.count();
    }

    double seconds;

    static TimePoint mark() { return std::chrono::high_resolution_clock::now(); }
    static double sdiff(TimePoint start, TimePoint stop) { return std::chrono::duration<double>(stop - start).count(); }
};

//------------------------------------------------------------------------------
using mat = arma::mat;
using cvec = arma::colvec;
using rvec = arma::rowvec;

int main()
{
    unsigned n_threads = std::thread::hardware_concurrency();
    STDOUT("Detected " << n_threads << " supported concurrent threads.");

    /*
     * Create row vector of n points representing fixed values, column vector
     * of m points representing variable values. Each between -1 and 1.
     */
    unsigned m_pts = 9999;
    unsigned n_pts = 10000;
    rvec rowdat = arma::linspace<rvec>(-1., 1., m_pts);
    cvec coldat = arma::linspace<cvec>(-1., 1., n_pts);

    /*
     * Now need to create a matrix defined by 1/(rowvec - colvec).
     */

    // The old way.
    mat I;
    double secs = TimeThis([&]()
            {
                I = arma::repmat(rowdat, coldat.n_elem, 1);
                I.each_col() -= coldat;
                I = 1./I;
            }).seconds;
    DOUT("Old way takes " << secs << " seconds for " << I.n_elem << " points.");
    DOUT("Matrix object has size " << I.n_rows << "x" << I.n_cols << " with memory usage "
            << sizeof(I) + I.n_elem*sizeof(double)/(1024*1024) << "Mb.");

    // Each operation by row.
    mat I2(n_pts, m_pts);
    secs = TimeThis([&]()
            {
                for (unsigned i = 0; i < I2.n_rows; ++i)
                {
                    double k = coldat(i);
                    I2.row(i) = 1./(rowdat - k);
                }
            }).seconds;
    DOUT("By row takes " << secs << " seconds.");
    bool same = approx_equal(I, I2, "absdiff", 1e-14);
    DOUT("By row operation makes same matrix: " << (same ? "true" : "false"));

    /*
     * For thread i in [0, n_threads),
     *   - need n_threads hunks, so each hunk has ni_rows = n_pts/n_threads,
     *     with last hunk having ni_rows + (n_pts - n_threads*ni_rows) rows.
     *   - each thread gets given span of rows to process.
     */
    unsigned ni_rows = n_pts / n_threads;
    unsigned ni_left = n_pts % n_threads;
    DOUT("Each thread hunk has " << ni_rows << " rows with last hunk having "
            << ni_rows + ni_left << " rows.");

    mat Ith(n_pts, m_pts);
    auto do_hunk = [&](unsigned row_0, unsigned row_n)
    {
        for (unsigned i = row_0; i < row_n; ++i)
        {
            double k = coldat(i);
            Ith.row(i) = 1./(rowdat - k);
        }
        DOUT("Rows " << row_0 << " through " << row_n - 1 << " done.");
    };

    using TList = std::vector<std::thread>;
    TList threads;
    auto start = TimeThis::mark();
    for (unsigned i = 0; i < n_threads; ++i)
    {
        unsigned row_0 = i*ni_rows;
        unsigned row_n = row_0 + ni_rows;
        if (i+1 == n_threads) row_n += ni_left;

        DOUT("Starting thread with rows " << row_0 << " through " << row_n - 1);
        threads.push_back(std::thread(do_hunk, row_0, row_n));
    }

    for_each(threads.begin(), threads.end(), [](std::thread& th) { th.join(); });
    auto stop = TimeThis::mark();
    DOUT("Threads took " << TimeThis::sdiff(start, stop) << " secs to do the same job.");

    same = arma::approx_equal(I, Ith, "absdiff", 1e-14);
    DOUT("The two methods have the same result: " << (same ? "true" : "false"));

    return 0;
}
