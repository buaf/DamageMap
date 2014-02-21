#ifndef WORLD_H
#define WORLD_H

#include <Box2D/Box2D.h>

#include "boost/thread.hpp"

#include <QHash>

#include "World/Unit/abstractunit.h"
class AbstractUnit;

#include "Server/server.h"
class Server;

class World
{
public:
    World(Server * serverPtr);
    ~World();

    void tick();

    void addUnit(int ownerId);
    void deleteUnit(int unitId);
    AbstractUnit * getUnitById(int unitId);

private:
    b2Vec2 * gravity;
    b2World * world;

    Server * server;

    QHash<int, AbstractUnit *> * unitList;
};

#endif // WORLD_H
