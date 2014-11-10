#ifndef _FNODE_H_
#define _FNODE_H_
#include "stdafx.h"
#include "irrlicht.h"
using namespace irr;
namespace FCE
{
	class NODE
	{
		public:
			NODE();
			~NODE();
			NODE* getParent();
			NODE* getChild();
			NODE* getNext();
			NODE* getPrev();
			NODE* getLastChild();
			NODE* getFirstParent();
			void attach(NODE* node);
			void detach();
			void addSibling(NODE* node);
			scene::ISceneNode* getIrrNode();
		private:
			NODE* parent;
			NODE* child;
			NODE* nextNode;
			NODE* prevNode;
		protected:
			scene::ISceneNode* thisNode;
	};
}
#endif //_NODE_H_
