cmake_minimum_required(VERSION 3.8)

project(Glitter)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(ExternalProject)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
	string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17 -pthread")
    
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl "${X11_X11_LIB}" "${CMAKE_THREAD_LIBS_INIT}" )
    endif()
endif()

include_directories(Glitter/Sources/
                    Glitter/Vendor/glad/
                    Glitter/Vendor/glm/
                    Glitter/Vendor/stb/
					Glitter/Vendor/imgui/
					Glitter/Vendor/imgui_impl/)
                    
if (MSVC)
    include_directories(Glitter/Sources/Pch)
else()
    include_directories(${CMAKE_BINARY_DIR}/pch)
endif()

file(GLOB VENDORS_SOURCES Glitter/Vendor/imgui/imgui*.cpp
                          Glitter/Vendor/imgui_impl/*.cpp)

file(GLOB_RECURSE PROJECT_SOURCES Glitter/Sources/*.cpp )
file(GLOB_RECURSE PROJECT_HEADERS Glitter/Sources/*.h)

file(GLOB_RECURSE PROJECT_SHADERS Glitter/Shaders/*.comp
                          Glitter/Shaders/*.frag
                          Glitter/Shaders/*.geom
                          Glitter/Shaders/*.vert
						  Glitter/Shaders/*.glsl
						  )

file(GLOB PROJECT_TEXTURES Glitter/Textures/*.jpg
                           Glitter/Textures/*.png)

file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)


if (MSVC)
	#set(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/stdafx.pch")
	#set_source_files_properties(${PROJECT_SOURCES} PROPERTIES COMPILE_FLAGS "/Yustdafx.h /Fp\"${PrecompiledBinary}\""  OBJECT_OUTPUTS "${PrecompiledBinary}")
    #set_source_files_properties(Glitter/Sources/stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h /Fp\"${PrecompiledBinary}\""  OBJECT_OUTPUTS "${PrecompiledBinary}")

    set_source_files_properties(${PROJECT_SOURCES} PROPERTIES COMPILE_FLAGS "/Yu")
    set_source_files_properties(Glitter/Sources/Pch/stdafx.cpp PROPERTIES COMPILE_FLAGS "/Yc")
    set_source_files_properties(Glitter/Sources/impl.cpp PROPERTIES COMPILE_FLAGS "/Y- /Ox")

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	function(MYGL_GROUP_SOURCES)
		cmake_parse_arguments(MY1 "" NAME SRCS ${ARGN})

		# Begin copypaste from SO
		foreach(FILE ${MY1_SRCS}) 
			# Get the directory of the source file
			get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

			# Remove common directory prefix to make the group
			string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/Glitter" "" GROUP "${PARENT_DIR}")

			# Make sure we are using windows slashes
			string(REPLACE "/" "\\" GROUP "${GROUP}")

			source_group("${GROUP}" FILES "${FILE}")
		endforeach()
		# End copypaste
	endfunction(MYGL_GROUP_SOURCES)

	MYGL_GROUP_SOURCES(NAME "Sources" SRCS ${PROJECT_SOURCES} ${PROJECT_HEADERS})
	MYGL_GROUP_SOURCES(NAME "Shaders" SRCS ${PROJECT_SHADERS})

else()
    set(CMAKE_CXX_FLAGS "-Winvalid-pch ${CMAKE_CXX_FLAGS}")
endif()

#source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Textures" FILES ${PROJECT_TEXTURES})
#source_group("Sources" FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(Glitter ${PROJECT_SOURCES} ${PROJECT_HEADERS} 
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS} ${PROJECT_TEXTURES}
                               ${VENDORS_SOURCES})

include(CMake/glfw.cmake)
include(CMake/bullet.cmake)
include(CMake/assimp.cmake)
include(CMake/test.cmake)

target_link_libraries(Glitter ${GLAD_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Glitter")
