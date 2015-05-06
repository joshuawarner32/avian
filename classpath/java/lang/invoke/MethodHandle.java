package java.lang.invoke;

public abstract class MethodHandle {
  public MethodType type() { throw new RuntimeException(); }
  public final native Object invokeExact(Object... o) throws Throwable;
  public final native Object invoke(Object... o) throws Throwable;
  public Object invokeWithArguments(Object... o) throws Throwable { throw new RuntimeException(); }
  public Object invokeWithArguments(java.util.List<?> l) throws Throwable { throw new RuntimeException(); }
  public MethodHandle asType(MethodType mt) { throw new RuntimeException(); }
  public MethodHandle asSpreader(Class<?> c, int i) { throw new RuntimeException(); }
  public MethodHandle asCollector(Class<?> c, int i) { throw new RuntimeException(); }
  public MethodHandle asVarargsCollector(Class<?> c) { throw new RuntimeException(); }
  public boolean isVarargsCollector() { throw new RuntimeException(); }
  public MethodHandle asFixedArity() { throw new RuntimeException(); }
  public MethodHandle bindTo(Object o) { throw new RuntimeException(); }
  public String toString() { throw new RuntimeException(); }
}
