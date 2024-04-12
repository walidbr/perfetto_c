#include "trace_json.h"
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <string.h>

FILE* json;
char buffer_cache[TRACE_JSON_CACHE_SIZE];
#define FILENAME "trace.json"

static int64_t get_timestamp() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void write_file(const char* buffer) {
	if(strlen(buffer_cache) < TRACE_JSON_CACHE_SIZE - MAX_BUFFER_SIZE){
		// Write buffer to local cache
		strcat(buffer_cache, buffer);
	}
	else if(ftell(json) < TRACE_JSON_BUFFER_SIZE - strlen(buffer_cache) - strlen(buffer)){
		// Dump cache to a file
		if(!json) fopen(FILENAME, "a");
		// overwirte the last ] in the file
		if(ftell(json) > 2)
			fseek(json, -2, SEEK_END);
		fprintf(json, buffer_cache);
		fprintf(json, buffer);
		fprintf(json, "]\n");
		buffer_cache[0] = '\0';
		if(!json) fclose(json);
	}
}

void trace_json_start() {
	if(!json) json = fopen(FILENAME, "w");
	fprintf(json, "[\n");
	if(!json) fclose(json);
}

void trace_json_init(const char* filename, trace_json_buffer_t buffer_type, uint32_t buffer_size) {
	// json = fopen(filename, "w");
	buffer_cache[0] = '\0';
}

void trace_json_stop() {
	if(!json) json = fopen(FILENAME, "a");
	if(buffer_cache[0] != '\0') {
		if(ftell(json) > 2)
			fseek(json, -2, SEEK_END);
		fprintf(json,buffer_cache);
		buffer_cache[0] = '\0';
		fprintf(json, "]\n");
		if(!json) fclose(json);
	}
}

void trace_json_begin(const char* name, int pid) {
	int64_t ts = get_timestamp();
    const char * category = "test";
	const char *tid = name;
	char buffer [MAX_BUFFER_SIZE];
	sprintf(buffer, "{\"name\": \"%s\", \"cat\": \"%s\", \"ph\": \"B\", \"pid\": %d, \"tid\": \"%s\", \"ts\": %lu},\n", name, category, pid, tid, ts);
	write_file(buffer);
}

void trace_json_end(const char* name, int pid) {
	int64_t ts = get_timestamp();
    const char * category = "test";
	const char *tid = name;
	char buffer [MAX_BUFFER_SIZE];
	sprintf(buffer, "{\"name\": \"%s\", \"cat\": \"%s\", \"ph\": \"E\", \"pid\": %d, \"tid\": \"%s\", \"ts\": %lu},\n", name, category, pid, tid, ts);
	write_file(buffer);
}

void trace_json_marker(const char* name, int pid) {
	char buffer [MAX_BUFFER_SIZE];
	sprintf(buffer, "{\"name\": \"%s\", \"ph\": \"i\", \"s\": \"t\", \"pid\": %d, \"tid\": \"%s\", \"ts\": %lu},\n", name, pid, name, get_timestamp());
	write_file(buffer);
}

void trace_json_counter(const char* name, int pid, int value) {
	char buffer [MAX_BUFFER_SIZE];
	sprintf(buffer, "{\"name\": \"%s\", \"ph\": \"C\", \"pid\": %d, \"tid\": \"%s\", \"ts\": %lu, \"args\": {\"%s\": %d}},\n", name, pid, name, get_timestamp(), name, value);
	write_file(buffer);
}
