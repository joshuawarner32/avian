
if (MSVC)
  #todo: support mingw compiler
  add_library(avian_system windows.cpp windows/crash.cpp windows/signal.cpp windows/memory.cpp)
else()
  add_library(avian_system posix.cpp posix/crash.cpp posix/signal.cpp posix/memory.cpp)
endif()
