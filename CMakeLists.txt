cmake_minimum_required (VERSION 2.8.1)

######## Project settings ########
PROJECT(FaceTracker)
SET(LICENSE "TBD")

######## Build and include settings ########


find_package( OpenCV REQUIRED )

include_directories(
	inc
)

link_directories(
	${LINK_DIRECTORIES}
)


file(GLOB SOURCES
	"src/*.cpp"
)

add_executable(
	FaceTracker

	${SOURCES}
)

TARGET_LINK_LIBRARIES(
	FaceTracker
	opencv_highgui
	opencv_core
	opencv_imgproc
	opencv_objdetect
	pthread
)

######## Install targets ########
INSTALL(TARGETS FaceTracker
	RUNTIME DESTINATION usr/bin
)
