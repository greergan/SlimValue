BUILD_DIR := build
INSTALL_PREFIX ?= /usr/local
CMAKE := cmake
DIST_DIR ?= .
LOCAL_SRC ?= ON;

.PHONY: all configure build install test deb rpm packages clean

all: build

configure:
	$(CMAKE) -S . -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) \
		-DCPACK_OUTPUT_FILE_PREFIX=$(DIST_DIR) \
		-DSLIM_USE_LOCAL_SOURCE=$(LOCAL_SRC)

build: configure
	$(CMAKE) --build $(BUILD_DIR)

install: build
	$(CMAKE) --install $(BUILD_DIR)

test: build
	cd $(BUILD_DIR) && ctest --output-on-failure --verbose
	$(BUILD_DIR)/slim_tests

deb: build test
	cd $(BUILD_DIR) && cpack -G DEB

rpm: build test
	cd $(BUILD_DIR) && cpack -G RPM

packages: test
	cd $(BUILD_DIR) && for f in DEB RPM; do cpack -G $$f; done

clean:
	rm -rf $(BUILD_DIR)