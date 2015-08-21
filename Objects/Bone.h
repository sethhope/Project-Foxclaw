#ifndef _FBONEOBJECT_H_
#define _FBONEOBJECT_H_
#include "stdafx.h"
#include "Object.h"

namespace FCE
{
class BONEOBJECT : public OBJECT
{
public:
    BONEOBJECT(scene::ISceneNode* bone);
    ~BONEOBJECT();

    void onInit();
    void onUpdate();
    void onRender();
    std::string getOType();

};
}
#endif
