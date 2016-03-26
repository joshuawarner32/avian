/* Copyright (c) 2008-2015, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */


#include <avian/arch.h>
#include <avian/append.h>

#include <avian/system/system.h>
#include <avian/system/signal.h>
#include <avian/system/memory.h>
#include <avian/util/math.h>

#define ACQUIRE(x) MutexResource MAKE_NAME(mutexResource_)(x)

using namespace vm;
using namespace avian::util;

namespace {

class MutexResource {
 public:
 //  MutexResource(pthread_mutex_t& m) : m(&m)
 //  {
 //    // pthread_mutex_lock(&m);
 //  }
 //
 //  ~MutexResource()
 //  {
 //    // pthread_mutex_unlock(m);
 //  }
 //
 // private:
 //  // pthread_mutex_t* m;
};

class MySystem;
MySystem* globalSystem;

void* run(void* r)
{
  static_cast<System::Runnable*>(r)->run();
  return 0;
}

// const bool Verbose = false;

const unsigned Notified = 1 << 0;

class MySystem : public System {
 public:
  class Thread : public System::Thread {
   public:
    virtual void interrupt()
    {
      ::abort();
    }

    virtual bool getAndClearInterrupted()
    {
      ::abort();
    }

    virtual void join()
    {
      ::abort();
    }

    virtual void dispose()
    {
      ::abort();
    }
  };

  class Mutex : public System::Mutex {
   public:
    virtual void acquire()
    {
      ::abort();
    }

    virtual void release()
    {
      ::abort();
    }

    virtual void dispose()
    {
      ::abort();
    }
  };

  class Monitor : public System::Monitor {
   public:
    virtual bool tryAcquire(System::Thread* context UNUSED)
    {
      ::abort();
    }

    virtual void acquire(System::Thread* context UNUSED)
    {
      ::abort();
    }

    virtual void release(System::Thread* context UNUSED)
    {
      ::abort();
    }

    virtual void wait(System::Thread* context UNUSED, int64_t time UNUSED)
    {
      ::abort();
    }

    virtual bool waitAndClearInterrupted(System::Thread* context, int64_t time UNUSED)
    {
      ::abort();
    }

    virtual void notify(System::Thread* context UNUSED)
    {
      ::abort();
    }

    virtual void notifyAll(System::Thread* context UNUSED)
    {
      ::abort();
    }

    virtual System::Thread* owner()
    {
      ::abort();
    }

    virtual void dispose()
    {
      ::abort();
    }
  };

  class Local : public System::Local {
   public:
    virtual void* get()
    {
      ::abort();
    }

    virtual void set(void* p)
    {
      ::abort();
    }

    virtual void dispose()
    {
      ::abort();
    }
  };

  class Region : public System::Region {
   public:
    virtual const uint8_t* start()
    {
      ::abort();
    }

    virtual size_t length()
    {
      ::abort();
    }

    virtual void dispose()
    {
      ::abort();
    }
  };

  class Directory : public System::Directory {
   public:
    virtual const char* next()
    {
      ::abort();
    }

    virtual void dispose()
    {
      ::abort();
    }
  };

  class Library : public System::Library {
   public:
    virtual void* resolve(const char* function)
    {
      ::abort();
    }

    virtual const char* name()
    {
      ::abort();
    }

    virtual System::Library* next()
    {
      ::abort();
    }

    virtual void setNext(System::Library* lib)
    {
      ::abort();
    }

    virtual void disposeAll()
    {
      ::abort();
    }
  };

  MySystem(bool reentrant) : reentrant(reentrant)
  {
  }

  virtual void* tryAllocate(size_t sizeInBytes)
  {
    return malloc(sizeInBytes);
  }

  virtual void free(const void* p)
  {
    if (p)
      ::free(const_cast<void*>(p));
  }

  virtual bool success(Status s) {
    return s == 0;
  }

  virtual Status attach(Runnable* r)
  {
    Thread* t = new (allocate(this, sizeof(Thread))) Thread();
    // t->thread = pthread_self();
    r->attach(t);
    return 0;
  }

  virtual Status start(Runnable* r)
  {
    Thread* t = new (allocate(this, sizeof(Thread))) Thread();
    r->attach(t);
    // int rv UNUSED = pthread_create(&(t->thread), 0, run, r);
    // expect(this, rv == 0);
    return 0;
  }

  virtual Status make(System::Mutex** m)
  {
    *m = new (allocate(this, sizeof(Mutex))) Mutex();
    return 0;
  }

  virtual Status make(System::Monitor** m)
  {
    *m = new (allocate(this, sizeof(Monitor))) Monitor();
    return 0;
  }

  virtual Status make(System::Local** l)
  {
    *l = new (allocate(this, sizeof(Local))) Local();
    return 0;
  }

  virtual Status visit(System::Thread* st UNUSED,
                       System::Thread* sTarget,
                       ThreadVisitor* visitor)
  {
    ::abort();
  }

  virtual Status map(System::Region** region, const char* name)
  {
    ::abort();
  }

  virtual Status open(System::Directory** directory, const char* name)
  {
    ::abort();
  }

  virtual FileType stat(const char* name, size_t* length)
  {
    ::abort();
  }

  virtual const char* libraryPrefix()
  {
    ::abort();
  }

  virtual const char* librarySuffix()
  {
    ::abort();
  }

  virtual const char* toAbsolutePath(AllocOnly* allocator, const char* name)
  {
    ::abort();
  }

  virtual Status load(System::Library** lib, const char* name)
  {
    ::abort();
  }

  virtual char pathSeparator()
  {
    return ':';
  }

  virtual char fileSeparator()
  {
    return '/';
  }

  virtual int64_t now()
  {
    ::abort();
  }

  virtual void yield()
  {
    ::abort();
  }

  virtual void exit(int code)
  {
    ::exit(code);
  }

  virtual void abort()
  {
    ::abort();
  }

  virtual void dispose()
  {
    ::free(this);
  }

  bool reentrant;
};

}  // namespace

namespace vm {

AVIAN_EXPORT System* makeSystem(bool reentrant)
{
  return new (malloc(sizeof(MySystem))) MySystem(reentrant);
}

}  // namespace vm
