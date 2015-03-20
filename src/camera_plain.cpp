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

using namespace Ogre;

bool Camera_plain::initialize(){
    logger.debug("init") <<"init camera_plain";

    //get camera
    image = datamanager()->readChannel<lms::imaging::Image>(this,"CAMERA_IMAGE");

    firstrun = true;
    window = VisualManager::getInstance()->getWindow(this,"Image");
    imageGroundMaterial = Ogre::MaterialManager::getSingleton().create("CameraImageGroundMaterial",
                          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);


    window->getCamera()->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
    window->getCamera()->setPosition(Ogre::Vector3(0,0,-1));
    //window->getCamera()->setFixedYawAxis(true, Ogre::Quaternion::IDENTITY * Ogre::Vector3::UNIT_Z);
    window->getCamera()->lookAt(Ogre::Vector3::ZERO);

    window->resetCamera();

    return true;
}

bool Camera_plain::deinitialize(){
    logger.error("Deinit:")<< "Not implemented yet";
    //TODO
    return false;
}

bool Camera_plain::cycle (){

    if (firstrun) {
        firstrun = false;
        //TODO
    //    setupEnvironment(info->width, info->height);
          setupEnvironment(320, 240);
    }
    billboards->setDefaultDimensions(window->getGfxWindow()->getWidth()-1, window->getGfxWindow()->getHeight()-1);
    //printf("%i|%i\n", window->getGfxWindow()->getWidth(), window->getGfxWindow()->getHeight());
    window->resetCamera();

    drawImage();

    return true;
}

void Camera_plain::setupEnvironment( int w, int h ){

    imageTexture = TextureManager::getSingleton().createManual(
                       "ImageImageTextureMap", // name
                       ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                       TEX_TYPE_2D,      // type
                       w, h,         // width & height
                       0,                // number of mipmaps
                       PF_BYTE_BGRA,     // pixel format
                       TU_DEFAULT);

    Ogre::TextureUnitState *tusDebug = imageGroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();
    tusDebug->setTexture(imageTexture);
    tusDebug->setColourOperationEx(Ogre::LBX_BLEND_TEXTURE_ALPHA, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);

    rootNode = window->getSceneManager()->getRootSceneNode()->createChildSceneNode("CameraPlainNode");
    billboards = window->getSceneManager()->createBillboardSet("BBSetCameraCamera");
    billboards->setMaterial(imageGroundMaterial);
    billboards->setBillboardOrigin(Ogre::BillboardOrigin::BBO_CENTER);
    //billboards->setDefaultDimensions(w*4.2,h*4.2);
    rootNode->attachObject(billboards);
    rootNode->setPosition(Ogre::Vector3::ZERO);

    billboards->createBillboard(Ogre::Vector3::ZERO);

}

//http://www.ogre3d.org/forums/viewtopic.php?f=5&t=55824
void Camera_plain::drawImage(){

    HardwarePixelBufferSharedPtr pixelBuffer = imageTexture->getBuffer();
    // Lock the pixel buffer and get a pixel box
    pixelBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
    const PixelBox& pixelBox = pixelBuffer->getCurrentLock();
    logger.debug("draw image") << pixelBox.getRowSkip() << " " <<Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
    uint8* pDest = static_cast<uint8*>(pixelBox.data);
    //TODO set image
    lms::imaging::convertRaw(image->format(),image->data(),image->size(),lms::imaging::Format::BGRA, pDest);
    pixelBuffer->unlock();

    /*
    HardwarePixelBufferSharedPtr pixelBuffer = imageTexture->getBuffer();

    // Lock the pixel buffer and get a pixel box
    pixelBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
    const PixelBox& pixelBox = pixelBuffer->getCurrentLock();

    uint8* pDest = static_cast<uint8*>(pixelBox.data);
    for (unsigned int i = 0; i < 240; ++i) {
        for (unsigned int j = 0; j < 320; ++j) {
            unsigned char r, g, b;
            r = g = b = 100;
            //WHY????
            if (i == 0 || j == 0 || i == 240 - 1 || j == 320 - 1) {
                *pDest++ = 0; // B
                *pDest++ = 0; // G
                *pDest++ = 255; // R
                *pDest++ = 255;   // A
            } else {
                *pDest++ = b; // B
                *pDest++ = g; // G
                *pDest++ = r; // R
                *pDest++ = 255;   // A
            }
            pDest += pixelBox.getRowSkip() * Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
        }
    }
    pixelBuffer->unlock();
    */

}
