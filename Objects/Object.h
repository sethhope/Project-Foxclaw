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
		protected:
			core::vector3df position;
			core::vector3df rotation;
			core::vector3df scale;
	};
}
#endif //_FOBJECT_H_
