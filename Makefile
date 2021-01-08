ve2: build/bin/ve2
	./build/bin/ve2

build/bin/ve2:  | build
	cd build && conan install ..
	cd build && cmake .. -DCMAKE_BUILD_TYPE=Release
	cd build && cmake --build .

build:
	mkdir -p $@

clean:
	rm -rf build

.PHONY: ve2 clean
