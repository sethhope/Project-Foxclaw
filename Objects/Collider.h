#ifndef _COLLIDER_H_
#define _COLLIDER_H_
#include "stdafx.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
#include "irrBullet.h"
#define COL_CUBE 0
#define COL_SPHERE 1
#define COL_MESH_GIMPACT 2
#define COL_MESH_CONVEXHULL 3
#define COL_MESH_TRIMESH 4
#define COL_LIQUID 5
#define COL_SOFTBODY 6
#define COL_CAPSULE 7
#define COL_MESH_BHV 8
#define MAX_TYPE 8


//TODO: Apply force and torque integration
using namespace irr;
namespace FCE
{

class COLLIDER
{
public:
    COLLIDER(scene::ISceneNode* parent, scene::ISceneManager* manager, irrBulletWorld* world, int type, float mass, LOGGER* log);
    ~COLLIDER();
    void init();
    void setLocal(bool local);
    void setType(int type);
    void setMass(float mass);
    void setVelocity(core::vector3df velocity);
    void setAVelocity(core::vector3df velocity);
    void addCentralForce(core::vector3df force);
    void addForce(core::vector3df force, core::vector3df position);
    void addTorque(core::vector3df torque);
    void setFriction(float friction);
    void setDamping(float lDamping, float aDamping);
    void setMesh(scene::IMesh* colMesh);
    int getType();
    float getMass();
    IRigidBody* body;
    irrBulletWorld *world;
    scene::ISceneNode* node;
private:
    int type;
    float friction, mass, damping;
    float rad;
    bool local;
    scene::IMesh* colMesh;

    ILiquidBody* lbody;
    ISoftBody* sbody;
    LOGGER* log;
    scene::ISceneManager* manager;
};
}
#endif
