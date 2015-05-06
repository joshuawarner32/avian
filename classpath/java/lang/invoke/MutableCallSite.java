public class MutableCallSite extends CallSite {
  public MutableCallSite(MethodType mt) { throw new RuntimeException(); }
  public MutableCallSite(MethodHandle mh) { throw new RuntimeException(); }
  public final MethodHandle getTarget() { throw new RuntimeException(); }
  public void setTarget(MethodHandle mh) { throw new RuntimeException(); }
  public final MethodHandle dynamicInvoker() { throw new RuntimeException(); }
  public static void syncAll(MutableCallSite[] mcs) { throw new RuntimeException(); }
}
