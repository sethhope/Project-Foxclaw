#ifndef _COLLIDER_H_
#define _COLLIDER_H_
#include "stdafx.h"
#include "Node.h"
#include "Object.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
#include "irrBullet.h"

#define COL_CUBE 0
#define COL_SPHERE 1
#define COL_MESH 2

#define MAX_TYPE 2
using namespace irr;
namespace FCE
{
	
	class COLLIDER : public OBJECT
	{
		public:
			COLLIDER(NODE* parent, scene::ISceneManager* manager, irrBulletWorld* world, LOGGER* log, float mass, int type);
			~COLLIDER();
			
			void onInit();
			void onUpdate();
			void onRender();
			
			void setType(int type);
			void setMass(float mass);
			void setVelocity(core::vector3df velocity);
			void setFriction(float friction);
			void setDamping(float lDamping, float aDamping);
			void setMesh(scene::IMesh* colMesh);
			void setID(int id);
			int getID();
		private:
			int type;
			int id;
			float friction, mass, damping;
			float rad;
			LOGGER* log;
			irrBulletWorld *world;
			scene::IMesh* colMesh;
			IRigidBody* body;
	};
}
#endif
