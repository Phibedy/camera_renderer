set ( SOURCES 
	"src/camera_plain.cpp"
	"src/interface.cpp"
)

set (HEADERS 
	"include/camera_plain.h"
	${HEADERS_SHARED}
)

include_directories("include")

add_library ( camera_renderer SHARED ${SOURCES} ${HEADERS})
#link ogre lms and the ogre_window_manager
target_link_libraries(camera_renderer PRIVATE lms ogre_binding ogre_window_manager)
