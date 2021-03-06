set(ASSIMP_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install/assimp)
set(ASSIMP_INCLUDE_DIR ${ASSIMP_INSTALL_DIR}/include)
set(ASSIMP_LIB_DIR ${ASSIMP_INSTALL_DIR}/lib)

ExternalProject_Add(assimp
  DOWNLOAD_COMMAND 	""
  SOURCE_DIR 		${CMAKE_SOURCE_DIR}/Glitter/Vendor/assimp
  CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX:PATH=${ASSIMP_INSTALL_DIR} -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_SAMPLES=OFF -DASSIMP_BUILD_TESTS=OFF
)

if (MSVC) 
    set(ASSIMP_LIB_LINK assimp-vc140-mt${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
    set(ASSIMP_LIB_LINK libassimp.so)
endif()


include_directories(${ASSIMP_INCLUDE_DIR})
target_link_libraries(Glitter ${ASSIMP_LIB_DIR}/${ASSIMP_LIB_LINK})

add_dependencies(Glitter assimp)

if (MSVC) 
add_custom_command(TARGET Glitter POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different  
		"${ASSIMP_INSTALL_DIR}/bin/assimp-vc140-mt.dll"
		$<TARGET_FILE_DIR:Glitter>)
else()
    add_custom_command(TARGET Glitter POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different  
    		"${ASSIMP_LIB_DIR}/${ASSIMP_LIB_LINK}"
    		$<TARGET_FILE_DIR:Glitter>)
endif()
