#ifndef _FRENDERER_H_
#define _FRENDERER_H_

#include "irrlicht.h"
#include "Misc/PostProcessing/PostProcessManager.h"

namespace FCE
{
class Renderer
{
public:
    Renderer();
    ~Renderer();
    void init();
    void update();
    void render();

    u32 addShader(std::string filename);
    void removeShader(u32 id);
private:


};
}
#endif
