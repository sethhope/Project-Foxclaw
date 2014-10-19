#ifndef _FFRAMEWORK_H_
#define _FFRAMEWORK_H_
#include "stdafx.h"
#include "Misc/Logger.h"
#include "Scene.h"
#include "Objects/Mesh.h"
#include "Misc/Script.h"
#include "Irrlicht.h"
#define SCREENWIDTH 1024
#define SCREENHEIGHT 800
#define BUILD "2.1.0.1"
#define PLAT "Win32"
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
			LOGGER* log;
 			IrrlichtDevice *device;
			video::IVideoDriver* driver;
			gui::IGUIEnvironment* gui;
			scene::ISceneManager* manager;
			SCENE* scene;
	};
}
#endif //_FRAMEWORK_H_
