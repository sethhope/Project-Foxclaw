#include "NODE.h"

using namespace FCE;
using namespace irr;

NODE::NODE()
{
	child = NULL;
	parent = NULL;
}

NODE::~NODE()
{
	if(parent)
	{
		if(child)
		{
			child->parent = parent;
			parent->child = child;
		}else
		{
			parent->child = NULL;
		}
	}else
	{
		if(child)
		{
			child->parent = NULL;
		}
	}

}

void NODE::attachTo(NODE* node)
{
	parent = node->getLastChild();
	if(thisNode&&parent->getIrrNode())
	{
		thisNode->setParent(parent->getIrrNode());
	}
	node->getLastChild()->child = this;
}

void NODE::detach()
{
	if(parent)
	{
		parent->child = NULL;
		parent->getIrrNode()->removeChild(thisNode);
		parent = NULL;
	}
}

void NODE::addSibling(NODE* node)
{
}

NODE* NODE::getChild()
{
	if(child)
	{
		return child;
	}
	return NULL;
}

NODE* NODE::getParent()
{
	if(parent)
	{
		return parent;
	}
	return NULL;
}

NODE* NODE::getNext()
{
	if(nextNode)
	{
		return nextNode;
	}
	return NULL;
}

NODE* NODE::getPrev()
{
	if(prevNode)
	{
		return prevNode;
	}
	return NULL;
}

NODE* NODE::getLastChild()
{
	if(child)
	{
		return child->getLastChild();
	}
	else
	{
		return this;
	}
}

NODE* NODE::getFirstParent()
{
	if(parent)
	{
		return parent->getFirstParent();
	}else
	{
		return this;
	}
}

scene::ISceneNode* NODE::getIrrNode()
{
	return thisNode;
}

