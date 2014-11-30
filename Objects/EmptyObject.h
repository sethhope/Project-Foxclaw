#ifndef _FEMPTYOBJECT_H_
#define _FEMPTYOBJECT_H_
#include "stdafx.h"
#include "Object.h"

namespace FCE
{
	class EMPTYOBJECT : public OBJECT
	{
		public:
			EMPTYOBJECT();
			~EMPTYOBJECT();
			
			void onInit();
			void onUpdate();
			void onRender();
			
	};
}
#endif
