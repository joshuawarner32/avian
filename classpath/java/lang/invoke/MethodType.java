package java.lang.invoke;

public final class MethodType implements java.io.Serializable {
  public static MethodType methodType(Class<?> c, Class<?>[] cs) { throw new RuntimeException(); }
  public static MethodType methodType(Class<?> c, java.util.List<Class<?>> cs) { throw new RuntimeException(); }
  public static MethodType methodType(Class<?> c, Class<?> c2, Class<?>... cs) { throw new RuntimeException(); }
  public static MethodType methodType(Class<?> c) { throw new RuntimeException(); }
  public static MethodType methodType(Class<?> c, Class<?> c2) { throw new RuntimeException(); }
  public static MethodType methodType(Class<?> c, MethodType mt) { throw new RuntimeException(); }
  public static MethodType genericMethodType(int i, boolean b) { throw new RuntimeException(); }
  public static MethodType genericMethodType(int i) { throw new RuntimeException(); }
  public MethodType changeParameterType(int i, Class<?> c) { throw new RuntimeException(); }
  public MethodType insertParameterTypes(int i, Class<?>... c) { throw new RuntimeException(); }
  public MethodType appendParameterTypes(Class<?>... c) { throw new RuntimeException(); }
  public MethodType insertParameterTypes(int i, java.util.List<Class<?>> c) { throw new RuntimeException(); }
  public MethodType appendParameterTypes(java.util.List<Class<?>> c) { throw new RuntimeException(); }
  public MethodType dropParameterTypes(int i, int i2) { throw new RuntimeException(); }
  public MethodType changeReturnType(Class<?> c) { throw new RuntimeException(); }
  public boolean hasPrimitives() { throw new RuntimeException(); }
  public boolean hasWrappers() { throw new RuntimeException(); }
  public MethodType erase() { throw new RuntimeException(); }
  public MethodType generic() { throw new RuntimeException(); }
  public MethodType wrap() { throw new RuntimeException(); }
  public MethodType unwrap() { throw new RuntimeException(); }
  public Class<?> parameterType(int i) { throw new RuntimeException(); }
  public int parameterCount() { throw new RuntimeException(); }
  public Class<?> returnType() { throw new RuntimeException(); }
  public java.util.List<Class<?>> parameterList() { throw new RuntimeException(); }
  public Class<?>[] parameterArray() { throw new RuntimeException(); }
  public boolean equals(Object o) { throw new RuntimeException(); }
  public int hashCode() { throw new RuntimeException(); }
  public String toString() { throw new RuntimeException(); }
  public static MethodType fromMethodDescriptorString(String s, java.lang.ClassLoader cl) throws java.lang.IllegalArgumentException, java.lang.TypeNotPresentException { throw new RuntimeException(); }
  public String toMethodDescriptorString() { throw new RuntimeException(); }
}
