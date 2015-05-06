package java.lang.invoke;

public class MethodHandles {
  public static MethodHandles.Lookup lookup() { throw new RuntimeException(); }
  public static MethodHandles.Lookup publicLookup() { throw new RuntimeException(); }
  public static <T extends java.lang.reflect.Member> T reflectAs(java.lang.Class<T> c, MethodHandle mh) { throw new RuntimeException(); }
  public static MethodHandle arrayElementGetter(Class<?> c) throws java.lang.IllegalArgumentException { throw new RuntimeException(); }
  public static MethodHandle arrayElementSetter(Class<?> c) throws java.lang.IllegalArgumentException { throw new RuntimeException(); }
  public static MethodHandle spreadInvoker(MethodType mt, int i) { throw new RuntimeException(); }
  public static MethodHandle exactInvoker(MethodType mt) { throw new RuntimeException(); }
  public static MethodHandle invoker(MethodType mt) { throw new RuntimeException(); }
  public static MethodHandle explicitCastArguments(MethodHandle mh, MethodType mt) { throw new RuntimeException(); }
  public static MethodHandle permuteArguments(MethodHandle mh, MethodType mt, int... i) { throw new RuntimeException(); }
  public static MethodHandle constant(Class<?> c, Object o) { throw new RuntimeException(); }
  public static MethodHandle identity(Class<?> c) { throw new RuntimeException(); }
  public static MethodHandle insertArguments(MethodHandle mh, int i, Object... o) { throw new RuntimeException(); }
  public static MethodHandle dropArguments(MethodHandle mh, int i, java.util.List<Class<?>> cs) { throw new RuntimeException(); }
  public static MethodHandle dropArguments(MethodHandle mh, int i, Class<?>... c) { throw new RuntimeException(); }
  public static MethodHandle filterArguments(MethodHandle mh, int i, MethodHandle... mhs) { throw new RuntimeException(); }
  public static MethodHandle collectArguments(MethodHandle mh, int i, MethodHandle mh2) { throw new RuntimeException(); }
  public static MethodHandle filterReturnValue(MethodHandle mha, MethodHandle mhb) { throw new RuntimeException(); }
  public static MethodHandle foldArguments(MethodHandle mha, MethodHandle mhb) { throw new RuntimeException(); }
  public static MethodHandle guardWithTest(MethodHandle mh, MethodHandle mha, MethodHandle mhb) { throw new RuntimeException(); }
  public static MethodHandle catchException(MethodHandle mha, java.lang.Class<? extends Throwable> ct, MethodHandle mhb) { throw new RuntimeException(); }
  public static MethodHandle throwException(Class<?> c, java.lang.Class<? extends Throwable> ct) { throw new RuntimeException(); }

  public final class Lookup {
    public static final int PUBLIC = 0;
    public static final int PRIVATE = 0;
    public static final int PROTECTED = 0;
    public static final int PACKAGE = 0;
    public Class<?> lookupClass() { throw new RuntimeException(); }
    public int lookupModes() { throw new RuntimeException(); }
    public Lookup in(Class<?> c) { throw new RuntimeException(); }
    public String toString() { throw new RuntimeException(); }
    public MethodHandle findStatic(Class<?> c, String s, MethodType mt) throws java.lang.NoSuchMethodException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle findVirtual(Class<?> c, String s, MethodType mt) throws java.lang.NoSuchMethodException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle findConstructor(Class<?> c, MethodType mt) throws java.lang.NoSuchMethodException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle findSpecial(Class<?> c, String s, MethodType mt, Class<?> c2) throws java.lang.NoSuchMethodException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle findGetter(Class<?> c, String s, Class<?> c2) throws java.lang.NoSuchFieldException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle findSetter(Class<?> c, String s, Class<?> c2) throws java.lang.NoSuchFieldException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle findStaticGetter(Class<?> c, String s, Class<?> c2) throws java.lang.NoSuchFieldException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle findStaticSetter(Class<?> c, String s, Class<?> c2) throws java.lang.NoSuchFieldException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle bind(Object o, String s, MethodType mt) throws java.lang.NoSuchMethodException, java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle unreflect(java.lang.reflect.Method m) throws java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle unreflectSpecial(java.lang.reflect.Method m, Class<?> c) throws java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle unreflectConstructor(java.lang.reflect.Constructor<?> c) throws java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle unreflectGetter(java.lang.reflect.Field f) throws java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandle unreflectSetter(java.lang.reflect.Field f) throws java.lang.IllegalAccessException { throw new RuntimeException(); }
    public MethodHandleInfo revealDirect(MethodHandle mh) { throw new RuntimeException(); }
  }

}
