#ifndef _FEMPTYOBJECT_H_
#define _FEMPTYOBJECT_H_
#include "stdafx.h"
#include "Object.h"

namespace FCE
{
	class EMPTYOBJECT : public OBJECT
	{
		public:
			EMPTYOBJECT(scene::ISceneManager* manager);
			~EMPTYOBJECT();
			
			void onInit();
			void onUpdate();
			void onRender();
			std::string getOType();
			
	};
}
#endif
