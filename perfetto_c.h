#ifndef __PERFETTO_C_H__
#define __PERFETTO_C_H__
#ifdef __cplusplus
extern "C" {
#endif
void perfetto_c_init(void);
void perfetto_c_trace_event(char *name);
void perfetto_c_trace_event_begin(char *name);
void perfetto_c_trace_event_end(void);
void perfetto_c_log(char *msg);
void perfetto_c_event_instant(const char *name);
void perfetto_c_event_counter(const char *name, int value);
#ifdef __cplusplus
}
#endif

#endif