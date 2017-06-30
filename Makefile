MAKE_FLAGS?=-j5

all: clean update
	mkdir -p build;
	cd build; cmake ..;
	cd build; ${MAKE} ${MAKE_FLAGS}

update:
	git submodule update --init;

clean:
	rm -Rf build

.PHONY: clean update
