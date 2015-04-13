#ifndef _FTERRAIN_H_
#define _FTERRAIN_H_
#include "stdafx.h"
#include "Object.h"
#include "Misc/Logger.h"
#include "irrlicht.h"
using namespace irr;
namespace FCE
{
class HEIGHTMAP
{
public:
    HEIGHTMAP(u32 height, u32 width);
    ~HEIGHTMAP();
    f32 getData(u32 x, u32 y);
    void setHeight(u32 x, u32 y, f32 height);
    u32 getWidth()
    {
        return width;
    };
    u32 getHeight()
    {
        return height;
    };
    void load(std::string filename);
    core::vector3df getNormal(u32 x, u32 y, f32 scalingFactor);
    f32 scalingFactor;
private:
    bool fromFile;
    u32 height;
    u32 width;

    core::array<f32> data;
};

class TERRAIN : public OBJECT
{
public:
    TERRAIN(scene::ISceneManager* manager, LOGGER* log);
    ~TERRAIN();
    void onInit();
    void onUpdate();
    void onRender();
    void empty(u32 width, u32 height);
    void load(std::string filename);
    std::string getOType();
    HEIGHTMAP* getHeightmap()
    {
        return hMap;
    };
    std::string getFilename();
    bool fromFile;
    void setHeight(u32 x, u32 y, f32 height);

    scene::IAnimatedMesh* getMesh();
private:
    void addstrip(u32 y0, u32 y1, u32 bufNum);
    void makeMesh();
    void remakeMesh();
    std::string filename;
    scene::SMesh* mesh;
    LOGGER* log;
    HEIGHTMAP* hMap;
    scene::ISceneManager* manager;
};
}
#endif
