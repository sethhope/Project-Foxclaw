#ifndef CAPSULESHAPE_H_INCLUDED
#define CAPSULESHAPE_H_INCLUDED

//Added by Seth Hope to the IrrBullet physics library
//Based on the sphereshape.cpp file

#include "collisionshape.h"
#include "Bullet/BulletCollision/CollisionShapes/btCapsuleShape.h"


class ICapsuleShape : public ICollisionShape
{
public:
    ICapsuleShape(irr::scene::ISceneNode *n, irr::f32 m, bool overrideMargin = false);

    virtual ~ICapsuleShape();


protected:
    void createShape(bool overrideMargin);
};

#endif // CAPSULESHAPE_H_INCLUDED
