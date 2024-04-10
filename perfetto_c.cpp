
#include "perfetto_c.h"
#include "perfetto_setup.h"
#include <time.h>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>

PERFETTO_TRACK_EVENT_STATIC_STORAGE();
std::unique_ptr<perfetto::TracingSession> g_tracing_session;
std::mutex mtx;
std::condition_variable cv;
enum class ThreadState { Idle, Start, End };
struct ThreadInfo {
    ThreadState state;
    bool keep_running;
    std::condition_variable cv;
    std::mutex mtx;
    std::thread thread;
    std::string track_name;
    std::string thread_name;
};
std::unordered_map<const char*, ThreadInfo> thread_map;

// When there is a new thread_name specified, spin this thread_function and wait for the condition variable
// to either start tracing or stop tracing.
void thread_function(ThreadInfo& info) {
    while (info.keep_running) {
        std::unique_lock<std::mutex> lock(info.mtx);
        info.cv.wait(lock, [&info]() {
            return !info.keep_running || 
            info.state != ThreadState::Idle ||
            info.state != ThreadState::Start ||
            info.state != ThreadState::End;
        });

        if (!info.keep_running) {
            break; // Exit the loop if keep_running is false
        }

        // Execute one of the three functions based on the state
        if (ThreadState::Idle == info.state){
            // TRACE_EVENT("io", "TODO pass the name in CV");
        } else if (ThreadState::Start == info.state){
            info.state = ThreadState::Idle;
            TRACE_EVENT_BEGIN("io", info.track_name.c_str());
        } else if (ThreadState::End == info.state){
            info.state = ThreadState::Idle;
            TRACE_EVENT_END("io");
        }
        // info.state = ThreadState::Idle;
    }
    // Set this in the end to reduce the latency
    perfetto_c_set_current_thread_name(info.thread_name.c_str());
}

std::unique_ptr<perfetto::TracingSession> StartTracing() {
  // The trace config defines which types of data sources are enabled for
  // recording. In this example we just need the "track_event" data source,
  // which corresponds to the TRACE_EVENT trace points.
  perfetto::TraceConfig cfg;
  cfg.add_buffers()->set_size_kb(1024);
  auto* ds_cfg = cfg.add_data_sources()->mutable_config();
  ds_cfg->set_name("track_event");

  auto tracing_session = perfetto::Tracing::NewTrace();
  tracing_session->Setup(cfg);
  tracing_session->StartBlocking();
  return tracing_session;
}

void StopTracing(std::unique_ptr<perfetto::TracingSession> tracing_session) {
  // Make sure the last event is closed for this example.
  perfetto::TrackEvent::Flush();

  // Stop tracing and read the trace data.
  tracing_session->StopBlocking();
  std::vector<char> trace_data(tracing_session->ReadTraceBlocking());

  // Write the result into a file.
  // Note: To save memory with longer traces, you can tell Perfetto to write
  // directly into a file by passing a file descriptor into Setup() above.
  std::ofstream output;
  output.open("example.pftrace", std::ios::out | std::ios::binary);
  output.write(&trace_data[0], std::streamsize(trace_data.size()));
  output.close();
  PERFETTO_LOG(
      "Trace written in example.pftrace file. To read this trace in "
      "text form, run `./tools/traceconv text example.pftrace`");
}

void trace_helper_init(perfetto_backend_t backend){
    perfetto::Category::Group("io");
    perfetto::TracingInitArgs args;
    if(backend & PERFETTO_C_SYSTEAM_BACKEND)
        args.backends |= perfetto::kSystemBackend;
    if(backend& PERFETTO_C_IN_PROCESS_BACKEND)
        args.backends |= perfetto::kInProcessBackend;
    perfetto::Tracing::Initialize(args);
    perfetto::TrackEvent::Register();  // Add this.

    perfetto::ProcessTrack process_track = perfetto::ProcessTrack::Current();
    perfetto::protos::gen::TrackDescriptor desc = process_track.Serialize();
    desc.mutable_process()->set_process_name("perfetto_c_lib");
    perfetto::TrackEvent::SetTrackDescriptor(process_track, desc);
}


void perfetto_c_trace_event(char *name){
    TRACE_EVENT("io", name);
}
void perfetto_c_trace_event_begin(char *name){
    TRACE_EVENT_BEGIN("io", name);
}
void perfetto_c_trace_event_end(void){
    TRACE_EVENT_END("io");
}

void perfetto_c_trace_thread_begin(char* thread_name, char *name){
    if(thread_map.find(thread_name) == thread_map.end()){
        thread_map[thread_name].state = ThreadState::Idle;
        thread_map[thread_name].keep_running = true;
        thread_map[thread_name].thread = std::thread(thread_function, std::ref(thread_map[thread_name]));
        thread_map[thread_name].track_name = std::string(name);
        thread_map[thread_name].thread_name = std::string(thread_name);
    }
    thread_map[thread_name].state = ThreadState::Start;
}
void perfetto_c_trace_thread_end(char* thread_name){
    if(thread_map.find(thread_name) != thread_map.end()){
        thread_map[thread_name].state = ThreadState::End;
        thread_map[thread_name].cv.notify_one();
    }
}

void perfetto_c_log(char *msg){
    PERFETTO_LOG(msg);
}
void perfetto_c_init(perfetto_backend_t backend){
    trace_helper_init(backend);
}
void perfetto_c_event_instant(const char *name){
    TRACE_EVENT_INSTANT("io", name);
}
void perfetto_c_event_counter(const char *name, int value){
    TRACE_COUNTER("io", name, value);
}

void perfetto_c_start_tracing(void){
    g_tracing_session = StartTracing();
}

void perfetto_c_stop_tracing(void){
    for(auto it = thread_map.begin(); it != thread_map.end(); it++){
        it->second.keep_running = false;
        it->second.cv.notify_one();
        it->second.thread.join();
    }
    StopTracing(std::move(g_tracing_session));
}
// Relabel the current thread
void perfetto_c_set_current_thread_name(const char *name){
    perfetto::ThreadTrack thread_track = perfetto::ThreadTrack::Current();
    perfetto::protos::gen::TrackDescriptor desc = thread_track.Serialize();
    desc.mutable_thread()->set_thread_name(name);
    perfetto::TrackEvent::SetTrackDescriptor(thread_track, desc);
}

void perfetto_c_register_process(const char *name){
    perfetto::ProcessTrack process_track = perfetto::ProcessTrack::Current();
    perfetto::protos::gen::TrackDescriptor desc = process_track.Serialize();
    desc.mutable_process()->set_process_name(name);
    perfetto::TrackEvent::SetTrackDescriptor(process_track, desc);
}