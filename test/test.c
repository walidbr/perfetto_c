#include <stdio.h>
#include "perfetto_c.h"
#include <time.h>

void foo(){
	perfetto_c_trace_event("foo"); // Does not work, (no fire diagram)
	printf("foo\n");
}

void bar(){
	perfetto_c_trace_event_begin("bar");
	printf("bar\n"); // Use this one instead
	foo();
	perfetto_c_trace_event_end();
}

int main(){
	perfetto_c_init();
	for(int i = 0 ; i < 10 ; i++){
		perfetto_c_event_instant("main");
		perfetto_c_event_counter("iteration", i);
		foo();
		bar();
        struct timespec ts;
        ts.tv_sec = 1;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
	}
	return 0;
}