#include <camera_plain.h>

extern "C" {
void* getInstance () {
	return new Camera_plain();
}
const char* getName() {
	return "Camera_plain"; 
}
}
