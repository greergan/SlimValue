BUILD_DIR := build
INSTALL_PREFIX ?= /usr/local
CMAKE := cmake

.PHONY: all configure build install test deb rpm release clean

all: build

configure:
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX)

build: configure
	$(CMAKE) --build $(BUILD_DIR)

install: build
	$(CMAKE) --install $(BUILD_DIR)

test: build
	cd $(BUILD_DIR) && ctest --output-on-failure --verbose
	$(BUILD_DIR)/slim_tests

deb: build
	cd $(BUILD_DIR) && cpack -G DEB

rpm: build
	cd $(BUILD_DIR) && cpack -G RPM

release: deb rpm

clean:
	rm -rf $(BUILD_DIR)