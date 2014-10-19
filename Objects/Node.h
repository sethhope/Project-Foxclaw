#ifndef _FNODE_H_
#define _FNODE_H_
#include "stdafx.h"

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
		private:
			NODE* parent;
			NODE* child;
			NODE* nextNode;
			NODE* prevNode;

	};
}
#endif //_NODE_H_
