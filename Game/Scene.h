#ifndef _FSCENE_H_
#define _FSCENE_H_
#include "stdafx.h"
#define SCREENWIDTH 1024
#define SCREENHEIGHT 800
#include "Misc/Script.h"
#include "Misc/Logger.h"
#include "Objects/Mesh.h"
#include "Objects/Light.h"
#include "Objects/Particle.h"
#include "Objects/Sound.h"
#include "Objects/Collider.h"
#include "Objects/EmptyObject.h"
#include "Objects/Camera.h"
#include "EventReceiver.h"
#include "Irrlicht.h"
#include "irrKlang.h"
#include "irrBullet.h"

using namespace irr;
using namespace irrklang;

namespace FCE
{
	
	//TODO:add scene loading from XML
	class SCENE
	{
		public:
			SCENE();
			SCENE(LOGGER* log, IrrlichtDevice* device, FEventReceiver receiver);
			~SCENE();
			void init();
			void update();
			void render();
			int addParticleSystem(core::vector3df pos, core::vector3df dir, core::vector3df scale, std::string filename);
			PARTICLE* editParticleSystem(int id);
			int addSound(std::string filename, core::vector3df pos, bool loop);
			SOUND* editSound(int id);
			int addMesh(std::string filename, core::vector3df pos, core::vector3df rot, core::vector3df scale);
			MESH* editMesh(int id);
			int addLight(core::vector3df pos, core::vector3df rot, core::vector3df scale, float dropoff, video::E_LIGHT_TYPE type);
			LIGHT* editLight(int id);
			int addEmptyObject(core::vector3df pos, core::vector3df rot, core::vector3df scale);
			EMPTYOBJECT* editEmpty(int id);
			
			bool keyDown(EKEY_CODE keycode);
			irrBulletWorld* getWorld();
			IrrlichtDevice* getDevice();
			LOGGER* getLog();
		private:
			
			//Standard variables
			float timeStamp;
			float deltaTime;
			int soundID;
			int lastID;
			
			//Irrlicht Classes
			IrrlichtDevice* device;
			gui::IGUIEnvironment* gui;
			scene::ISceneManager* manager;
			ISoundEngine* sound;
			irrBulletWorld *world;
			
			//Custom Classes
			LOGGER* log;
			SCRIPT* mainScript;
			CAMERA* camera;
			std::vector<OBJECT*> objects;
			FEventReceiver receiver;
			
			//Other
			lua_State* L;
			
			
			

			

	};
}
#endif // _SCENE_H_
