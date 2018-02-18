#include <chrono>
#include <functional>

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
