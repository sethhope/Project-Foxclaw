#include "SOFTMESH.h"
using namespace FCE;

SOFTMESH::SOFTMESH(scene::ISceneManager* manager, irrBulletWorld* world, LOGGER* log)
{
    this->manager = manager;
    this->log = log;
    this->world = world;
    id = -1;
    name = "SOFTMESH";
    position = core::vector3df(0, 0, 0);
    rotation = core::vector3df(0, 0, 0);
    scale = core::vector3df(1, 1, 1);
}

SOFTMESH::~SOFTMESH()
{
}

u8 SOFTMESH::load(std::string filename, f32 mass)
{
    log->logData("Loading Softbody Mesh", filename);
    this->filename = filename;
    mesh = manager->getMesh(filename.c_str());
    scene::IMeshSceneNode* node = manager->addMeshSceneNode(mesh);
    if(node)
    {
        log->debugData(MAJOR, "Loaded mesh");
        node->setAutomaticCulling(scene::EAC_OFF);
        thisNode = node;
        this->node = node;
        if(!thisNode)
        {
            log->debugData(MAJOR, "Failed to load thisNode");
            return 1;
        }
        sbody = world->addSoftBody((scene::IMeshSceneNode*)thisNode);
        if(!sbody)
        {
            log->debugData(MAJOR, "Failed to create softbody");
            return 1;
        }
        sbody->getConfiguration().poseMatchingCoefficient = 0.0;
        sbody->updateConfiguration();

        sbody->generateBendingConstraints(2);
        sbody->randomizeConstraints();
        return 0;
    }
    log->logData("Failed to load Softbody mesh", filename);
    return 1;
}

void SOFTMESH::onInit()
{
}

void SOFTMESH::onUpdate()
{
}
void SOFTMESH::onRender()
{
}

scene::IMesh* SOFTMESH::getMesh()
{
    return mesh;
}

std::string SOFTMESH::getOType()
{
    return "SOFTMESH";
}

std::string SOFTMESH::getFilename()
{
    return filename;
}

ISoftBody* SOFTMESH::getSoftBody()
{
    return sbody;
}
