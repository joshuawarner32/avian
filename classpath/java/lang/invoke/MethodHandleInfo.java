package java.lang.invoke;

public interface MethodHandleInfo {
  public static final int REF_getField = 0;
  public static final int REF_getStatic = 0;
  public static final int REF_putField = 0;
  public static final int REF_putStatic = 0;
  public static final int REF_invokeVirtual = 0;
  public static final int REF_invokeStatic = 0;
  public static final int REF_invokeSpecial = 0;
  public static final int REF_newInvokeSpecial = 0;
  public static final int REF_invokeInterface = 0;
  public abstract int getReferenceKind();
  public abstract Class<?> getDeclaringClass();
  public abstract String getName();
  public abstract MethodType getMethodType();
  public abstract <T extends java.lang.reflect.Member> T reflectAs(java.lang.Class<T> c, MethodHandles.Lookup l);
  public abstract int getModifiers();

  // default methods are not supported in -source 1.6
  // public default boolean isVarArgs() { throw new RuntimeException(); }

  // static interface methods are not supported in -source 1.6
  // public static String referenceKindToString(int i) { throw new RuntimeException(); }
  // public static String toString(int i, Class<?> c, String s, MethodType mt) { throw new RuntimeException(); }
}
