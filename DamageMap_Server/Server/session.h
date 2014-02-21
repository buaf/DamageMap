ifndef SESSION_H
#define SESSION_H

#include "Server/server.h"
class Server;

#include "Config/config.h"

class Session
{
public:
    Session(boost::asio::io_service & ioService, Server * serverPtr);
    ~Session();
    boost::asio::ip::tcp::socket & getSocket();

    void postReceive();
    void postWrite(std::string & writeMsg);

    void createUnit(int ownerId);
    void setNickName(std::string * name);
    std::string * getNickName();

    void parseQuery(const std::string &query);
    void setCurrentUserUnitId(int id);

    int getCurrentUnitId();

    AbstractUnit * getCurrentUnit();

private:
    void handleWrite(const boost::system::error_code & error, size_t bytes_transferred);
    void handleReceive( const boost::system::error_code & error, size_t bytes_transferred);

    boost::asio::ip::tcp::socket currentSocket;
    std::string writeMessage;
    char dataBuffer[Config::MAX_BUFFER_SIZE];
    Server * server;

    std::string * nickName;
    int id;
    int currentUserUnitId;
};


#endif // SESSION_H
