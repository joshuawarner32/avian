package java.lang.invoke;

public class LambdaMetafactory {
  public static CallSite metafactory(MethodHandles.Lookup caller,
                                     String invokedName,
                                     MethodType invokedType,
                                     MethodType samMethodType,
                                     MethodHandle implMethod,
                                     MethodType instantiatedMethodType)
                              throws LambdaConversionException
  {
    throw new RuntimeException();
  }

  public static CallSite altMetafactory(MethodHandles.Lookup caller,
                                        String invokedName,
                                        MethodType invokedType,
                                        Object... args)
                                 throws LambdaConversionException
  {
    throw new RuntimeException();
  }
}