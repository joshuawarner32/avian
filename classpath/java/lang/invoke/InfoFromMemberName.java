final class InfoFromMemberName implements MethodHandleInfo {
  public Class<?> getDeclaringClass() { throw new RuntimeException(); }
  public String getName() { throw new RuntimeException(); }
  public MethodType getMethodType() { throw new RuntimeException(); }
  public int getModifiers() { throw new RuntimeException(); }
  public int getReferenceKind() { throw new RuntimeException(); }
  public String toString() { throw new RuntimeException(); }
  public <T extends java.lang.reflect.Member> T reflectAs(java.lang.Class<T> c, MethodHandles.Lookup l) { throw new RuntimeException(); }
}
