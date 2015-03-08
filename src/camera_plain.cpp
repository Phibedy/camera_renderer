#include <camera_plain.h>

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

using namespace Ogre;

bool Camera_plain::initialize(){

    printf("Init: camera_plain\n");
    firstrun = true;
    debug_enabled = true;
//    handleImage = datamanager()->acquire_channel<unsigned char*>("IMAGE_RAW", Access::READ);

    window = VisualManager::getInstance()->getWindow("Image");

    imageGroundMaterial = Ogre::MaterialManager::getSingleton().create("CameraImageGroundMaterial",
                          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    window->addKeyFunction(OIS::KC_P, [this]() {
        debug_enabled = !debug_enabled;
    });

    window->getCamera()->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
    window->getCamera()->setPosition(Ogre::Vector3(0,0,-1));
    //window->getCamera()->setFixedYawAxis(true, Ogre::Quaternion::IDENTITY * Ogre::Vector3::UNIT_Z);
    window->getCamera()->lookAt(Ogre::Vector3::ZERO);

    window->resetCamera();

    return true;
}

bool Camera_plain::deinitialize()
{
    printf("Deinit: camera_plain");

    //TODO

    return true;
}

bool Camera_plain::cycle (){

    //Data::ImageInfo *info = handleImageInfo->get();

    if (firstrun) {
        firstrun = false;
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

void Camera_plain::drawImage(){
    /*
    unsigned char *image = *handleImage->get();
    unsigned char *debug = handleDebug->get();
    Data::ImageInfo *imageInfo = handleImageInfo->get();
    HardwarePixelBufferSharedPtr pixelBuffer = imageTexture->getBuffer();

    // Lock the pixel buffer and get a pixel box
    pixelBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
    const PixelBox& pixelBox = pixelBuffer->getCurrentLock();

    uint8* pDest = static_cast<uint8*>(pixelBox.data);
    for (unsigned int i = 0; i < imageInfo->height; ++i) {
        for (unsigned int j = 0; j < imageInfo->width; ++j) {
            unsigned char r, g, b;
            r = g = b = image[i * imageInfo->width + j];
            unsigned char dbg_color = debug[i * imageInfo->width + j];
            if (debug_enabled && dbg_color != 0) {
                switch (dbg_color) {
                case 0:
                    break;
                case 1:
                    r = 0;
                    g = 0;
                    b = 255;
                    break;
                case 2:
                    r = 255;
                    g = 0;
                    b = 0;
                    break;
                case 3:
                    r = 0;
                    g = 255;
                    b = 0;
                    break;
                case 4:
                    r = 255;
                    g = 255;
                    b = 0;
                    break;
                case 5:
                    r = 255;
                    g = 0;
                    b = 255;
                    break;
                case 6:
                    r = 0;
                    g = 0;
                    b = 0;
                    break;
                case 10:
                    r = 255.*0.3;
                    g = 255.*0.3;
                    b = 255;
                    break;
                default:
                    printf("Unrecognized image debug color! Code:%i \n",dbg_color);
                    r = 100;
                    g = 255;
                    b = 100;
                    printf("x:%i y:%i \n",j,i);
                    break;
                }
            }
            if (i == 0 || j == 0 || i == imageInfo->height - 1 || j == imageInfo->width - 1) {
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
