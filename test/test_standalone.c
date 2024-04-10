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
	perfetto_c_init(PERFETTO_C_IN_PROCESS_BACKEND);
	perfetto_c_start_tracing();
	for(int i = 0 ; i < 4 ; i++){
		perfetto_c_event_instant("main");
		// perfetto_c_register_thread("test_thread1");
		perfetto_c_event_counter("iteration", i);
		perfetto_c_event_counter("iterationx2", i*i);
		perfetto_c_trace_thread_begin("test_thread1", "foobar track");
		perfetto_c_trace_thread_begin("test_thread2", "track test 2");
		foo();
		bar();
		perfetto_c_trace_thread_end("test_thread1");
		perfetto_c_trace_thread_end("test_thread2");
        struct timespec ts;
        ts.tv_sec = 1;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
	}
	perfetto_c_stop_tracing();
	return 0;
}