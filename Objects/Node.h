#ifndef _FNODE_H_
#define _FNODE_H_
#include "stdafx.h"
#include "Irrlicht.h"
using namespace irr;
namespace FCE
{
	class NODE
	{
		public:
			NODE();
			~NODE();
			NODE* getParent();
			NODE* getChild(u32 id);
			NODE* getFirstParent();
			void attachTo(NODE* node);
			void detach();
			scene::ISceneNode* getIrrNode();
			
			void addChild(NODE* n);
			void removeChild(u32 id);
		private:
			NODE* parent;
			
  		protected:
  			u32 cID;
			u32 _cID;
			scene::ISceneNode* thisNode;
			std::vector<NODE*> children;
	};
}
#endif //_NODE_H_
