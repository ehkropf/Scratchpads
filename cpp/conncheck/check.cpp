#include "CheckRemote.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "ERR: provide a host and port\n";
        return -1;
    }

    std::string host{argv[1]};
    int port;
    try
    {
        port = std::stoi(argv[2]);
    }
    catch (const std::exception& e)
    {
        std::cout << "ERR: " << e.what() << "\n";
        return -1;
    }

    AddrInfo addrinfo{host, port};
    if (addrinfo.HasError())
    {
        std::cout << "ERR: " << addrinfo.GetErrorMsg() << "\n";
        return -1;
    }

    if (not addrinfo.CheckRemote())
    {
        std::cout << "ERR: host " << host << ":" << port << " does not appear to be ok (" << addrinfo.GetErrorMsg() << ")\n";
        return -1;
    }

    std::cout << "OK: host " << host << ":" << port << " appears to be taking connections.\n";
    return 0;
}
