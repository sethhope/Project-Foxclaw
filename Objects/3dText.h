#ifndef _3DTEXT_H_INCLUDED
#define _3DTEXT_H_INCLUDED
#include "stdafx.h"
#include "Object.h"
#include "Misc/Logger.h"

namespace FCE
{
    class TEXT3D : public OBJECT
    {
    public:
        TEXT3D(scene::ISceneManager* manager, LOGGER* log);
        ~TEXT3D();
        void onInit();
        void onUpdate();
        void onRender();
        std::string getOType();
        void setText(std::string text);
        std::string getText();
        void setSize(f32 x, f32 y);
    private:
        bool created;
        std::string text;
        f32 sizeX, sizeY;
        LOGGER* log;
        scene::ISceneManager* manager;
    };
}

#endif // 3DTEXT_H_INCLUDED
