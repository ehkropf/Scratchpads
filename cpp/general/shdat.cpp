#include <iostream>
#include <memory>
#include <string>

////////////////////////////////////////////////////////////////////////////////
struct ShareData
{
    using Ptr = std::shared_ptr<ShareData>;
    using ConstPtr = std::shared_ptr<const ShareData>;

    virtual ~ShareData() = default;
};

class AData : public ShareData
{
    std::string _string;

public:
    AData() : _string("Some AData") {}

    const std::string& string() const { return _string; }
    void setString(const std::string& ins) { _string = ins; }
    void setString(std::string&& ins) { _string = std::move(ins); }

    friend std::ostream& operator<<(std::ostream& o, const AData& d)
    {
        return o << d._string;
    }
};

////////////////////////////////////////////////////////////////////////////////
class AThing
{
    ShareData::Ptr _data;

public:
    AThing() : _data(std::make_shared<AData>()) {}
    // AThing(const AThing& thing) : _data(thing._data) {}

    const ShareData::Ptr shareDataPtr() const { return _data; }
    const AData& shareData() const
    {
        return *std::dynamic_pointer_cast<AData>(_data);
    }

    friend std::ostream& operator<<(std::ostream& o, const AThing& thing)
    {
        return o << "This thing has data: " << thing.shareData().string()
            << " (count " << thing._data.use_count() << ")";
    }
};

////////////////////////////////////////////////////////////////////////////////
int main()
{
    AThing thing;
    {
        AThing thing2(thing);
        std::cout << thing2 << std::endl;
        std::cout << "Post return count is " << thing2.shareDataPtr().use_count() << std::endl;;

        const AData& data = thing2.shareData();
        std::cout << "Made a reference to 2s shared data: " << data.string() << std::endl;
        std::cout << thing2 << std::endl;
    }
    std::cout << "Got rid of thing2" << std::endl;
    std::cout << thing << std::endl;
    std::cout << "Post return count is " << thing.shareDataPtr().use_count() << std::endl;;

    return 0;
}
