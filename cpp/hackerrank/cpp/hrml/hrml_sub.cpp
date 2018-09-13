#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <map>

//------------------------------------------------------------------------------
#define OUT(S) std::cout << S << std::endl

//------------------------------------------------------------------------------
std::string get_token(std::string& str, const std::string& delim)
{
    std::string token;
    size_t dpos;
    if ((dpos = str.find(delim)) != std::string::npos)
    {
        token = str.substr(0, dpos);
        str.erase(0, dpos + delim.size());
    }

    return token;
}

//------------------------------------------------------------------------------

class Tag
{
public:
    using Tags = std::map<std::string, Tag>;
    using Attributes = std::map<std::string, std::string>;

private:
    std::string m_name;
    Attributes m_attributes;
    Tags m_subtags;

public:
    Tag() = default;
    Tag(const std::string& name) : m_name(name) {}

    const std::string& name() const { return m_name; }
    const Attributes& attributes() const { return m_attributes; }
    const Tag::Tags& subtags() const { return m_subtags; }

    void add_attribute(const std::string& name, const std::string& value)
    {
        m_attributes[name] = value;
    }

    void add_subtag(Tag tag)
    {
        m_subtags[tag.name()] = tag;
    }
};

//------------------------------------------------------------------------------
class HRML
{
    Tag::Tags m_tags;

private:
    void set_tags(std::istream& in, int& N, Tag* p_open_tag = nullptr)
    {
        while (N-- > 0)
        {
            std::string line(getline(in));

            if (line.substr(0, 2) == "</")
            {
                return;
            }

            // Have a tag for sure, but does it have attributes?
            line.erase(0, 1);
            line.erase(line.size()-1, 1);
            std::string tok = get_token(line, " ");
            Tag tag;
            if (!tok.empty())
            {
                // we have attributes:
                // 1. save tag name
                tag = Tag(tok);
                // 2. loop over and add attributes
                while (!line.empty())
                {
                    tok = get_token(line, " = ");
                    std::string value(get_token(line, " "));
                    if (value.empty())
                    {
                        value = line;
                        line.clear();
                    }

                    tag.add_attribute(tok, value.substr(1, value.size() - 2));
                }
            }
            else
            {
                // No attributes, just tag name.
                tag = Tag(line);
            }

            if (p_open_tag)
            {
                set_tags(in, N, &tag);
                p_open_tag->add_subtag(tag);
            }
            else
            {
                set_tags(in, N, &tag);
                m_tags[tag.name()] = tag;
            }
        }
    }

    void open_tag(const std::string& name)
    {
        m_tags.emplace(name, name);
    }

private:
    std::string getline(std::istream& in) const
    {
        std::string line;
        std::getline(in, line);
        return line;
    }

    std::string query(std::string& q, const Tag::Tags* p_tags = nullptr) const
    {
        if (!p_tags) p_tags = &m_tags;
        const Tag::Tags& tags = *p_tags;

        Tag::Tags::const_iterator itag;

        std::string tok = get_token(q, ".");
        if (tok.empty())
        {
            // No dot, is it the tilde?
            tok = get_token(q, "~");
            itag = tags.find(tok);
            if (itag != tags.end())
            {
                const Tag::Attributes& attr = itag->second.attributes();
                Tag::Attributes::const_iterator iattr = attr.find(q);
                if (iattr != attr.end())
                {
                    return iattr->second;
                }
            }
        }
        else
        {
            // Go next level down with subtags.
            itag = tags.find(tok);
            if (itag != tags.end())
            {
                return query(q, &itag->second.subtags());
            }
        }

        return "Not Found!";
    }

public:
    HRML(std::istream& in, int N)
    {
        set_tags(in, N);
    }

    std::string run_query(std::istream& in) const
    {
        std::string line(getline(in));
        return query(line);
    }
};

//------------------------------------------------------------------------------
int main()
{
    int N, Q;
    std::cin >> N >> Q;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    HRML hrml(std::cin, N);

    // Process queries.
    while (Q--)
        OUT(hrml.run_query(std::cin));

    return 0;
}
