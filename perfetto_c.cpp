
#include "perfetto_c.h"
#include "perfetto_setup.h"
#include <time.h>
#include <fstream>

PERFETTO_TRACK_EVENT_STATIC_STORAGE();

namespace {
void trace_helper_init(void){
    perfetto::Category::Group("io");
    perfetto::TracingInitArgs args;
    // args.backends |= perfetto::kInProcessBackend;
    args.backends |= perfetto::kSystemBackend;
    perfetto::Tracing::Initialize(args);
    perfetto::TrackEvent::Register();  // Add this.

    perfetto::ProcessTrack process_track = perfetto::ProcessTrack::Current();
    perfetto::protos::gen::TrackDescriptor desc = process_track.Serialize();
    desc.mutable_process()->set_process_name("Example");
    perfetto::TrackEvent::SetTrackDescriptor(process_track, desc);
}

}  // namespace

void perfetto_c_trace_event(char *name){
    TRACE_EVENT("io", name);
}
void perfetto_c_trace_event_begin(char *name){
    TRACE_EVENT_BEGIN("io", name);
}
void perfetto_c_trace_event_end(void){
    TRACE_EVENT_END("io");
}
void perfetto_c_log(char *msg){
    PERFETTO_LOG(msg);
}
void perfetto_c_init(void){
    trace_helper_init();
}
void perfetto_c_event_instant(const char *name){
    TRACE_EVENT_INSTANT("io", name);
}
void perfetto_c_event_counter(const char *name, int value){
    TRACE_COUNTER("io", name, value);
}