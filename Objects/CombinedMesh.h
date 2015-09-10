#ifndef COMBINEDMESH_H_INCLUDED
#define COMBINEDMESH_H_INCLUDED
#include "stdafx.h"
#include "Object.h"
#include "Misc/Logger.h"
#include "Mesh.h"
#include "Misc/CMeshCombiner.h"
namespace FCE
{
    class COMBINEDMESH
    {
    public:
        COMBINEDMESH(scene::ISceneManager* manager, u32 bufferSize, LOGGER* log);
        ~COMBINEDMESH();
        void onInit();
        void onUpdate();
        void onRender();
        void addMesh(MESH* mesh);
        scene::IMesh* getMesh();
        std::vector<u32> meshIDs;
    private:
        CMeshCombiner* combiner;
        core::array<scene::IMeshSceneNode*> nodes;

        scene::ISceneManager* manager;
        LOGGER* log;
    };
}

#endif // COMBINEDMESH_H_INCLUDED
