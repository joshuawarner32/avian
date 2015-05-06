public class SwitchPoint {
  public SwitchPoint() { throw new RuntimeException(); }
  public boolean hasBeenInvalidated() { throw new RuntimeException(); }
  public MethodHandle guardWithTest(MethodHandle mh, MethodHandle mh2) { throw new RuntimeException(); }
  public static void invalidateAll(SwitchPoint[] sps) { throw new RuntimeException(); }
}
