#ifndef __PERFETTO_SETUP_H__
#define __PERFETTO_SETUP_H__

#include <perfetto.h>
#include <iostream>
PERFETTO_DEFINE_CATEGORIES(
    perfetto::Category("io").SetDescription("This is the io category"),
    perfetto::Category("dbg").SetTags("debug"));
#endif