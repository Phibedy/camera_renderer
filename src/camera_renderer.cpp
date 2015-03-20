#include <camera_plain.h>
#include <lms/imaging/converter.h>
#include <ogre/visualmanager.h>
#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreWindowEventUtilities.h>
#include <OGRE/OgrePlugin.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreMaterial.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreTechnique.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreHardwarePixelBuffer.h>
#include <OGRE/OgreRectangle2D.h>
using namespace Ogre;

bool Camera_plain::initialize(){
    logger.debug("init") <<"init camera_plain";
    //set values
    lastWidth = 0;
    lastHeight = 0;

    //get the image
    image = datamanager()->readChannel<lms::imaging::Image>(this,"CAMERA_IMAGE");

    //get the window you want to draw an
    window = VisualManager::getInstance()->getWindow(this,"Image");
    //setup material for texture
    imageGroundMaterial = Ogre::MaterialManager::getSingleton().create("CameraImageGroundMaterial",
                          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);


    return true;
}

bool Camera_plain::deinitialize(){
    logger.error("Deinit:")<< "Not implemented yet";
    return false;
}

bool Camera_plain::cycle (){
    //set camera values
    window->getCamera()->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
    window->getCamera()->setPosition(Ogre::Vector3(0,0,-1));
    window->getCamera()->lookAt(Ogre::Vector3::ZERO);

    //check if bounds of the image changed
    if (lastWidth != image->width() ||lastHeight != image->height()) {
        lastWidth = image->width();
        lastHeight = image->height();
        setupEnvironment(image->width(), image->height());
    }
    //draw the image
    drawImage();
    return true;
}

void Camera_plain::setupEnvironment( int w, int h ){
    //TODO destroy old stuff if this method is called a second time
    imageTexture = TextureManager::getSingleton().createManual(
                       "ImageImageTextureMap", // name
                       ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                       TEX_TYPE_2D,      // type
                       w, h,         // width & height
                       0,                // number of mipmaps
                       PF_BYTE_BGRA,     // pixel format
                       TU_DEFAULT);

    //TODO Maybe that can be done else
    //TODO destroy it
    Ogre::TextureUnitState *unitState = imageGroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();
    unitState->setTexture(imageTexture);
    unitState->setColourOperationEx(Ogre::LBX_BLEND_TEXTURE_ALPHA, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);

    if(rootNode != nullptr){
        //clear the rootnode
        rootNode->removeAllChildren();
        rootNode->detachAllObjects();
    }else{
        rootNode = window->getSceneManager()->getRootSceneNode()->createChildSceneNode("CameraPlainNode");
    }
    if(rect != nullptr){
        delete rect;
    }
    // Create background rectangle covering the whole screen
    rect = new Rectangle2D(true);
    //TODO create rect in world-coordinates (size of the image)
    rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    rect->setMaterial("CameraImageGroundMaterial");
    rootNode->attachObject(rect);
}

void Camera_plain::drawImage(){
    HardwarePixelBufferSharedPtr pixelBuffer = imageTexture->getBuffer();
    // Lock the pixel buffer and get a pixel box
    pixelBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
    const PixelBox& pixelBox = pixelBuffer->getCurrentLock();
    uint8* pDest = static_cast<uint8*>(pixelBox.data);
    lms::imaging::convertRaw(image->format(),image->data(),image->size(),lms::imaging::Format::BGRA, pDest);
    pixelBuffer->unlock();
}
