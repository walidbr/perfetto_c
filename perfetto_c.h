#ifndef __PERFETTO_C_H__
#define __PERFETTO_C_H__
#ifdef __cplusplus
extern "C" {
#endif

    // args.backends |= perfetto::kInProcessBackend;
    // args.backends |= perfetto::kSystemBackend;
// Make Enum for both backend
typedef enum {
    PERFETTO_C_IN_PROCESS_BACKEND = 1,
    PERFETTO_C_SYSTEAM_BACKEND = 2
} perfetto_backend_t;


void perfetto_c_init(perfetto_backend_t backend);
void perfetto_c_trace_event(char *name);
void perfetto_c_trace_event_begin(char *name);
void perfetto_c_trace_event_end(void);
void perfetto_c_trace_thread_begin(char* thread_name, char *name);
void perfetto_c_trace_thread_end(char* thread_name);
void perfetto_c_log(char *msg);
void perfetto_c_event_instant(const char *name);
void perfetto_c_event_counter(const char *name, int value);
void perfetto_c_start_tracing(void);
void perfetto_c_stop_tracing(void);
void perfetto_c_set_current_thread_name(const char *name);
void perfetto_c_register_process(const char *name);

#ifdef __cplusplus
}
#endif

#endif