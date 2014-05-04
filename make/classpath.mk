
ifneq ($(openjdk),)
  openjdk-arch = $(arch)
  ifeq ($(arch),x86_64)
    openjdk-arch = amd64
  endif

  ifneq ($(openjdk-src),)
    include openjdk-src.mk
    options := $(options)-openjdk-src
    classpath-objects = $(openjdk-objects) $(openjdk-local-objects)
    classpath-cflags = -DAVIAN_OPENJDK_SRC -DBOOT_JAVAHOME
    openjdk-jar-dep = $(build)/openjdk-jar.dep
    classpath-jar-dep = $(openjdk-jar-dep)
    javahome = $(embed-prefix)/javahomeJar
    javahome-files = lib/currency.data lib/security/java.security \
      lib/security/java.policy lib/security/cacerts

    ifneq (,$(wildcard $(openjdk)/jre/lib/zi))
      javahome-files += lib/zi
    endif

    ifneq (,$(wildcard $(openjdk)/jre/lib/tzdb.dat))
      javahome-files += lib/tzdb.dat
    endif

    local-policy = lib/security/local_policy.jar
    ifneq (,$(wildcard $(openjdk)/jre/$(local-policy)))
      javahome-files += $(local-policy)
    endif

    export-policy = lib/security/US_export_policy.jar
    ifneq (,$(wildcard $(openjdk)/jre/$(export-policy)))
      javahome-files += $(export-policy)
    endif

    ifeq ($(platform),windows)
      javahome-files += lib/tzmappings
    endif
    javahome-object = $(build)/javahome-jar.o
    boot-javahome-object = $(build)/boot-javahome.o
    stub-sources = $(src)/openjdk/stubs.cpp
    stub-objects = $(call cpp-objects,$(stub-sources),$(src),$(build))
  else
    soname-flag = -Wl,-soname -Wl,$(so-prefix)jvm$(so-suffix)
    version-script-flag = -Wl,--version-script=openjdk.ld
    options := $(options)-openjdk
    test-executable = $(shell pwd)/$(executable-dynamic)
    ifeq ($(build-platform),darwin)
      library-path = \
        $(library-path-variable)=$(build):$(openjdk)/jre/lib
    else
      library-path = \
        $(library-path-variable)=$(build):$(openjdk)/jre/lib/$(openjdk-arch)
    endif
    javahome = "$$($(native-path) "$(openjdk)/jre")"
  endif

  classpath = openjdk
  boot-classpath := "$(boot-classpath)$(path-separator)$$($(native-path) "$(openjdk)/jre/lib/rt.jar")"
  build-javahome = $(openjdk)/jre
endif

ifneq ($(android),)
  options := $(options)-android
  classpath-jar-dep = $(build)/android.dep
  luni-native = $(android)/libcore/luni/src/main/native
  classpath-cflags = -DBOOT_JAVAHOME
  android-cflags = -I$(luni-native) \
    -I$(android)/libnativehelper/include/nativehelper \
    -I$(android)/system/core/include \
    -I$(android)/external/zlib \
    -I$(android)/external/icu4c/i18n \
    -I$(android)/external/icu4c/common \
    -I$(android)/external/expat \
    -I$(android)/external/openssl/include \
    -I$(android)/libcore/include \
    -I$(build)/android-src/external/fdlibm \
    -I$(build)/android-src \
    -fno-exceptions \
    -D_FILE_OFFSET_BITS=64 \
    -DOS_SHARED_LIB_FORMAT_STR="\"$(so-prefix)%s$(so-suffix)\"" \
    -DJNI_JARJAR_PREFIX= \
    -D__DARWIN_UNIX03=1 \
    -D__PROVIDE_FIXMES \
    -g3 \
    -Werror

  luni-cpps := $(shell find $(luni-native) -name '*.cpp')

  libnativehelper-native := $(android)/libnativehelper
  libnativehelper-cpps := $(libnativehelper-native)/JniConstants.cpp \
    $(libnativehelper-native)/toStringArray.cpp

  crypto-native := $(android)/libcore/crypto/src/main/native

  crypto-cpps := $(crypto-native)/org_conscrypt_NativeCrypto.cpp

  ifeq ($(platform),windows)
    android-cflags += -D__STDC_CONSTANT_MACROS
    blacklist = $(luni-native)/java_io_Console.cpp \
      $(luni-native)/java_lang_ProcessManager.cpp \
      $(luni-native)/libcore_net_RawSocket.cpp

    luni-cpps := $(filter-out $(blacklist),$(luni-cpps))
    icu-libs := $(android)/external/icu4c/lib/sicuin.a \
      $(android)/external/icu4c/lib/sicuuc.a \
      $(android)/external/icu4c/lib/sicudt.a
    platform-lflags := -lgdi32
  else
    android-cflags += -fPIC -DHAVE_SYS_UIO_H
    icu-libs := $(android)/external/icu4c/lib/libicui18n.a \
      $(android)/external/icu4c/lib/libicuuc.a \
      $(android)/external/icu4c/lib/libicudata.a
  endif

  classpath-lflags := \
    $(icu-libs) \
    $(android)/external/fdlibm/libfdm.a \
    $(android)/external/expat/.libs/libexpat.a \
    $(android)/openssl-upstream/libssl.a \
    $(android)/openssl-upstream/libcrypto.a \
    $(platform-lflags) \
    -lstdc++

  ifeq ($(platform),linux)
    classpath-lflags += -lrt
  endif

  classpath-objects = \
    $(call cpp-objects,$(luni-cpps),$(luni-native),$(build)) \
    $(call cpp-objects,$(crypto-cpps),$(crypto-native),$(build)) \
    $(call cpp-objects,$(libnativehelper-cpps),$(libnativehelper-native),$(build))
  luni-java = $(android)/libcore/luni/src/main/java
  luni-javas := $(shell find $(luni-java) -name '*.java')
  luni-nonjavas := $(shell find $(luni-java) -not -type d -not -name '*.java')
  luni-copied-nonjavas = $(call noop-files,$(luni-nonjavas),$(luni-java),)
  libdvm-java = $(android)/libcore/libdvm/src/main/java
  libdvm-javas := $(shell find $(libdvm-java) -name '*.java')
  crypto-java = $(android)/libcore/crypto/src/main/java
  crypto-javas := $(shell find $(crypto-java) -name '*.java')
  dalvik-java = $(android)/libcore/dalvik/src/main/java
  dalvik-javas := $(shell find $(dalvik-java) -name '*.java')
  xml-java = $(android)/libcore/xml/src/main/java
  xml-javas := $(shell find $(xml-java) -name '*.java')
  android-classes = \
    $(call java-classes,$(luni-javas),$(luni-java),$(build)/android) \
    $(call java-classes,$(libdvm-javas),$(libdvm-java),$(build)/android) \
    $(call java-classes,$(crypto-javas),$(crypto-java),$(build)/android) \
    $(call java-classes,$(dalvik-javas),$(dalvik-java),$(build)/android) \
    $(call java-classes,$(xml-javas),$(xml-java),$(build)/android)
  classpath = android

  javahome-files = tzdata
  javahome-object = $(build)/javahome-jar.o
  boot-javahome-object = $(build)/boot-javahome.o
  build-javahome = $(android)/bionic/libc/zoneinfo
  stub-sources = $(src)/android/stubs.cpp
  stub-objects = $(call cpp-objects,$(stub-sources),$(src),$(build))
endif

ifeq ($(classpath),avian)
  jni-sources := $(shell find $(classpath-src) -name '*.cpp')
  jni-objects = $(call target-objects,$(jni-sources))
  classpath-objects = $(jni-objects)
endif

ifneq ($(classpath),avian)
# Assembler, ConstantPool, and Stream are not technically needed for a
# working build, but we include them since our Subroutine test uses
# them to synthesize a class:
  classpath-sources := \
    $(classpath-src)/avian/Addendum.java \
    $(classpath-src)/avian/AnnotationInvocationHandler.java \
    $(classpath-src)/avian/Assembler.java \
    $(classpath-src)/avian/Callback.java \
    $(classpath-src)/avian/Cell.java \
    $(classpath-src)/avian/ClassAddendum.java \
    $(classpath-src)/avian/InnerClassReference.java \
    $(classpath-src)/avian/Classes.java \
    $(classpath-src)/avian/ConstantPool.java \
    $(classpath-src)/avian/Continuations.java \
    $(classpath-src)/avian/FieldAddendum.java \
    $(classpath-src)/avian/Function.java \
    $(classpath-src)/avian/IncompatibleContinuationException.java \
    $(classpath-src)/avian/Machine.java \
    $(classpath-src)/avian/MethodAddendum.java \
    $(classpath-src)/avian/Singleton.java \
    $(classpath-src)/avian/Stream.java \
    $(classpath-src)/avian/SystemClassLoader.java \
    $(classpath-src)/avian/Traces.java \
    $(classpath-src)/avian/VMClass.java \
    $(classpath-src)/avian/VMField.java \
    $(classpath-src)/avian/VMMethod.java \
    $(classpath-src)/avian/avianvmresource/Handler.java

  ifeq ($(openjdk),)
    classpath-sources := $(classpath-sources) \
      $(classpath-src)/sun/reflect/ConstantPool.java \
      $(classpath-src)/java/lang/ReflectiveOperationException.java \
      $(classpath-src)/java/net/ProtocolFamily.java \
      $(classpath-src)/java/net/StandardProtocolFamily.java \
      $(classpath-src)/sun/misc/Cleaner.java \
      $(classpath-src)/sun/misc/Unsafe.java \
      $(classpath-src)/java/lang/reflect/Proxy.java
  endif
else
  classpath-sources := $(shell find $(classpath-src) -name '*.java')
endif