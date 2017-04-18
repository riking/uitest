
cmake_minimum_required(VERSION 2.5.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE string "")

project(uitest)

set(LIBS)

#find_package(HIDAPI)

if(NOT APPLE)
	check_lib(PNG libpng png png.h QUIET)
endif()
if (PNG_FOUND)
	message(STATUS "Using shared libpng")
else()
	message(STATUS "Using static libpng from Externals")
	add_subdirectory(Externals/libpng)
	include_directories(Externals/libpng)
	set(PNG png)
endif()

find_library(ICONV_LIBRARIES NAMES iconv libiconv libiconv-2 c)
find_path(ICONV_INClUDE_DIR NAMES iconv.h)
if (ICONV_LIBRARIES AND ICONV_INCLUDE_DIR)
	mark_as_advanced(ICONV_INCLUDE_DIR ICONV_LIBRARIES)
else()
	message(STATUS "Using static iconv from Externals")
	include_directories(Externals/libiconv-1.14/include)
	add_subdirectory(Externals/libiconv-1.14)
	set(ICONV_LIBRARIES iconv)
endif()
list(APPEND LIBS ${ICONV_LIBRARIES})

# wxWidgets
if (1)
	find_package(wxWidgets COMPONENTS core aui adv)
	if (wxWidgets_FOUND)
		EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			COMMAND sh "${wxWidgets_CONFIG_EXECUTABLE}"
			${wxWidgets_CONFIG_OPTIONS} --version
			OUTPUT_VARIABLE wxWidgets_VERSION
			OUTPUT_STRIP_TRAILING_WHITESPACE
			ERROR_QUIET
			)
		message(STATUS "Found wxWidgets version ${wxWidgets_VERSION}")
		set(wxMIN_VERSION "3.1.0")
		if (${wxWidgets_VERSION} VERSION_LESS ${wxMIN_VERSION})
			message(STATUS "Ignoring wxWidgets version ${wxWidgets_VERSION}; at least ${wxMIN_VERSION} is required")
			unset(wxWidgets_FOUND)
		endif()
	endif()
	if (UNIX AND NOT APPLE AND NOT HAIKU)
		find_package(GTK2)
		if (GTK2_FOUND)
			include_directories($GTK2_INCLUDE_DIRS})
			list(APPEND LIBS ${GTK2_LIBRARIES})
		else()
			message(FATAL_ERROR "GTK is required to build the WX UI, please install GTK development libraries")
		endif()
	endif()
	if(wxWidgets_FOUND)
		include(${wxWidgets_USE_FILE})
	else()
		message(STATUS "Using static wxWidgets from Externals")
		if(APPLE)
		elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux|FreeBSD")
			check_lib(GTHREAD2 gthread-2.0 gthread-2.0 glib/gthread.h REQUIRED)
			check_lib(PANGOCAIRO pangocairo pangocairo pango/pangocairo.h REQUIRED)
			find_package(Backtrace REQUIRED)
		elseif(WIN32)
		else()
			message(FATAL_ERROR "vendored wxWidgets is not compatible with your platform")
		endif()

		include_directories(SYSTEM
			Externals/wxWidgets3
			Externals/wxWidgets3/include)
		add_subdirectory(Externals/wxWidgets3)
		set(wxWidgets_FOUND TRUE)
		set(wxWidgets_LIBRARIES "wx")
	endif()
endif()

add_subdirectory(src)
