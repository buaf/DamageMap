#include "Server/server.h"
#include "Config/config.h"

int main(int argc, char *argv[])
{
    boost::asio::io_service ioService;
    Server server(ioService, Config::PORT_NUMBER);
    ioService.run();
}
