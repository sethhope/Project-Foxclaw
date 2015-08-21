#ifndef _FOBJECT_H_
#define _FOBJECT_H_
#include "stdafx.h"
#include <map>
#include <sstream>
#include "node.h"
#include "Collider.h"
#include "Irrlicht.h"
#include "irrBullet.h"
#include "Misc/Logger.h"
#include "Game/ShaderCallback.h"
#include "Game/ShaderHandler.h"
using namespace irr;
using namespace core;
namespace FCE
{
	class OBJECT : public NODE
	{
		public:
			OBJECT();
			~OBJECT();
			virtual void onInit(){};
			virtual void onUpdate(){};
			virtual void onRender(){};
			virtual void onDeconstruct(){};
			virtual std::string getOType(){return "OBJECT";};
			void init();
			void update();
			void render();
			void deconstruct();
			void setPosition(core::vector3df pos);
			void setRotation(core::vector3df rot);
			void setScale(core::vector3df scale);
			void addCollider(LOGGER* log, u16 type, scene::ISceneManager* manager, irrBulletWorld* world, f32 mass);
			void addCollider(LOGGER* log, u16 type, scene::ISceneManager* manager, irrBulletWorld* world, f32 mass, scene::IMesh* colMesh);
			void setMetaData(std::string key, f32 data);
			f32 getMetaData(std::string key);
			void useShader(IrrlichtDevice* device, LOGGER* log, std::string shaderFile);
			void setID(u32 id);
			u32 getID();
			COLLIDER* getCollider();
			core::vector3df getPosition();
			core::vector3df getRotation();
			core::vector3df getScale();
			core::vector3df getUpVector();
			bool hasCollider;
			void setName(std::string name);
			std::string getName();
			std::string shaderName;
			void setShaderConstant(u32 shader, std::string key, f32 data);
            ShaderHandler* shader;
			bool hasShader;
			bool boneAttached;
			bool uDa;
		protected:
			core::vector3df rotation;
			core::vector3df position;
			core::vector3df scale;
			std::string name;
			COLLIDER* collider;
			u32 id;
		private:
			std::map<std::string, f32> metadata;
			std::map<std::string, f32> vsshaderConstants;
			std::map<std::string, f32> fsshaderConstants;
			bool initialized;


	};
}
#endif //_FOBJECT_H_
