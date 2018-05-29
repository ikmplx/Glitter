set(GLFW_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install/glfw)
set(GLFW_INCLUDE_DIR ${GLFW_INSTALL_DIR}/include)
set(GLFW_LIB_DIR ${GLFW_INSTALL_DIR}/lib)

ExternalProject_Add(glfw
  DOWNLOAD_COMMAND 	""
  SOURCE_DIR 		${CMAKE_SOURCE_DIR}/Glitter/Vendor/glfw
  CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX:PATH=${GLFW_INSTALL_DIR} -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF
)

include_directories(${GLFW_INCLUDE_DIR})
target_link_libraries(Glitter ${GLFW_LIB_DIR}/glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})

add_dependencies(Glitter glfw)