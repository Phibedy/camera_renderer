set ( SOURCES
	"src/image_renderer.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/image_renderer.h"
	${HEADERS_SHARED}
)

include_directories("include")

add_library ( image_renderer SHARED ${SOURCES} ${HEADERS})
#link ogre lms and the ogre_window_manager
target_link_libraries(image_renderer PRIVATE lmscore ogre_binding ogre_window_manager imaging)
