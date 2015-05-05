
class InvokeDynamicTest {
  public static void main(String[] args) throws Exception {

    try {
      Runnable r = () -> System.out.println("test");
      r.run();

      // Technically, this test will fail on openjdk, since it will get here just fine.
      throw new RuntimeException("failed");
    } catch(VirtualMachineError e) {
      // TODO: actually implement invokedynamic
      System.out.println("caught VirtualMachineError!");
    }
  }
}