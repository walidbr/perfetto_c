#include <stdio.h>
#include <unistd.h>
#include "trace_json.h"

int foo() {
	trace_json_marker("fooevent", 0);
	printf("foo\n");
	return 0;
}

int bar(int a) {
	trace_json_marker("barevnet", 0);
	printf("bar\n");
	foo();
	return a;
}


int main(void) {

	trace_json_init("trace.json", TRACE_JSON_SINGLE_FILE, 100);
	trace_json_start();
	for(int i = 0 ; i < 4; i++){
		trace_json_begin("Hello, World!", 0);
		printf("Hello, World!\n");
		trace_json_counter("counter", 0, i);
		int a = 0;
		TRACE_WRAP(bar(a));
		TRACE_WRAP(foo());
		trace_json_end("Hello, World!", 0);
		// sleep for 1sec
		usleep(1000000);
	}

	trace_json_stop();
    return 0;
}