#pragma once

#include <string>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>

constexpr const long default_timeout_seconds{2};

//////////////////////////////////////////////////////////////////////////////////
//! AddInfo: wraps usage for getaddrinfo() and the addrinfo structure.
//!
//! Use case: Given a host address and port, resolve the host address. Provide
//! a CheckRemote() method which returns true if the remote appears to be
//! accepting connections on the given port.
//!
//! Only currently coded for TCP socket streams.
//!
//! See man getaddrinfo() for more information.
//////////////////////////////////////////////////////////////////////////////////
class AddrInfo
{
public:
    //! Try to resolve host on construction.
    AddrInfo(const std::string & host, int port)
    {
        int error = getaddrinfo(host.c_str(), std::to_string(port).c_str(), AddrHint().GetHint(), &m_addr_ptr);
        if (error)
        {
            m_err_msg = gai_strerror(error);
            freepointer(); // just in case
        }
    };

    ~AddrInfo()
    {
        freepointer();
    };

    //! Checks remote host on given port. Returns false and sets error message on fail.
    bool CheckRemote();

    bool HasError() const { return not m_err_msg.empty(); }
    const std::string& GetErrorMsg() const { return m_err_msg; }

    //! Direct read access to addrinfo pointer.
    const addrinfo* GetPtr() const { return m_addr_ptr; }

protected:
    struct AddrHint
    {
        AddrHint()
        {
            memset(&m_hint, 0, sizeof m_hint);
            m_hint.ai_family = AF_INET;
            m_hint.ai_socktype = SOCK_STREAM;
            m_hint.ai_protocol = IPPROTO_TCP;
            m_hint.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;
        }

        const addrinfo* GetHint() const { return &m_hint; }

    private:
        addrinfo m_hint;
    };

private:
    addrinfo* m_addr_ptr{nullptr};

    std::string m_err_msg;

    void freepointer()
    {
        if (m_addr_ptr != nullptr)
        {
            try
            {
                freeaddrinfo(m_addr_ptr);
            }
            catch (...)
            {
                // No need to do anything here.
            }
            m_addr_ptr = nullptr;
        }
    }
};

bool AddrInfo::CheckRemote()
{
    if (HasError())
    {
        return false;
    }

    std::string_view cause;
    for (addrinfo* addr_ptr = m_addr_ptr; addr_ptr; addr_ptr = m_addr_ptr->ai_next)
    {
        int sock = socket(addr_ptr->ai_family, addr_ptr->ai_socktype, addr_ptr->ai_protocol);
        if (sock < 0)
        {
            cause = "failed to get socket";
            continue;
        }
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sock, &set);

        if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
        {
            cause = "unable to set socket to non-blocking";
            continue;
        }
        
        if (connect(sock, addr_ptr->ai_addr, addr_ptr->ai_addrlen) < 0)
        {
            timeval timeout{default_timeout_seconds, 0};
            if (errno != EINPROGRESS or select(sock+1, nullptr, &set, nullptr, &timeout) <= 0)
            {
                cause = "connect failed";
                close(sock);
                continue;
            }
        }

        // Got a connection.
        shutdown(sock, SHUT_RDWR);
        close(sock);
        return true;
    }
    m_err_msg = cause;
    return false;
}
