add_subdirectory(codegen)
add_subdirectory(system)
add_subdirectory(heap)
add_subdirectory(util)
add_subdirectory(tools)

add_library(avian_jvm_finder finder.cpp)

set(avian_generated_type_files)

foreach(output enums declarations constructors initializations java-initializations name-initializations)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/type-${output}.cpp
    COMMAND type_generator -cp ${classpath_jar} -i ${CMAKE_CURRENT_SOURCE_DIR}/types.def -o ${CMAKE_CURRENT_BINARY_DIR}/type-${output}.cpp -t ${output}
    DEPENDS type_generator ${classpath_jar} ${CMAKE_CURRENT_SOURCE_DIR}/types.def
  )
  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/type-${output}.cpp
    PROPERTIES GENERATED TRUE
  )
  list(APPEND avian_generated_type_files ${CMAKE_CURRENT_BINARY_DIR}/type-${output}.cpp)
endforeach()

add_custom_target(
  generated_type_files ALL
  DEPENDS ${avian_generated_type_files})

add_definitions(
  -DUSE_ATOMIC_OPERATIONS
  -DBOOT_CLASSPATH=\"[classpathJar]\"
  -DAVIAN_CLASSPATH=\"[classpathJar]\"
  -DAVIAN_VERSION=\"${avian_version}\"
  -DAVIAN_JAVA_HOME=\"BOGUS/JAVA/HOME/PATH\" # TODO: use the _real_ java home value
  -DAVIAN_EMBED_PREFIX=\"BOGUS/EMBED/PREFIX\" # TODO: use the _real_ embed prefix (what is this for?)
)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  $ENV{JAVA_HOME}/include
  ${PLATFORM_JAVA_HOME_INCLUDES}
)

enable_language(ASM)

set(classpath_object ${CMAKE_CURRENT_BINARY_DIR}/classpath.jar.o)

add_custom_command(
  OUTPUT ${classpath_object}
  COMMAND binary_to_object ${classpath_jar} ${classpath_object} _binary_classpath_jar_start _binary_classpath_jar_end ${AVIAN_FORMAT_NAME} ${AVIAN_ARCH_NAME}
)

set_source_files_properties(
    ${classpath_object} PROPERTIES
    EXTERNAL_OBJECT TRUE
    GENERATED TRUE
)

add_library(avian_jvm
  compile.cpp
  debug-util.cpp
  machine.cpp
  process.cpp
  util.cpp
  builtin.cpp
  classpath-avian.cpp
  jnienv.cpp
  compile-x86.S
  boot.cpp
  x86.S
  ${classpath_object}
)

add_dependencies(avian_jvm generated_type_files)

add_executable(avian main.cpp)

if(APPLE)
  set(WHOLE_avian_classpath_native -force_load avian_classpath_native)
elseif(MSVC)
  # TODO: not sure what linker options to use on MSVC to ensure the entire archive is linked...
  set(WHOLE_avian_classpath_native avian_classpath_native)
else()
  # assume gnu linker
  set(WHOLE_avian_classpath_native -Wl,--whole-archive avian_classpath_native -Wl,--no-whole-archive)
endif()

target_link_libraries(avian
  avian_jvm
  avian_heap
  avian_jvm_finder
  avian_codegen
  avian_codegen_x86
  avian_system
  avian_util
  ${WHOLE_avian_classpath_native}
  ${ZLIB_LIBRARIES}
  ${PLATFORM_LIBS}
)
