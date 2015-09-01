examples = org.eclipse.swt.examples/src
helloworld = org/eclipse/swt/examples/helloworld

.PHONY: build
build: example

.PHONY: example
example:
	make -f app.mk \
		name=example \
		properties-file=$(examples)/examples_helloworld.properties \
		data-files="$(wildcard $(examples)/$(helloworld)/*.png) \
			$(wildcard $(examples)/$(helloworld)/*.gif) \
			$(wildcard $(examples)/$(helloworld)/*.bmp) \
			$(wildcard $(examples)/$(helloworld)/*.html)" \
		data-directory=$(helloworld) \
		sources="$(shell find $(examples)/$(helloworld) -name '*.java')" \
		source-directory=$(examples) \
		main-class=org.eclipse.swt.examples.helloworld.HelloWorld1

.PHONY: clean
clean:
	rm -rf build
