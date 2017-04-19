
cmake_minimum_required(VERSION 2.5.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE string "")

project(uitest)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif()

set(LIBS)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/CMake
)
include(CheckAndAddFlag)
include(CheckCCompilerFlag)
include(CheckLib)

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
	if(NOT OSX_USE_DEFAULT_SEARCH_PATH)
		list(APPEND CMAKE_PREFIX_PATH "/usr")
	endif()

	check_and_add_flag(HAVE_MSSSE3 -mssse3)
	check_and_add_flag(HAVE_ARCH_CORE2 -march=core2)

	# Linker flags
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-dead_strip,-dead_strip_dylibs")

	find_library(APPKIT_LIBRARY AppKit)
	find_library(APPSERV_LIBRARY ApplicationServices)
	find_library(CARBON_LIBRARY Carbon)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(COREFUND_LIBRARY CoreFoundation)
	find_library(CORESERV_LIBRARY CoreServices)
	find_library(FOUNDATION_LIBRARY Foundation)
	find_library(IOB_LIBRARY IOBluetooth)
	find_library(IOK_LIBRARY IOKit)
	find_library(FORCEFEEDBACK ForceFeedback)

	list(APPEND LIBS
		${APPKIT_LIBRARY}
		${COREFUND_LIBRARY}
		${CORESERV_LIBRARY}
		${IOK_LIBRARY}
		${FORCEFEEDBACK}
	)
endif()

#find_package(HIDAPI)

find_package(ZLIB)
if(ZLIB_FOUND)
	message(STATUS "Using shared zlib")
	include_directories(${ZLIB_INCLUDE_DIRS})
else()
	message(STATUS "Shared zlib not found, using static zlib from Externals")
	message(FATAL_ERROR "do I really need to do this")
endif()

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

find_package(X11)

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
		set(wxMIN_VERSION "3.0.0")
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
