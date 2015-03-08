set ( SOURCES 
	"src/camera_plain.cpp"
	"src/interface.cpp"
)

set (HEADERS 
	"include/camera_plain.h"
	${HEADERS_SHARED}
)

include_directories("include")

add_library ( visualizer_camera_plain SHARED ${SOURCES} ${HEADERS})
target_link_libraries(visualizer_camera_plain PRIVATE lms)
