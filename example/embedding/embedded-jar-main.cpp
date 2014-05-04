#include "stdint.h"
#include "jni.h"
#include "stdlib.h"

#if (defined __MINGW32__) || (defined _MSC_VER)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default"))) __attribute__((used))
#endif

#if (!defined __x86_64__) && ((defined __MINGW32__) || (defined _MSC_VER))
#define SYMBOL(x) binary_boot_jar_##x
#else
#define SYMBOL(x) _binary_boot_jar_##x
#endif

extern "C" {

extern const uint8_t SYMBOL(start)[];
extern const uint8_t SYMBOL(end)[];

// This will be called by the VM to get a handle to the embedded jar.
// We tell the VM about this jar by setting the boot classpath to "[bootJar]".
EXPORT const uint8_t* bootJar(unsigned* size)
{
  *size = SYMBOL(end) - SYMBOL(start);
  return SYMBOL(start);
}

}  // extern "C"

extern "C" void __cxa_pure_virtual(void)
{
  abort();
}

int main(int ac, const char** av)
{
  JavaVMInitArgs vmArgs;
  vmArgs.version = JNI_VERSION_1_2;
  vmArgs.nOptions = 1;
  vmArgs.ignoreUnrecognized = JNI_TRUE;

  JavaVMOption options[vmArgs.nOptions];
  vmArgs.options = options;

  options[0].optionString = const_cast<char*>("-Xbootclasspath:[bootJar]");

  JavaVM* vm;
  void* env;
  JNI_CreateJavaVM(&vm, &env, &vmArgs);
  JNIEnv* e = static_cast<JNIEnv*>(env);

  jclass c = e->FindClass("Hello");
  if (not e->ExceptionCheck()) {
    jmethodID m = e->GetStaticMethodID(c, "main", "([Ljava/lang/String;)V");
    if (not e->ExceptionCheck()) {
      jclass stringClass = e->FindClass("java/lang/String");
      if (not e->ExceptionCheck()) {
        jobjectArray a = e->NewObjectArray(ac - 1, stringClass, 0);
        if (not e->ExceptionCheck()) {
          for (int i = 1; i < ac; ++i) {
            e->SetObjectArrayElement(a, i - 1, e->NewStringUTF(av[i]));
          }

          e->CallStaticVoidMethod(c, m, a);
        }
      }
    }
  }

  int exitCode = 0;
  if (e->ExceptionCheck()) {
    exitCode = -1;
    e->ExceptionDescribe();
  }

  vm->DestroyJavaVM();

  return exitCode;
}
