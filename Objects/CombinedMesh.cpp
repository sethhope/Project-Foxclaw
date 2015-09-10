#include "CombinedMesh.h"
using namespace FCE;
COMBINEDMESH::COMBINEDMESH(scene::ISceneManager* manager, u32 bufferSize, LOGGER* log)
{
    combiner = new CMeshCombiner(0.8, bufferSize, ETPT_EXPAND);
    this->log = log;
    this->manager = manager;
}
COMBINEDMESH::~COMBINEDMESH()
{

}

void COMBINEDMESH::addMesh(MESH* mesh)
{
    log->debugData(MAJOR, "Adding mesh", mesh->getName());
    nodes.push_back((scene::IMeshSceneNode*)mesh->getIrrNode());
    mesh->uDa = false;
    meshIDs.push_back(mesh->getID());
}

scene::IMesh* COMBINEDMESH::getMesh()
{
    log->debugData(MAJOR, "Combining nodes");
    scene::IMesh* mesh = combiner->combineMeshes(manager->getVideoDriver(), nodes);
    return(mesh);
}
