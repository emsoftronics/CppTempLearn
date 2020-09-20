#include <iostream>
#include <cutils/sockets.h>
#include <unistd.h>
#include <cstring>
#include <cpputils-base/logging.h>

#define MAX_BUF 100
char buffer[MAX_BUF];
using namespace cpputils::base;
int main(int argc, char **argv)
{
    LOG(INFO) << "client is tring to connect.";
    int cskt = socket_local_client("JP", 0, SOCK_STREAM);
    if (cskt > 0)
    {
        LOG(INFO) << "client is tring send message.";
        int len = 0; //sprintf(buffer, "Hello Server it's me!\n");
        do
        {
            std::cin >> buffer;
            write(cskt, buffer, strlen(buffer));
            len = read(cskt, buffer, MAX_BUF - 1);
            if (len > 0)
            {
                buffer[len] = 0;
                LOG(INFO) << "Received Msg: \n"
                          << buffer; // << std::endl;
            }
        } while (buffer[0] != '~');
    }
    close(cskt);
    LOG(INFO) << "Exiting"; // << std::endl;
    return 0;
}
