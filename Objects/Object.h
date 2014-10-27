#ifndef _FOBJECT_H_
#define _FOBJECT_H_
#include "stdafx.h"
#include "node.h"
#include "Irrlicht.h"
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
			void setName(std::string name);
			std::string getName();
		protected:
			core::vector3df localPosition;
			core::vector3df globalPosition;
			core::vector3df localRotation;
			core::vector3df globalRotation;
			core::vector3df localScale;
			core::vector3df globalScale;
			std::string name;
	};
}
#endif //_FOBJECT_H_
