public class VolatileCallSite extends CallSite {
  public VolatileCallSite(MethodType mt) { throw new RuntimeException(); }
  public VolatileCallSite(MethodHandle mh) { throw new RuntimeException(); }
  public final MethodHandle getTarget() { throw new RuntimeException(); }
  public void setTarget(MethodHandle mh) { throw new RuntimeException(); }
  public final MethodHandle dynamicInvoker() { throw new RuntimeException(); }
}
