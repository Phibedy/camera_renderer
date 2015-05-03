#include <camera_renderer.h>

extern "C" {
void* getInstance () {
	return new Camera_plain();
}
}
