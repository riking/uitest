set(SRCS
	uihello.cpp
)

set(LIBS ${LIBS} png)

set(WXLIBS ${wxWidgets_LIBRARIES})
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(WXLIBS ${WXLIBS} dl)
endif()

add_executable(uitest ${SRCS})
target_link_libraries(uitest ${LIBS} ${WXLIBS})
