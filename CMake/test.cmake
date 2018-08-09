set(GTEST_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install/gtest)

ExternalProject_Add(gtest
  DOWNLOAD_COMMAND 	""
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/Glitter/Vendor/googletest
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX:PATH=${GTEST_INSTALL_DIR} -Dgtest_force_shared_crt=ON
)

add_dependencies(Glitter gtest)

include_directories(${GTEST_INSTALL_DIR}/include)
target_link_libraries(Glitter ${GTEST_INSTALL_DIR}/lib/gtestd${CMAKE_STATIC_LIBRARY_SUFFIX})

enable_testing()
add_test(NAME GlitterTest COMMAND Glitter --gtest)
