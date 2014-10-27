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
#include "Irrlicht.h"
#include "irrKlang.h"

using namespace irr;
using namespace irrklang;

namespace FCE
{
	
	//TODO: create sound type as OBJECT*
	typedef struct
	{
		ISound* s;
		vec3df pos;
		int id;
		float minDistance;
	} SFX;
	
	//TODO:add scene loading from XML
	class SCENE
	{
		public:
			SCENE();
			SCENE(LOGGER* log, IrrlichtDevice* device);
			~SCENE();
			void init();
			void update();
			void render();
			
			int addParticleSystem(core::vector3df pos, core::vector3df dir, core::vector3df scale, std::string filename);
			PARTICLE* editParticleSystem(int id);
			int addSound(std::string filename, vec3df pos, bool loop);
			void stopSound(int id);
			bool isPlaying(int id);
			
			int addMesh(std::string filename, core::vector3df pos, core::vector3df rot, core::vector3df scale);
			MESH* editMesh(int id);
			
			int addLight(core::vector3df pos, core::vector3df rot, core::vector3df scale, video::E_LIGHT_TYPE type);
			LIGHT* editLight(int id);
			
			IrrlichtDevice* getDevice();
			LOGGER* getLog();
		private:
			
			lua_State* L;
			LOGGER* log;
			
			IrrlichtDevice* device;
			gui::IGUIEnvironment* gui;
			scene::ISceneManager* manager;
			
			ISoundEngine* sound;
			scene::ISceneNode* node;
			
			vec3df cameraPos;
			vec3df cameraRot;
			
			SCRIPT* mainScript;
			
			//TODO: Save sounds as objects rather than SFX structs
			std::vector<SFX> sfx;
			std::vector<OBJECT*> objects;

			
			int soundID;
			int lastID;
	};
}
#endif // _SCENE_H_
