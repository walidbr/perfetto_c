#include <iostream>
#include "perfetto_c.h"
#include <time.h>

void bar(){
	perfetto_c_trace_event_begin("bar");
	printf("bar\n");
	perfetto_c_trace_event_end();
}

void foo(){
	perfetto_c_trace_event("foo");
	printf("foo\n");
	bar();
	bar();
	bar();
	bar();
}

int main(){
	perfetto_c_init(PERFETTO_C_SYSTEAM_BACKEND);
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