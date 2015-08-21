#ifndef _FBONEANIMATEDMESH_H_
#define _FBONEANIMATEDMESH_H_
#include "Object.h"
#include "Bone.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
using namespace irr;
using namespace core;
namespace FCE
{
class BONEANIMATEDMESH : public OBJECT
{
public:
    BONEANIMATEDMESH(scene::ISceneManager* manager, LOGGER* log);
    ~BONEANIMATEDMESH();
    void onInit();
    void onUpdate();
    void onRender();
    std::string getOType();

    int load(std::string filename);

    void setFrameLoop(u32 begin, u32 end);
    void setSpeed(f32 speed);
    u32 getCurrentFrame();
    std::string getFilename();
    scene::IAnimatedMesh* getMesh();
    scene::IAnimatedMeshSceneNode* getNode();

    int addAnimation(std::string key, std::string filename);
    void removeAnimation(std::string key);
    void setCurrentAnimation(std::string key);
    void attachToBone(OBJECT* o, std::string bone);
    core::vector3df getBonePosition(std::string bone);
    core::vector3df getBoneRotation(std::string bone);
    void setNode(scene::ISceneNode* node);
private:
    LOGGER* log;
    u32 flbegin, flend;
    f32 animSpeed;
    std::string filename;
    scene::IAnimatedMesh* mesh;
    scene::IAnimatedMeshSceneNode* anim;
    scene::ISceneManager* manager;
    std::string currentAnim;
    std::map<std::string, scene::IAnimatedMeshSceneNode*> animations;
    std::map<std::string, OBJECT*> connectedObjects;
    std::map<std::string, core::vector3df> lastRot;
};
}
#endif
