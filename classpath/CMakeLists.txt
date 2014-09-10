
add_definitions(-DUSE_ATOMIC_OPERATIONS)

include_directories(
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_SOURCE_DIR}/src
  $ENV{JAVA_HOME}/include
  ${PLATFORM_JAVA_HOME_INCLUDES}
)

add_library(avian_classpath_native
  java-io.cpp
  java-lang.cpp
  java-net.cpp
  java-nio.cpp
  java-util-zip.cpp
  java-util.cpp
  sockets.cpp
)

add_dependencies(avian_classpath_native generated_type_files)
