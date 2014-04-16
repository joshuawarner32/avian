#!/bin/bash

if [[ -f /usr/libexec/java_home ]]; then
  /usr/libexec/java_home -v1.7
  exit 0
fi

for p in \
  /usr/local/java/ \
  /usr/lib/jvm/java-7-openjdk-amd64 \
  /usr/lib/jvm/java-7-openjdk-i386 \
  /usr/local/openjdk7
do
  if [[ -f $p/bin/javac ]]; then
    echo $p
    exit 0
  fi
done

for p in "C:/Program Files" "C:/Program Files(x86)"; do
  if [[ -d "$p/Java" ]]; then
    ls -tr "$p/Java/*" | tail -n 1
    exit 0
  fi
fi

echo "Couldn't determine JAVA_HOME" >&2
exit 1