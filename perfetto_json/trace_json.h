#ifndef __TRACE_JSON_H
#define __TRACE_JSON_H

// buffer size 500MB
#define TRACE_JSON_BUFFER_SIZE 500000000
// How often the cache is written to the file
#define TRACE_JSON_CACHE_SIZE 2048
#define MAX_BUFFER_SIZE 200

typedef enum {
  TRACE_JSON_CIRCULAR_BUFFER,
  TRACE_JSON_SINGLE_FILE
} trace_json_buffer_t;

void trace_json_start();
void trace_json_init(const char* filename,trace_json_buffer_t buffer_type, unsigned int buffer_size);
void trace_json_stop();
void trace_json_begin(const char* name, int pid);
void trace_json_end(const char* name, int pid);
void trace_json_marker(const char* name, int pid);
void trace_json_counter(const char* name, int pid, int value);

#define START_TRACE(func_name) trace_json_begin(func_name,0)
#define END_TRACE(func_name) trace_json_end(func_name, 0)

// Define TRACE_WRAP macro
// This will allow the tracing of function without modifying its code
#define TRACE_WRAP(func_call) \
    ({ \
        START_TRACE(#func_call); \
        typeof(func_call) __result = func_call; \
        END_TRACE(#func_call); \
        __result; \
    })

#endif