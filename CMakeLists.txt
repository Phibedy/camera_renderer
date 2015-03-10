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
target_link_libraries(camera_renderer PRIVATE lms)
