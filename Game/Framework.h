#ifndef _FFRAMEWORK_H_
#define _FFRAMEWORK_H_
#include "stdafx.h"
#include "Misc/Logger.h"
#include "Scene.h"
#include "Objects/Mesh.h"
#include "Misc/Script.h"
#include "EventReceiver.h"
#include "Irrlicht.h"

#define SCREENWIDTH 1024
#define SCREENHEIGHT 800

#define BUILD "2.1.1"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PLAT "Win32"
#elif defined(__APPLE__)
#define PLAT "Apple"
#elif defined(__CYGWIN__)
#define PLAT "CYGWIN"
#else
#define PLAT "Other (Probably Linux)"
#endif
//TODO:create compressed filesystem

//TODO: Comment codebase
using namespace irr;
namespace FCE
{
class FRAMEWORK
{
public:
    FRAMEWORK();
    ~FRAMEWORK();
    int init();
    int update();
    int render();
    bool run;

private:
    //Custom classes
    LOGGER* log;
    SCENE* scene;
    FEventReceiver receiver;

    //Irrlicht classes
    IrrlichtDevice *device;
    video::IVideoDriver* driver;
    gui::IGUIEnvironment* gui;
    scene::ISceneManager* manager;
    io::IXMLReader* settings;

};
}
#endif //_FFRAMEWORK_H_
