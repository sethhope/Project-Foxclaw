#include "stdafx.h"
#include "Object.h"
#include "Misc/Logger.h"
#include "Irrlicht.h"
#include "irrBullet.h"

#define COL_CUBE 0
#define COL_SPHERE 1
#define COL_MESH 2
using namespace irr;
namespace FCE
{
	
	class COLLIDER : public OBJECT
	{
		public:
			COLLIDER(scene::ISceneNode* parent, LOGGER* log);
			~COLLIDER();
			
			void onInit();
			void onUpdate();
			void onRender();
			
			void setType(int type);
			void setMass(float mass);
			void setVelocity(core::vector3df velocity);
			void setFriction(float friction);
			void setDamping(float damping);
			
		private:
			int type;
			float friction, mass, damping;
			float rad;
			LOGGER* log;
			IRigidBody* body;
	};
}
