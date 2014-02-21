#include "b2mycontactfilter.h"
#include "Units/tankunit.h"
#include "bodyinfo.h"

#include "Effects/explode1.h"

#include <QDebug>
#include <typeinfo>

b2MyContactFilter::b2MyContactFilter(QMap<int, AbstractEffect *> * EffectBase)
{
    this->EffectBase = EffectBase;
}

bool b2MyContactFilter::ShouldCollide(b2Fixture * fixtureA, b2Fixture * fixtureB)
{
    BodyInfo * infoA = (BodyInfo*)fixtureA->GetBody()->GetUserData();
    BodyInfo * infoB = (BodyInfo*)fixtureB->GetBody()->GetUserData();

    //при столкновении 2 танков или столкновения с конструкцией ничего происходить не будет
    if((infoA->name  == "Tank" && infoB->name  == "Tank") || infoA->name == "Construction" || infoB->name == "Construction")
       return 1;

    if(infoA->name == "Bullet" && infoB->name == "Bullet")
       return 1;

    if(infoA->id != infoB->id) //если у танки и пули разные id (значит они враги)
    {
        if(infoA->name == "Bullet")
        {
            if(infoB->tankPointer)
               infoB->tankPointer->setDamage(infoA->bulletPointer->getDamageSize());
            infoA->bulletPointer->setFlyEnd();
            int id=0;
            while(1)
            {
                id = qrand()%50000;
                if(!EffectBase->contains(id))
                    break;
            }
            EffectBase->insert(id, new Explode1(NULL, infoA->bulletPointer->get_bul_x(), infoA->bulletPointer->get_bul_y(), 1, id));



            if(infoB->tankPointer->getHealth() < 1)
            {
                infoB->tankPointer->getBody()->SetTransform(b2Vec2((qrand()%500 + (0-qrand()%500)),(qrand()%500 + (0-qrand()%500))), qrand()%360);
                infoB->tankPointer->setDamage(-infoB->tankPointer->healthMax());
                infoA->bulletPointer->getTankUnit()->addScore();
                int id=0;
                while(1)
                {
                    id = qrand()%50000;
                    if(!EffectBase->contains(id))
                        break;
                }
                EffectBase->insert(id, new Explode1(NULL, infoB->tankPointer->getxPos(), infoB->tankPointer->getyPos(), 2.5, id));
            }

            return 1;
        }
        if(infoB->name == "Bullet")
        {
            if(infoA->tankPointer)
               infoA->tankPointer->setDamage(infoB->bulletPointer->getDamageSize());
            infoB->bulletPointer->setFlyEnd();
            int id=0;
            while(1)
            {
                id = qrand()%50000;
                if(!EffectBase->contains(id))
                    break;
            }
            EffectBase->insert(id, new Explode1(NULL, infoB->bulletPointer->get_bul_x(), infoB->bulletPointer->get_bul_y(), 1, id));

            if(infoA->tankPointer->getHealth() < 1)
            {
                int id=0;
                while(1)
                {
                    id = qrand()%50000;
                    if(!EffectBase->contains(id))
                        break;
                }
                EffectBase->insert(id, new Explode1(NULL, infoA->tankPointer->getxPos(), infoA->tankPointer->getyPos(), 2.5, id));
                infoA->tankPointer->getBody()->SetTransform(b2Vec2((qrand()%500 + (0-qrand()%500)),(qrand()%500 + (0-qrand()%500))), qrand()%360);
                infoA->tankPointer->setDamage(-infoA->tankPointer->healthMax());
                infoB->bulletPointer->getTankUnit()->addScore();
            }

            return 1;
        }
    }

   return 1;
}
