.PHONY: perfetto clean build all test
all: clean build
# Pulls and build perfetto for linux
perfetto:
	git clone https://android.googlesource.com/platform/external/perfetto -b v43.1
	cd perfetto && ./tools/install-build-deps
	cd perfetto && ./tools/gn gen --args='is_debug=false' out/linux
	cd perfetto && ./tools/ninja -C out/linux tracebox traced traced_probes perfetto 

build:
	cmake .
	make -f Makefile

clean:
	rm -rf CmakeCache.txt CMakeFiles CMakeCache.txt Makefile cmake_install.cmake
	rm -rf libperfetto_c.a

test:
	make -C test all

run:
	./test/test_standalone
	perfetto/tools/open_trace_in_ui example.pftrace

trace:
	# pkill traced
	# pkill traced_probes
	# pkill tracebox
	perfetto/out/linux/traced &
	perfetto/out/linux/traced_probes &
	perfetto/out/linux/tracebox perfetto -o sdk_with_system.pftrace --txt -c config/with_track_event.cfg &
	sleep 2
	test/test_c
	test/trace_cpp
	# pkill tracebox

