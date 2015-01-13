#ifndef _FSCENE_H_
#define _FSCENE_H_
#include "stdafx.h"
#define SCREENWIDTH 1024
#define SCREENHEIGHT 800
#include <map>
#include "Misc/Script.h"
#include "Misc/Logger.h"
#include "Objects/Mesh.h"
#include "Objects/Light.h"
#include "Objects/Particle.h"
#include "Objects/Sound.h"
#include "Objects/Collider.h"
#include "Objects/EmptyObject.h"
#include "Objects/Camera.h"
#include "Objects/AnimatedMesh.h"
#include "EventReceiver.h"
#include "Irrlicht.h"
#include "irrKlang.h"
#include "irrBullet.h"
#include "GUI.h"
using namespace irr;
using namespace core;
using namespace irrklang;

namespace FCE
{
	
	//TODO:add scene loading from XML
	class SCENE
	{
		public:
			SCENE();
			SCENE(LOGGER* log, IrrlichtDevice* device);
			~SCENE();
			void init(FEventReceiver receiver);
			void update(FEventReceiver receiver);
			void render();
			
			void removeObject(u32 id);
			
			u32 addParticleSystem(core::vector3df pos, core::vector3df dir, core::vector3df scale, std::string filename);
			u32 addSound(std::string filename, core::vector3df pos, bool loop);
			u32 addMesh(std::string filename, core::vector3df pos, core::vector3df rot, core::vector3df scale);
			u32 addLight(core::vector3df pos, core::vector3df rot, core::vector3df scale, f32 dropoff, video::E_LIGHT_TYPE type);
			u32 addEmptyObject(core::vector3df pos, core::vector3df rot, core::vector3df scale);
			u32 addAnimatedMesh(std::string filename, core::vector3df pos, core::vector3df rot, core::vector3df scale);
			void setCharacter(IKinematicCharacterController* character);
			void setDebug(bool debug);
			void setSkydome(std::string filename);
			u32 addGui(GUI* gui, u32 parentID);
			PARTICLE* editParticleSystem(u32 id);
			SOUND* editSound(u32 id);
			MESH* editMesh(u32 id);
			LIGHT* editLight(u32 id);
			EMPTYOBJECT* editEmpty(u32 id);
			ANIMATEDMESH* editAnimatedMesh(u32 id);
			CAMERA* getCamera();
			OBJECT* getObject(u32 id);
			IKinematicCharacterController* getCharacter();
			irrBulletWorld* getWorld();
			IrrlichtDevice* getDevice();
			LOGGER* getLog();
			void save(std::string filename);
			void setMetaData(std::string key, f32 data);
			f32 getMetaData(std::string key);

			f32 getTimeScale();
			void setTimeScale(f32 timeScale);
			bool keyDown(EKEY_CODE keycode);
			bool MouseDown(u8 button);
			position2di MousePos();
			std::vector<GUI*> guiObjects;
			std::vector<GUI*> callers;
			lua_State* L;
			u32 lastID;
			
			void load(std::string filename);
		private:
			
			//Standard variables
			f32 timeStamp;
			f32 deltaTime;
			f32 timeScale;
			u32 soundID;
			std::map<std::string, f32> metadata;
			u32 lastGUI;
			std::string skyFile;
			bool debug;
			
			//Irrlicht Classes
			IrrlichtDevice* device;
			gui::IGUIEnvironment* gui;
			scene::ISceneManager* manager;
   ISoundEngine* sound;
			irrBulletWorld *world;
			scene::ISceneNode* skydome;
			IKinematicCharacterController* character;
			
			//Custom Classes
			LOGGER* log;
			SCRIPT* mainScript;
			CAMERA* camera;

			std::vector<OBJECT*> objects;
			FEventReceiver receiver;
			
			
			
			

			

	};
}
#endif // _SCENE_H_
