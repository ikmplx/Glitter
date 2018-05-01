cmake_minimum_required(VERSION 3.8)

project(Glitter)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/glfw)

# Assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/assimp)

# Bullet
option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(Glitter/Vendor/bullet)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
	string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Glitter/Sources/
                    Glitter/Vendor/assimp/include/
                    Glitter/Vendor/glad/include/
                    Glitter/Vendor/glfw/include/
                    Glitter/Vendor/glm/
                    Glitter/Vendor/stb/
                    
                    Glitter/Vendor/bullet/src/

					Glitter/Vendor/imgui/
					Glitter/Vendor/imgui_impl/
					)

file(GLOB VENDORS_SOURCES Glitter/Vendor/glad/src/glad.c 
                          Glitter/Vendor/imgui/imgui*.cpp
                          Glitter/Vendor/imgui_impl/*.cpp
						  )

file(GLOB_RECURSE PROJECT_SOURCES Glitter/Sources/*.cpp )
file(GLOB_RECURSE PROJECT_HEADERS Glitter/Sources/*.h)

file(GLOB PROJECT_SHADERS Glitter/Shaders/*.comp
                          Glitter/Shaders/*.frag
                          Glitter/Shaders/*.geom
                          Glitter/Shaders/*.vert)

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
    set_source_files_properties(Glitter/Sources/stdafx.cpp PROPERTIES COMPILE_FLAGS "/Yc")
    set_source_files_properties(Glitter/Sources/stb.cpp PROPERTIES COMPILE_FLAGS "/Y- /Ox")

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	# Begin copypaste from SO
	foreach(FILE ${PROJECT_SOURCES} ${PROJECT_HEADERS}) 
		# Get the directory of the source file
		get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

		# Remove common directory prefix to make the group
		string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

		# Make sure we are using windows slashes
		string(REPLACE "/" "\\" GROUP "${GROUP}")

		source_group("Sources${GROUP}" FILES "${FILE}")
	endforeach()
	# End copypaste

endif()

source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Textures" FILES ${PROJECT_TEXTURES})
#source_group("Sources" FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} 
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS} ${PROJECT_TEXTURES}
                               ${VENDORS_SOURCES})

target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath
                      )

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Glitter")
