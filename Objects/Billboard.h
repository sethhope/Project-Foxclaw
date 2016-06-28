#ifndef FBILLBOARD_H
#define FBILLBOARD_H

#include "stdafx.h"
#include "Object.h"
#include "Misc/Logger.h"
namespace FCE
{
class BILLBOARD : public OBJECT
{
public:
    BILLBOARD(scene::ISceneManager* manager, LOGGER* log);
    ~BILLBOARD();

    void onInit();
    void onUpdate();
    void onRender();
    void setTexture(IrrlichtDevice* device, std::string file);
    std::string getOType(){return "BILLBOARD";};
    void setSize(f32 x, f32 y);
private:
    bool setup;
    std::string filename;
    scene::ISceneManager* manager;
    LOGGER* log;
};
}
#endif
