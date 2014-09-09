if(APPLE)
   include_directories( /Developer/Headers/FlatCarbon )
   find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)

   mark_as_advanced(CORE_FOUNDATION_LIBRARY)

   set(PLATFORM_LIBS ${CORE_FOUNDATION_LIBRARY})
endif()

if(MSVC)
	set(PLATFORM_CXX_FLAGS "/Wall")
else()
	set(PLATFORM_CXX_FLAGS "-Wall -Werror -fno-exceptions -std=c++0x")
  set(PLATFORM_LIBS ${PLATFORM_LIBS} pthread dl)
endif()

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

if(WIN32)
  set(PLATFORM_GRADLE_COMMAND "${CMAKE_SOURCE_DIR}/gradlew.bat")
else()
  set(PLATFORM_GRADLE_COMMAND "${CMAKE_SOURCE_DIR}/gradlew")
endif()