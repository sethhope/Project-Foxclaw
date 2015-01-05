#ifndef _FCAMERA_H_
#define _FCAMERA_H_
#include "stdafx.h"
#include "Object.h"
#include "Misc/Logger.h"
#include "irrlicht.h"

#define FCAM_FPS 0
#define FCAM_FREE 1
#define FCAM_MAYA 2

using namespace irr;
namespace FCE
{
	class CAMERA : public OBJECT
	{
		public:
			CAMERA(scene::ISceneManager* manager, LOGGER* log);
			~CAMERA();
			
			void onInit();
			void onUpdate();
			void onRender();
			std::string getOType();
			
			void setType(int type);
			int getType();
			void setTarget(OBJECT* target);
			void setTarget(core::vector3df pos);
			void setOffset(core::vector3df offset);
			void setUpVector(core::vector3df pos);
			void setFOV(float fov);
			void setFarClipping(float clip);
			void setNearClipping(float clip);
			void setAspectRatio(float aspect);
			float getFOV();
			float getFarClipping();
			float getNearClipping();
			scene::ISceneManager* getManager();
			scene::ICameraSceneNode* getCamNode();
		private:
			int type;
			core::vector3df offset;
			scene::ICameraSceneNode* camNode;
			LOGGER* log;
			scene::ISceneManager* manager;
			bool initialized;
			bool objectTarget;
			OBJECT* target;
	};
}
#endif
