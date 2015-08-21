#include "Terrain.h"

using namespace irr;
using namespace FCE;

TERRAIN::TERRAIN(scene::ISceneManager* manager, LOGGER* log)
{
    this->log = log;
    this->manager = manager;
    mesh = new scene::SMesh();
}

TERRAIN::~TERRAIN()
{
    mesh->drop();
    log->debugData(MAJOR, "DROPPING MESH");
}
void TERRAIN::onDeconstruct()
{
    log->debugData(MAJOR, "DROPPING MESH");
    mesh->clear();
    //mesh->drop();
    //m->remove();
}
void TERRAIN::onInit()
{
    hMap = new HEIGHTMAP(100, 100);
    empty(100, 100);
    fromFile = false;
}

void TERRAIN::onUpdate()
{

}

void TERRAIN::onRender()
{

}

void TERRAIN::load(std::string filename)
{
    mesh->clear();
    fromFile = true;
    log->debugData(MAJOR, "Loading heightmap from file");
    this->filename = filename;
    if(hMap)
    {
        delete hMap;
    }
    video::IImage* image = manager->getVideoDriver()->createImageFromFile(filename.c_str());
    if(!image)
    {
        log->logData("Failed to load",filename);
        return;
    }
    dimension2d<u32> size = image->getDimension();
    u32 width = size.Width;
    u32 height = size.Height;
    hMap = new HEIGHTMAP(height, width);
    for(u32 x = 0; x < width; x++)
    {
        for(u32 y = 0; y < height; y++)
        {
            hMap->setHeight(x, y, image->getPixel(x, y).getAverage());
        }
    }

    makeMesh();
}

void TERRAIN::empty(u32 width, u32 height, u32 tilefactor)
{
    this->tilefactor = tilefactor;
    mesh->clear();
    fromFile = false;
    log->debugData(MAJOR, "making empty heightmap");
    if(hMap)
    {
        delete hMap;
    }
    hMap = new HEIGHTMAP(height, width);
    for(u32 x = 0; x < width; x++)
    {
        for(u32 y = 0; y < height; y++)
        {
            hMap->setHeight(x, y, 0);
        }
    }
    makeMesh();
}

void TERRAIN::makeMesh()
{
    u32 mp = manager->getVideoDriver()->getMaximalPrimitiveCount();
    u32 sw = mp / (6*hMap->getHeight());

    u32 i=0;
    for(u32 y0 = 0; y0 < hMap->getHeight(); y0+=sw)
    {
        u32 y1 = y0+sw;
        if(y1 >= hMap->getHeight())
        {
            y1 = hMap->getHeight()-1;
        }
        addstrip(y0, y1, i);
        i++;
    }
    if(i<mesh->getMeshBufferCount())
    {
        for(u32 j=i; j<mesh->getMeshBufferCount(); j++)
        {
            mesh->getMeshBuffer(j)->drop();
        }
        mesh->MeshBuffers.erase(i, mesh->getMeshBufferCount()-i);
    }
    mesh->setDirty();
    mesh->recalculateBoundingBox();

    thisNode = manager->addMeshSceneNode(mesh);
}

void TERRAIN::remakeMesh()
{
    u32 mp = manager->getVideoDriver()->getMaximalPrimitiveCount();
    u32 sw = mp / (6*hMap->getHeight());

    u32 i=0;
    for(u32 y0 = 0; y0 < hMap->getHeight(); y0+=sw)
    {
        u32 y1 = y0+sw;
        if(y1 >= hMap->getHeight())
        {
            y1 = hMap->getHeight()-1;
        }
        addstrip(y0, y1, i);
        i++;
    }
    if(i<mesh->getMeshBufferCount())
    {
        for(u32 j=i; j<mesh->getMeshBufferCount(); j++)
        {
            mesh->getMeshBuffer(j)->drop();
        }
        mesh->MeshBuffers.erase(i, mesh->getMeshBufferCount()-i);
    }
    mesh->setDirty();
    mesh->recalculateBoundingBox();

    manager->addToDeletionQueue(thisNode);
    thisNode = manager->addMeshSceneNode(mesh);
    thisNode->setPosition(position);
    thisNode->setRotation(rotation);
    thisNode->setScale(scale);

}
void TERRAIN::addstrip(u32 y0, u32 y1, u32 bufNum)
{
    scene::SMeshBuffer *buf = 0;
    if (bufNum<mesh->getMeshBufferCount())
    {
        buf = (scene::SMeshBuffer*)mesh->getMeshBuffer(bufNum);
    }
    else
    {
        buf = new scene::SMeshBuffer();
        mesh->addMeshBuffer(buf);
        buf->drop();
    }
    buf->Vertices.set_used((1 + y1 - y0) * hMap->getWidth());

    u32 i=0;
    for (u32 y = y0; y <= y1; ++y)
    {
        for (u32 x = 0; x < hMap->getWidth(); ++x)
        {

            const f32 xx = (f32)((f32)x/(f32)hMap->getWidth());
            const f32 yy = (f32)((f32)y/(f32)hMap->getHeight());
            const f32 z = hMap->getData(x, y);
            video::S3DVertex& v = buf->Vertices[i++];
            v.Pos.set(x, 0.2 * z, y);
            v.Normal.set(hMap->getNormal(x, y, 0.2));
            v.Color=video::SColor(255,255,255,255);
            v.TCoords.set(xx*tilefactor, yy*tilefactor);
        }
    }

    buf->Indices.set_used(6 * (hMap->getWidth() - 1) * (y1 - y0));
    i=0;
    for(u32 y = y0; y < y1; ++y)
    {
        for(u32 x = 0; x < hMap->getWidth() - 1; ++x)
        {
            const u16 n = (y-y0) * hMap->getWidth() + x;
            buf->Indices[i]=n;
            buf->Indices[++i]=n + hMap->getWidth();
            buf->Indices[++i]=n + hMap->getWidth() + 1;
            buf->Indices[++i]=n + hMap->getWidth() + 1;
            buf->Indices[++i]=n + 1;
            buf->Indices[++i]=n;
            ++i;
        }
    }

    buf->recalculateBoundingBox();
}


std::string TERRAIN::getOType()
{
    return "TERRAIN";
}

std::string TERRAIN::getFilename()
{
    return filename;
}

void TERRAIN::setHeight(u32 x, u32 y, f32 height)
{
    hMap->setHeight(x, y, height);
    mesh->clear();
    remakeMesh();
}
void TERRAIN::setHeightNoRebuild(u32 x, u32 y, f32 height)
{
    hMap->setHeight(x, y, height);
}
void TERRAIN::rebuild()
{
    mesh->clear();
    remakeMesh();
}
scene::IAnimatedMesh* TERRAIN::getMesh()
{
    return (scene::IAnimatedMesh*)(((scene::IMeshSceneNode*)thisNode)->getMesh());
}

HEIGHTMAP::HEIGHTMAP(u32 height, u32 width)
{
    this->height = height;
    this->width = width;
    scalingFactor = sqrtf((f32)(width*width+height*height));
    data.set_used(width*height);
}

HEIGHTMAP::~HEIGHTMAP()
{
    data.clear();
}

f32 HEIGHTMAP::getData(u32 x, u32 y)
{
    return data[y*width+x];
}

void HEIGHTMAP::setHeight(u32 x, u32 y, f32 height)
{
    data[y*width+x]=height;

}

core::vector3df HEIGHTMAP::getNormal(u32 x, u32 y, f32 scalingFactor)
{
    f32 zc = getData(x, y);
    f32 zl, zr, zu, zd;

    if (x == 0)
    {
        zr = getData(x + 1, y);
        zl = zc + zc - zr;
    }
    else if (x == width - 1)
    {
        zl = getData(x - 1, y);
        zr = zc + zc - zl;
    }
    else
    {
        zr = getData(x + 1, y);
        zl = getData(x - 1, y);
    }

    if (y == 0)
    {
        zd = getData(x, y + 1);
        zu = zc + zc - zd;
    }
    else if (y == height - 1)
    {
        zu = getData(x, y - 1);
        zd = zc + zc - zu;
    }
    else
    {
        zd = getData(x, y + 1);
        zu = getData(x, y - 1);
    }

    return vector3df(scalingFactor * 2 * (zl - zr), 4, scalingFactor * 2 * (zd - zu)).normalize();
}



