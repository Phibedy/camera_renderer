#ifndef VISUALIZER_CAMERA_PLAIN_VISUALIZER_H
#define VISUALIZER_CAMERA_PLAIN_VISUALIZER_H

#include <ogre/visualmanager.h>
#include <lms/datamanager.h>
#include <lms/module.h>
#include <ogre/window.h>
#include <lms/imaging/image.h>

#include <OGRE/OgreBillboardSet.h>
#include <OGRE/OgreMaterial.h>
#include <OGRE/OgreTexture.h>

class Camera_plain : public lms::Module {
public:
	bool initialize();
	bool deinitialize();

	bool cycle();

protected: 
    visual::Window *window;
    float plane_size;
    int texture_size;
    /**
     * @brief image the image to draw
     */
    const lms::imaging::Image* image;

    Ogre::MaterialPtr imageGroundMaterial;
    Ogre::TexturePtr imageTexture;
    
    Ogre::SceneNode *rootNode;
    Ogre::BillboardSet *billboards;
private:
    bool firstrun; 
    
    void setupEnvironment(int width, int height);
    void drawImage();
};

#endif
