define compile-object
  @echo "compiling $(@)"
  @mkdir -p $(dir $(@))
  $(cxx) $(cflags) -c $$($(windows-path) $(<)) $(call output,$(@))
endef

define compile-asm-object
  @echo "compiling $(@)"
  @mkdir -p $(dir $(@))
  $(as) $(asmflags) $(call asm-output,$(@)) $(call asm-input,$(<))
endef

define compile-unittest-object
  @echo "compiling $(@)"
  @mkdir -p $(dir $(@))
  $(cxx) $(cflags) -c $$($(windows-path) $(<)) -I$(unittest) $(call output,$(@))
endef

$(build)/objects/target/%.cpp.o: %.cpp
	$(compile-object)
