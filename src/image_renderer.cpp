#include <image_renderer.h>
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

//test
#include <lms/imaging/image_factory.h>
//TODO remove that
using namespace Ogre;

std::string ImageRenderer::groundMatName = "CameraImageGroundMaterial";

bool ImageRenderer::initialize(){
    logger.debug("init") <<"init";
    //set values
    lastWidth = 0;
    lastHeight = 0;
    m_groundMatName = groundMatName + getName();
    //get the image
    image = datamanager()->readChannel<lms::imaging::Image>(this,"IMAGE");

    //get the window you want to draw an
    window = VisualManager::getInstance()->getWindow(this,getChannelMapping("WINDOW"));
    //setup material for texture
    imageGroundMaterial = Ogre::MaterialManager::getSingleton().create(m_groundMatName,
                          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    imageGroundMaterial->getTechnique(0)->getPass(0)->setSceneBlending(SceneBlendType::SBT_TRANSPARENT_ALPHA);
    imageGroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);

    return true;
}

bool ImageRenderer::deinitialize(){
    logger.error("Deinit:")<< "Not implemented yet";
    return false;
}

bool ImageRenderer::cycle (){
    //set camera values
    window->getCamera()->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
    window->getCamera()->setPosition(Ogre::Vector3(0,0,-1));
    window->getCamera()->lookAt(Ogre::Vector3::ZERO);
    if(image->width() == 0 || image->height() == 0){
        logger.error("ZERO");
    }else{
        //check if bounds of the image changed
        if (lastWidth != image->width() ||lastHeight != image->height()) {
            lastWidth = image->width();
            lastHeight = image->height();
            setupEnvironment(image->width(), image->height());
            logger.debug("imageBounds changed");
        }
        //draw the image
        drawImage();
    }
    return true;
}

void ImageRenderer::setupEnvironment( int w, int h ){
    static std::string textureName = "ImageImageTextureMap";
    //TODO destroy old stuff if this method is called a second time
    imageTexture = TextureManager::getSingleton().createManual(
                       textureName+getName(), // name
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
    unitState->setColourOperationEx(Ogre::LBX_BLEND_TEXTURE_ALPHA);//, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);

    if(rootNode != nullptr){
        //clear the rootnode
        //that should never be called!
        logger.error("Called weird part of init");
        rootNode->removeAllChildren();
        rootNode->detachAllObjects();
    }else{
        if(window->getSceneManager()->hasSceneNode("CameraNode")){
            rootNode = window->getSceneManager()->getSceneNode("CameraNode");
        }else{
            rootNode = window->getSceneManager()->getRootSceneNode()->createChildSceneNode("CameraNode");
        }
    }
    if(rect != nullptr){
        delete rect;
    }
    // Create background rectangle covering the whole screen
    rect = new Rectangle2D(true);
    //TODO create rect in world-coordinates (size of the image)
    rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    rect->setMaterial(m_groundMatName);
    // Use infinite AAB to always stay visible
    AxisAlignedBox aabInf;
    aabInf.setInfinite();
    rect->setBoundingBox(aabInf);
    if(getPriority() < 0 || getPriority() > 255) {
        logger.error("setupEnvironment") << "Don't summon the evil! "
                                         << "Priority is out of range 0-255: "
                                         << getPriority();
        //TODO
        return;
    }
    std::uint8_t priority =getPriority(); //255 - getPriority();

    rect->setRenderQueueGroup(priority);

    rootNode->attachObject(rect);
}

int ImageRenderer::getPriority(){
    return getChannelPriority("WINDOW");
}

void ImageRenderer::drawImage(){
    HardwarePixelBufferSharedPtr pixelBuffer = imageTexture->getBuffer();
    // Lock the pixel buffer and get a pixel box
    pixelBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
    const PixelBox& pixelBox = pixelBuffer->getCurrentLock();
    uint8* pDest = static_cast<uint8*>(pixelBox.data);
    lms::imaging::convertRaw(image->format(),image->data(),image->size(),lms::imaging::Format::BGRA, pDest);
    pixelBuffer->unlock();

}
