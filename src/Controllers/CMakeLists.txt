set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRCS
		ControllerStatus.h
		ControllerStatus.cpp ../Common/EventSource.h)

add_library(controllers STATIC ${SRCS})
target_link_libraries(controllers PUBLIC ${LIBS})