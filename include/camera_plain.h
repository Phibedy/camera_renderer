#ifndef VISUALIZER_CAMERA_PLAIN_VISUALIZER_H
#define VISUALIZER_CAMERA_PLAIN_VISUALIZER_H

#include <ogre/visualmanager.h>
#include <core/datamanager.h>
#include <core/module.h>
#include <ogre/window.h>
//#include <data/image_data.h>
//#include <data/image_info.h>

#include <OGRE/OgreBillboardSet.h>
#include <OGRE/OgreMaterial.h>
#include <OGRE/OgreTexture.h>

class Camera_plain : public lms::Module {
public:
	bool initialize();
	bool deinitialize();

	bool cycle();

protected: 
    visual::window *window;
    float plane_size;
    int texture_size;

//    Handle<unsigned char*> *handleImage;
//    Handle<unsigned char> *handleDebug;
//    Handle<Data::ImageInfo> *handleImageInfo;

    Ogre::MaterialPtr imageGroundMaterial;
    Ogre::TexturePtr imageTexture;
    
    Ogre::SceneNode *rootNode;
    Ogre::BillboardSet *billboards;
private:
    bool debug_enabled;
    bool firstrun; 
    
    void setupEnvironment(int width, int height);
    void drawImage();
};

#endif
