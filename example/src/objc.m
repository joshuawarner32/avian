#include <AppKit/AppKit.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "jni.h"

#ifdef __MINGW32__
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __attribute__ ((visibility("default")))
#endif

// #include "jni-util.h"

@interface MyApplicationDelegate : NSObject <NSApplicationDelegate,
                                               NSWindowDelegate>
{
  NSWindow* window;
}
@end

@implementation MyApplicationDelegate : NSObject
- (id) init {
  if ((self = [super init])) {
    // allocate and initialize window and stuff here ..
  }
  return self;
}

- (void) applicationWillFinishLaunching: (NSNotification*) notification
{
  [window makeKeyAndOrderFront: self];
}

- (void) dealloc {
  [window release];
  [super dealloc];
}
@end

JNIEXPORT void JNICALL
EXPORT(Java_org_eclipse_swt_examples_helloworld_HelloWorld1_nsAppRun)
(JNIEnv* e, jclass cls)
{
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSApplication* application = [NSApplication sharedApplication];

  // MyApplicationDelegate* appDelegate
  //   = [[[MyApplicationDelegate alloc] init] autorelease];

  // [application setDelegate: appDelegate];
  // [application run];

  [pool drain];
}
