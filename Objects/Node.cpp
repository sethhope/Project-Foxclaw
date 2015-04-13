#include "NODE.h"

using namespace FCE;
using namespace irr;

NODE::NODE()
{
    children.clear();
    parent = NULL;
    _cID = 1;
    cID = 0;
}

NODE::~NODE()
{
    if(parent)
    {
        if(!children.empty())
        {
            for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
            {
                (*it)->parent = parent;
                parent->addChild((*it));
            }
        }
        else
        {
            parent->removeChild(cID);
        }
    }
    else
    {
        if(!children.empty())
        {
            for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
            {
                (*it)->parent = NULL;
                (*it)->cID = 0;
            }
        }
    }
    if(thisNode)
    {
        thisNode->remove();
    }
}

void NODE::attachTo(NODE* node)
{
    parent = node;
    if(thisNode && parent->getIrrNode())
    {
        thisNode->setParent(parent->getIrrNode());
    }
    node->addChild(this);
}

void NODE::detach()
{
    if(parent)
    {
        parent->removeChild(cID);
        parent->getIrrNode()->removeChild(thisNode);
        parent = NULL;
    }
}

NODE* NODE::getChild(u32 id)
{
    for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
    {
        if((*it)->cID == id)
        {
            return ((NODE*)(*it));
        }
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

NODE* NODE::getFirstParent()
{
    if(parent)
    {
        return parent->getFirstParent();
    }
    else
    {
        return this;
    }
}


void NODE::addChild(NODE* n)
{
    bool exists = false;
    for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
    {
        if((*it)==n)
        {
            exists = true;
        }
    }
    if(!exists)
    {
        n->cID = _cID;
        _cID++;
        children.push_back(n);
    }
}

void NODE::removeChild(u32 id)
{
    for(std::vector<NODE*>::iterator it = children.begin(); it < children.end(); it++)
    {
        if((*it)->cID == id)
        {
            children.erase(it);
            return;
        }
    }
}
scene::ISceneNode* NODE::getIrrNode()
{
    return thisNode;
}

