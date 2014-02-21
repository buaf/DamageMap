#include "world.h"

#include "World/Unit/standarttank.h"

World::World(Server * serverPtr)
{
    gravity = new b2Vec2(0.0f, 0.0f);
    world = new b2World(*gravity, 0);
    unitList = new QHash<int, AbstractUnit *>();
    server = serverPtr;
}

World::~World()
{
    delete gravity;
    delete world;
}

void World::tick()
{
    boost::xtime m_period;
    m_period.sec = 0;
    m_period.nsec = 1000;

    while (true)
    {
        world->Step(1.0f / 60.0f, 20.0f, 20.0f);
        boost::thread::sleep(m_period);
    }
}

void World::addUnit(int ownerId)
{
    for(int i = 0; i < 100; i++)
    {
        int unitId = rand();
        if (!unitList->contains(unitId))
        {
            unitList->insert(unitId, new StandartTank(world, 20, 20, ownerId));
            //server->getSessionById(ownerId)->setCurrentUserUnitId(unitId);
            std::cout << "Create unit id:" << unitId << " Owner id:" << ownerId << std::endl;
            return;
        }
    }
}

void World::deleteUnit(int unitId)
{
    std::cout << "Delete unit:" << unitId << std::endl;
    unitList->remove(unitId);
}

AbstractUnit * World::getUnitById(int unitId)
{
    return unitList->value(unitId);
}
