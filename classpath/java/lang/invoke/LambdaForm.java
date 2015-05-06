class LambdaForm {
  public static final int VOID_RESULT = 0;
  public static final int LAST_RESULT = 0;

  public void prepare() { throw new RuntimeException(); }
  public String toString() { throw new RuntimeException(); }
  public static char basicType(Class<?> c) { throw new RuntimeException(); }
  public static char[] basicTypes(java.util.List<Class<?>> cs) { throw new RuntimeException(); }
  public static String basicTypeSignature(MethodType mt) { throw new RuntimeException(); }
}
