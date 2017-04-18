include_directories(include)
include_directories(libcharset/include)

# warnings in dependencies are not our problem
add_definitions(-w)

set(SRCS	lib/iconv.c
		lib/relocatable.c
		libcharset/lib/localcharset.c
)

add_library(iconv STATIC ${SRCS})
