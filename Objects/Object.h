#ifndef _FOBJECT_H_
#define _FOBJECT_H_
#include "stdafx.h"
#include "node.h"
#include "Collider.h"
#include "Irrlicht.h"
#include "irrBullet.h"
using namespace irr;
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
			void init();
			void update();
			void render();
			void setPosition(core::vector3df pos);
			void setRotation(core::vector3df rot);
			void setScale(core::vector3df scale);
			void addCollider(int type, scene::ISceneManager* manager, irrBulletWorld* world, float mass);
			void addCollider(int type, scene::ISceneManager* manager, irrBulletWorld* world, float mass, scene::IMesh* colMesh);
			COLLIDER* getCollider();
			core::vector3df getPosition();
			core::vector3df getRotation();
			core::vector3df getScale();
			
			void setName(std::string name);
			std::string getName();
		protected:
			core::vector3df rotation;
			core::vector3df position;
			core::vector3df scale;
			std::string name;
			COLLIDER* collider;
		private:
			bool uDa;
			bool hasCollider;

	};
}
#endif //_FOBJECT_H_
