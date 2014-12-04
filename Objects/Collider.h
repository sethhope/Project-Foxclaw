#ifndef _COLLIDER_H_
#define _COLLIDER_H_
#include "stdafx.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
#include "irrBullet.h"

#define COL_CUBE 0
#define COL_SPHERE 1
#define COL_MESH_GIMPACT 2
#define COL_MESH_CONVEXHULL 3
#define COL_MESH_TRIMESH 4

#define MAX_TYPE 4
using namespace irr;
namespace FCE
{
	
	class COLLIDER
	{
		public:
			COLLIDER(scene::ISceneNode* parent, scene::ISceneManager* manager, irrBulletWorld* world, int type, float mass, LOGGER* log);
			~COLLIDER();
			void init();
			void setType(int type);
			void setMass(float mass);
			void setVelocity(core::vector3df velocity);
			void setFriction(float friction);
			void setDamping(float lDamping, float aDamping);
			void setMesh(scene::IMesh* colMesh);
		private:
			int type;
			float friction, mass, damping;
			float rad;
			irrBulletWorld *world;
			scene::IMesh* colMesh;
			IRigidBody* body;
			LOGGER* log;
			scene::ISceneNode* node;
	};
}
#endif
