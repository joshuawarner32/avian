package java.lang.invoke;

public class ConstantCallSite extends CallSite {
  public ConstantCallSite(MethodHandle mh) { throw new RuntimeException(); }
  public final MethodHandle getTarget() { throw new RuntimeException(); }
  public final void setTarget(MethodHandle mh) { throw new RuntimeException(); }
  public final MethodHandle dynamicInvoker() { throw new RuntimeException(); }
}
