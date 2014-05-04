#!/bin/sh

cd $(dirname "${0}")

config="${1}"

if [ -z "${config}" ]; then
  config=$(ls ../../build/ | head -n 1)
fi

if [ -z "${config}" ]; then
  echo "Usage: ${0} <config>"
  echo "ex: ${0} darwin-x86_64"
  exit 1
fi

log() {
  echo $(tput smso)"# ${@}"$(tput sgr0)
}

run() {
  echo "${@}"
  printf "$(tput setaf 2)"
  "${@}" 2>&1
  if [ $? -eq 0 ]; then
    printf "$(tput sgr0)"
  else
    printf "$(tput setaf 1)"
    echo "(failed)"
    printf "$(tput sgr0)"
    exit 1
  fi

}

platform=$(echo ${config} | grep -oE '(linux|windows|darwin|freebsd)')
arch=$(echo ${config} | grep -oE '(i386|x86_64)')

log Populate this directory with the bootstrap classpath jar.
run cp ../../build/${config}/classpath.jar boot.jar
echo

log Build the Java code and add it to the jar.
run javac -bootclasspath boot.jar Hello.java
run jar u0f boot.jar Hello.class
echo

log Make an object file out of the jar.
run ../../build/${config}/binaryToObject/binaryToObject boot.jar \
  boot-jar.o _binary_boot_jar_start _binary_boot_jar_end ${platform} ${arch}
echo

log Compile embedded-jar-main.cpp.
run g++ -I$JAVA_HOME/include -I$JAVA_HOME/include/${platform} -D_JNI_IMPLEMENTATION_ \
  -c embedded-jar-main.cpp -o main.o
echo

log Link the objects produced above to produce the final executable.
run g++ -rdynamic -Wl,-force_load -Wl,../../build/${config}/libavian.a *.o -ldl -lpthread -lz -o hello -framework CoreFoundation
echo

log Optionally strip its symbols.
run strip -S -x hello
echo

log Run generated hello binary
run ./hello
echo
