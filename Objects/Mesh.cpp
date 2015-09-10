#include "Mesh.h"
using namespace FCE;

MESH::MESH(scene::ISceneManager* manager,  LOGGER* log)
{
    this->manager = manager;
    this->log = log;
    id = -1;
    name = "MESH";
    position = core::vector3df(0, 0, 0);
    rotation = core::vector3df(0, 0, 0);
    scale = core::vector3df(1, 1, 1);
}

MESH::~MESH()
{
}

u8 MESH::load(std::string filename)
{
    log->logData("Loading mesh", filename);
    this->filename = filename;
    mesh = manager->getMesh(filename.c_str());
    scene::IMeshSceneNode* node = manager->addOctreeSceneNode(mesh, 0, -1, 64);
    if(node)
    {
        log->debugData(MAJOR, "Loaded mesh");
        thisNode = node;
        thisNode->setAutomaticCulling(scene::EAC_FRUSTUM_SPHERE);
        if(!thisNode)
        {
            log->debugData(MAJOR, "Failed to load thisNode");
            return 1;
        }
        return 0;
    }
    log->logData("Failed to load mesh", filename);
    return 1;
}

u8 MESH::create(scene::IMesh* mesh)
{
    log->logData("Creating mesh");
    this->mesh = mesh;
    scene::IMeshSceneNode* node = manager->addMeshSceneNode(mesh);
    if(node)
    {
        log->debugData(MAJOR, "Loaded mesh");
        thisNode = node;
        thisNode->setAutomaticCulling(scene::EAC_FRUSTUM_SPHERE);
        if(!thisNode)
        {
            log->debugData(MAJOR, "Failed to load thisNode");
            return 1;
        }
        return 0;
    }
    log->logData("Failed to create mesh");
    return 1;
}

void MESH::onInit()
{
}

void MESH::onUpdate()
{
}
void MESH::onRender()
{
}

scene::IMesh* MESH::getMesh()
{
    return mesh;
}

std::string MESH::getOType()
{
    return "MESH";
}

std::string MESH::getFilename()
{
    return filename;
}
