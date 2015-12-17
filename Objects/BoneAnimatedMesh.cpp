#include "BoneAnimatedMesh.h"
using namespace FCE;

BONEANIMATEDMESH::BONEANIMATEDMESH(scene::ISceneManager* manager,  LOGGER* log)
{
    this->manager = manager;
    this->log = log;
    id = -1;
    name = "BONEANIMATEDMESH";
    flbegin = 1;
    flend = 1;
    position = core::vector3df(0, 0, 0);
    rotation = core::vector3df(0, 0, 0);
    scale = core::vector3df(1, 1, 1);
}

BONEANIMATEDMESH::~BONEANIMATEDMESH()
{
    anim->remove();
}

int BONEANIMATEDMESH::load(std::string filename)
{
    log->logData("Loading Bone Animated mesh", filename);
    this->filename = filename;
    mesh = manager->getMesh(filename.c_str());
    anim = manager->addAnimatedMeshSceneNode(mesh);
    anim->setJointMode(scene::EJUOR_CONTROL);
    if(anim)
    {
        log->debugData(MAJOR, "Loaded Bone Animated mesh");
        thisNode = anim;
        return 0;
    }
    log->logData("Failed to load mesh", filename);
    return 1;
}

void BONEANIMATEDMESH::onInit()
{
}

void BONEANIMATEDMESH::onUpdate()
{
}

void BONEANIMATEDMESH::onRender()
{
    if(animations.size() > 0)
    {
        for(int i=0; i < ((scene::IAnimatedMeshSceneNode*)thisNode)->getJointCount();i++)
        {
            const c8* name = ((scene::IAnimatedMeshSceneNode*)thisNode)->getJointNode(i)->getBoneName();
            if(animations[currentAnim]->getJointNode(name) != NULL)
            {
                //((scene::IAnimatedMeshSceneNode*)thisNode)->getJointNode(i)->setPosition(animations[currentAnim]->getJointNode(name)->getPosition());
                ((scene::IAnimatedMeshSceneNode*)thisNode)->getJointNode(i)->setRotation(animations[currentAnim]->getJointNode(name)->getRotation());
                if(connectedObjects[name])
                {
                    connectedObjects[name]->setRotation(animations[currentAnim]->getJointNode(name)->getRotation());
                }
            }else
            {
                log->debugData(EXTRA, "Failed to find bone", name);
            }
        }
    }
}

scene::IAnimatedMesh* BONEANIMATEDMESH::getMesh()
{
    return mesh;
}

scene::IAnimatedMeshSceneNode* BONEANIMATEDMESH::getNode()
{
    return anim;
}

void BONEANIMATEDMESH::setFrameLoop(u32 begin, u32 end)
{
    animations[currentAnim]->setFrameLoop(begin, end);
    flbegin = begin;
    flend = end;
}

void BONEANIMATEDMESH::setSpeed(f32 speed)
{
    animations[currentAnim]->setAnimationSpeed(speed);
    animSpeed = speed;
}

std::string BONEANIMATEDMESH::getOType()
{
    return "BONEANIMATEDMESH";
}

std::string BONEANIMATEDMESH::getFilename()
{
    return filename;
}

u32 BONEANIMATEDMESH::getCurrentFrame()
{
    return animations[currentAnim]->getFrameNr();
}

int BONEANIMATEDMESH::addAnimation(std::string key, std::string filename)
{

    log->logData("Loading Bone Animation", filename);
    scene::IAnimatedMesh* m = manager->getMesh(filename.c_str());
    scene::IAnimatedMeshSceneNode* tmp = manager->addAnimatedMeshSceneNode(m);
    if(anim)
    {
        log->debugData(MAJOR, "Loaded Bone Animation");
        //tmp->setParent(thisNode);
        tmp->setFrameLoop(flbegin, flend);
        animations[key]=tmp;
        return 0;
    }
    log->logData("Failed to load animation", filename);
    return 1;
}

void BONEANIMATEDMESH::setCurrentAnimation(std::string key)
{
    currentAnim = key;
    animations[currentAnim]->setFrameLoop(flbegin, flend);
    animations[currentAnim]->setAnimationSpeed(animSpeed);

}

void BONEANIMATEDMESH::attachToBone(OBJECT* o, std::string bone)
{
    scene::IBoneSceneNode* tmpNode = ((scene::IAnimatedMeshSceneNode*)thisNode)->getJointNode(bone.c_str());
    if(tmpNode != NULL)
    {
        o->attachTo(this);
        connectedObjects[bone] = o;
        lastRot[bone] = tmpNode->getRotation();
        log->debugData(MAJOR, "Connected object to", bone);
    }else
    {
        log->debugData(MAJOR, "Bone not found", bone);
    }

}
core::vector3df BONEANIMATEDMESH::getBonePosition(std::string bone)
{
    return ((scene::IAnimatedMeshSceneNode*)thisNode)->getJointNode(bone.c_str())->getPosition();
}
core::vector3df BONEANIMATEDMESH::getBoneRotation(std::string bone)
{
    return ((scene::IAnimatedMeshSceneNode*)thisNode)->getJointNode(bone.c_str())->getRotation();
}
void  BONEANIMATEDMESH::setNode(scene::ISceneNode* node)
{
    thisNode = node;
}
