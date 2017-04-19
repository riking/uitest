set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRCS
	uihello.cpp
	configui.cpp
	ControllerStatusPanel.cpp
		asset.cpp asset.h)

set(WXLIBS ${wxWidgets_LIBRARIES})
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(WXLIBS ${WXLIBS} dl)
endif()

add_executable(uitest ${SRCS})
target_link_libraries(uitest ${LIBS} ${WXLIBS})
