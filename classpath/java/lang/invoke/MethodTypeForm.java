final class MethodTypeForm {
  public static final int NO_CHANGE = 0;
  public static final int ERASE = 0;
  public static final int WRAP = 0;
  public static final int UNWRAP = 0;
  public static final int INTS = 0;
  public static final int LONGS = 0;
  public static final int RAW_RETURN = 0;
  public MethodType erasedType() { throw new RuntimeException(); }
  public MethodType basicType() { throw new RuntimeException(); }
  public LambdaForm cachedLambdaForm(int i) { throw new RuntimeException(); }
  public synchronized LambdaForm setCachedLambdaForm(int i, LambdaForm lf) { throw new RuntimeException(); }
  public MethodHandle basicInvoker() { throw new RuntimeException(); }
  public int parameterCount() { throw new RuntimeException(); }
  public int parameterSlotCount() { throw new RuntimeException(); }
  public int returnCount() { throw new RuntimeException(); }
  public int returnSlotCount() { throw new RuntimeException(); }
  public int primitiveParameterCount() { throw new RuntimeException(); }
  public int longPrimitiveParameterCount() { throw new RuntimeException(); }
  public int primitiveReturnCount() { throw new RuntimeException(); }
  public int longPrimitiveReturnCount() { throw new RuntimeException(); }
  public boolean hasPrimitives() { throw new RuntimeException(); }
  public boolean hasNonVoidPrimitives() { throw new RuntimeException(); }
  public boolean hasLongPrimitives() { throw new RuntimeException(); }
  public int parameterToArgSlot(int i) { throw new RuntimeException(); }
  public int argSlotToParameter(int i) { throw new RuntimeException(); }
  public static MethodType canonicalize(MethodType mt, int i, int i2) { throw new RuntimeException(); }
  public String toString() { throw new RuntimeException(); }
}
