#include "session.h"

Session::Session(boost::asio::io_service& ioService, Server * serverPtr)
    : currentSocket(ioService), server(serverPtr)
{
}

Session::~Session()
{
    delete nickName;
}

boost::asio::ip::tcp::socket & Session::getSocket()
{
    return currentSocket;
}

void Session::postReceive()
{
    memset( &dataBuffer, '\0', sizeof(dataBuffer) );

    currentSocket.async_read_some
            (
                boost::asio::buffer(dataBuffer, Config::MAX_BUFFER_SIZE),
                boost::bind( &Session::handleReceive, this,
                             boost::asio::placeholders::error,
                             boost::asio::placeholders::bytes_transferred )
                );
}

void Session::postWrite(std::string & writeMsg)
{
    //char szMessage[Config::MAX_BUFFER_SIZE] = {0,};
    //sprintf(szMessage, "%s", str);
    //std::string writeMsg = szMessage;

    boost::asio::async_write(currentSocket, boost::asio::buffer(writeMsg),
                             boost::bind( &Session::handleWrite, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred )
                             );

    postReceive();
}

void Session::createUnit(int ownerId)
{
    std::cout << "Delete player:" << ownerId << std::endl;
    server->getWorld()->addUnit(ownerId);
}

void Session::setNickName(std::string * name)
{
    nickName = name;
    std::cout << "Socket:" << currentSocket.native() << " Set nickname:" << nickName->c_str()  << std::endl;
}

std::string * Session::getNickName()
{
    return nickName;
}

void Session::parseQuery(const std::string & query)
{
    std::istringstream ss(query);
    std::string token;

    while (std::getline(ss, token, ':')) {
        if (token == "rtg") //Rotate gun
        {
            std::getline(ss, token, ':');
            int i = std::stoi(token);
            getCurrentUnit()->setGunAnge(i);
            std::cout << "Unit id:" << getCurrentUnitId() << " Rotate gun:" << i << std::endl;
        }
        else if (token == "mvf") //Move forvard
        {
            getCurrentUnit()->moveForward();
            std::cout << "Unit id:" << getCurrentUnitId() << " Move forward" << std::endl;
        }
        else if (token == "mvb") //Move back
        {
            getCurrentUnit()->moveBack();
            std::cout << "Unit id:" << getCurrentUnitId() << " Move back" << std::endl;
        }
        else if (token == "rbl") //Rotate body left
        {
            getCurrentUnit()->setBodyAnge(getCurrentUnit()->getBodyAnge() - 5);
            std::cout << "Unit id:" << getCurrentUnitId() << " Rotate left" << std::endl;
        }
        else if (token == "rbr") //Rotate body right
        {
            getCurrentUnit()->setBodyAnge(getCurrentUnit()->getBodyAnge() + 5);
            std::cout << "Unit id:" << getCurrentUnitId() << " Rotate right" << std::endl;
        }
        else if (token == "sht") //Shot
        {
            std::cout << "Unit id:" << getCurrentUnitId() << " Unit shot" << std::endl;
        }
        else if (token == "cnt") //Create player set name and send id to client
        {
            std::getline(ss, token, ':');
            setNickName(new std::string(token));

            id = currentSocket.native();
            char szMessage[Config::MAX_BUFFER_SIZE] = {0,};
            sprintf(szMessage, ":sysid:%d:", id);
            std::string data = szMessage;
            postWrite(data);
        }
    }
}

void Session::setCurrentUserUnitId(int id)
{
    currentUserUnitId = id;
}

int Session::getCurrentUnitId()
{
    return currentUserUnitId;
}

AbstractUnit * Session::getCurrentUnit()
{
    return server->getWorld()->getUnitById(currentUserUnitId);
}

void Session::handleWrite(const boost::system::error_code & error, size_t /*bytes_transferred*/)
{
    if( error )
    {
        if( error == boost::asio::error::eof )
        {
            std::cout << "error == eof" << std::endl;
            server->deletePlayer(id);
        }
        else
        {
            std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
            server->deletePlayer(id);
        }
    }
}

void Session::handleReceive( const boost::system::error_code& error, size_t bytes_transferred )
{
    if( error )
    {
        if( error == boost::asio::error::eof )
        {
            std::cout << "error == eof" << std::endl;
            server->deletePlayer(id);
        }
        else
        {
            std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
            server->deletePlayer(id);
        }
    }
    else
    {
        const std::string strRecvMessage(dataBuffer);
        std::cout << "Socket:" << currentSocket.native() << " Client send: " << strRecvMessage << " Size: " << bytes_transferred << std::endl;



        parseQuery(strRecvMessage);



        /*char szMessage[Config::MAX_BUFFER_SIZE] = {0,};
        sprintf( szMessage, "Re:%s", strRecvMessage.c_str() );
        writeMessage = szMessage;

        boost::asio::async_write(currentSocket, boost::asio::buffer(writeMessage),
                                 boost::bind( &Session::handleWrite, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred )
                                 );*/

        postReceive();
    }
}
