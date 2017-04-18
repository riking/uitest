set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND LIBS
	${PNG}
	z
)

include_directories(.)
add_subdirectory(WxUI)
add_subdirectory(Controllers)
