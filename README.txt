pull and build perfetto : make perfetto
build c wrapper : make build
build examples / tests : make test
Test capture :  
traced &
traced_probes &
tracebox perfetto -o sdk_with_system.pftrace --txt -c config/with_track_event.cfg &
test/test_c
Check the output file in perfetto UI
