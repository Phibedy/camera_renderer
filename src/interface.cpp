#include <image_renderer.h>

extern "C" {
void* getInstance () {
    return new ImageRenderer();
}
}
