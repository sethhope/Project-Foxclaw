#ifndef _FSOFTMESH_H_
#define _FSOFTMESH_H_
#include "Object.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
#include "irrBullet.h"
using namespace irr;
using namespace core;
namespace FCE
{
class SOFTMESH : public OBJECT
{
public:
    SOFTMESH(scene::ISceneManager* manager, irrBulletWorld* world, LOGGER* log);
    ~SOFTMESH();
    void onInit();
    void onUpdate();
    void onRender();
    std::string getOType();

    u8 load(std::string filename, f32 mass);
    std::string getFilename();
    scene::IMesh* getMesh();
    ISoftBody* getSoftBody();
    //scene::IAnimatedMeshSceneNode* getNode();

private:
    u32 id;
    LOGGER* log;
    std::string filename;
    scene::IMesh* mesh;
    scene::IMeshSceneNode* node;
    scene::ISceneManager* manager;
    irrBulletWorld* world;
    ISoftBody* sbody;
};
}
#endif
