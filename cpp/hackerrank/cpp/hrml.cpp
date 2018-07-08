#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <map>

//------------------------------------------------------------------------------
#define OUT(S) std::cout << S << std::endl

#define DEBUG
#ifdef DEBUG
#define DOUT(S) std::cout << "DEBUG: " << S << std::endl
#else
#define DOUT(S)
#endif

//------------------------------------------------------------------------------
std::string get_token(std::string& str, const std::string& delim)
{
    std::string token;
    size_t dpos;
    if ((dpos = str.find(delim)) != std::string::npos)
    {
        token = str.substr(0, dpos);
        str.erase(0, dpos + delim.size());

//        DOUT("token='" << token << "'; remain='" << str << "'");
    }

    return token;
}

std::string get_token(std::stringstream& ss, char delim);

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

    void add_attribute(const std::string& name, const std::string& value)
    {
        m_attributes[name] = value;
    }

    void add_subtag(Tag tag)
    {
        m_subtags[tag.name()] = tag;
    }

    std::string to_str(std::string prefix = "") const
    {
        std::ostringstream ostr(prefix);
        ostr << "<" << m_name;
        for (auto& a : m_attributes)
        {
            ostr << " " << a.first << " = \"" << a.second << "\"";
        }
        ostr << ">";
        for (auto& t : m_subtags)
        {
            ostr << "\n" << t.second.to_str(prefix + "  ");
        }
        ostr << "\n</" << m_name << ">";
        return ostr.str();
    }

    friend std::ostream& operator<<(std::iostream&, const Tag&);
};

std::ostream& operator<<(std::ostream& os, const Tag& tag)
{
    os << tag.to_str();
    return os;
}

std::ostream& operator<<(std::ostream& os, const Tag::Tags& tags)
{
    for (auto& t : tags)
    {
        os << t.second << "\n";
    }
    return os;
}

//------------------------------------------------------------------------------
class HRML
{
    Tag::Tags m_tags;

private:
    void set_tags(std::istream& in, int& N, Tag* p_open_tag = nullptr)
    {
        DOUT("set_tags with open_tag=" << (p_open_tag ? p_open_tag->name() : "<empty>"));

        while (N-- > 0)
        {
            std::string line(getline(in));
            // DOUT("input: " << line);

            if (line.substr(0, 2) == "</")
            {
                DOUT("closing: " << line);
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

                    DOUT("attribute {" << tok << "=" << value << "}");
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
                DOUT("Tag(" << tag.name() << ") is subtag of Tag(" << p_open_tag->name() << ")");
                set_tags(in, N, &tag);
                p_open_tag->add_subtag(tag);
            }
            else
            {
                DOUT("top level Tag(" << tag.name() << ")");
                set_tags(in, N, &tag);
                m_tags[tag.name()] = tag;
            }
        }
    }

    void open_tag(const std::string& name)
    {
        m_tags.emplace(name, name);
    }

protected:
    std::string getline(std::istream& in) const
    {
        std::string line;
        std::getline(in, line);
        return line;
    }

    std::string query(std::string& q, Tag* p_open_tag = nullptr) const
    {
        std::string result("Not found!");

        std::string tok = get_token(q, ".");
        if (tok.empty())
        {
            // No dot, is it the tilde?
            tok = get_token(q, "~");
            const Tag& t = m_tags[tok];
        }
        else
        {

        }

        return result;
    }

public:
    HRML(std::istream& in, int N)
    {
        set_tags(in, N);
        OUT(m_tags);
    }

    std::string run_query(std::istream& in) const
    {
        std::string line(getline(in));

        // Actual query is recursive on the tag tokens?

        return line;
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
        DOUT("query {" << hrml.run_query(std::cin) << "}");

    return 0;
}
