#include <iostream>
#include <fstream>
#include <limits>
#include <stack>

#define OUT(S) std::cout << S << std::endl

using Stack = std::stack<char>;

int main(int, char** argv)
{
    std::ifstream ins(argv[1]);

    int n;
    ins >> n;
    ins.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    while (n--)
    {
        Stack bs;
        std::string s;
        std::getline(ins, s);
        std::string::const_iterator si = s.begin(), end = s.end();
        
        if (s.size() % 2) goto nope;
        
        while (si != end)
        {
            char x = *si++;
            if (x == '(' || x == '[' || x == '{')
            {
                bs.push(x);
                continue;
            }
            
            if (!bs.size()) goto nope;
            char l = bs.top();
            switch(x)
            {
                case ')':
                    if (l != '(') goto nope;
                    break;
                case ']':
                    if (l != '[') goto nope;
                    break;
                case '}':
                    if (l != '{') goto nope;
                    break;
            }
            bs.pop();
        }
        if (bs.size()) goto nope;

        OUT("YES");
        continue;
        
        nope:
        OUT("NO");
    }

    return 0;
}
