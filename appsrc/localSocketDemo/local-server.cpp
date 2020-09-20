#include <iostream>
#include <cutils/sockets.h>
#include <unistd.h>
#include <cstring>
#include <cpputils-base/logging.h>

#define MAX_BUF 100

using namespace cpputils::base;
char buffer[MAX_BUF];
int main(int argc, char **argv)
{
    int skt = socket_local_server("JP", 0, SOCK_STREAM);

    LOG(INFO) << "Wait for client:-" << std::endl;

    int cfd = accept(skt, NULL, NULL);

    if (cfd > 0)
    {
        LOG(INFO) << "Client connected with fd : " << cfd << std::endl;
        while (1)
        {
            int len = read(cfd, buffer, MAX_BUF - 1);
            if (len > 0)
            {
                if (buffer[0] == '~')
                    break;
                buffer[len] = 0;
                LOG(INFO) << "Received Msg: "
                          << buffer;
                len = sprintf(buffer, "Hello Client Message Recieved\n");
                write(cfd, buffer, len);
            }
        }
        close(cfd);
        close(skt);
    }
    else
    {
        LOG(INFO) << "Client connection failed!";
    }
    LOG(INFO) << "Exiting";
    return 0;
}
