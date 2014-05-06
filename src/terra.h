
#ifndef _included_terra_h
#define _included_terra_h

#define TERRA_NAME        "Terra"
#define TERRA_DESCRIPTION "Utility library"
#define TERRA_VERSION     "0.1.0"

void print_credits(void);

#include <glib.h>

#include "define.h"

#include "debugging/debug.h"
#include "debugging/tlog.h"
//#include "debugging/memory_leak.h"
//#include "debugging/profiler.h"

#include "tmath.h"
#include "trand.h"
#include "ttime.h"

#include "thread/thread.h"

#include "structure/slist.h"

#include "utility/filesys.h"
#include "utility/integer.h"

#include "utility/string.h"

#include "structure/array.h"
#include "structure/list.h"
//#include "structure/RBtree.h"
#include "structure/heap.h"
#include "structure/quadtree.h"
//#include "structure/octree.h"

#include "io/io_reader.h"
#include "io/io_writer.h"
#include "io/file_system.h"

#endif
