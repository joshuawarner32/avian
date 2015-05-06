public abstract class CallSite {
  public MethodType type() { throw new RuntimeException(); }
  public abstract MethodHandle getTarget();
  public abstract void setTarget(MethodHandle mh);
  public abstract MethodHandle dynamicInvoker();
}
