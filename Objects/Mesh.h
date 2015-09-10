#ifndef _FMESH_H_
#define _FMESH_H_
#include "Object.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
using namespace irr;
using namespace core;
namespace FCE
{
class MESH : public OBJECT
{
public:
    MESH(scene::ISceneManager* manager, LOGGER* log);
    ~MESH();
    void onInit();
    void onUpdate();
    void onRender();
    std::string getOType();

    u8 load(std::string filename);
    u8 create(scene::IMesh* mesh);
    std::string getFilename();
    scene::IMesh* getMesh();
    //scene::IAnimatedMeshSceneNode* getNode();

private:
    u32 id;
    LOGGER* log;
    std::string filename;
    scene::IMesh* mesh;
    scene::ISceneManager* manager;
};
}
#endif
