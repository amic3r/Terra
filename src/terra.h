
#ifndef _included_terra_h
#define _included_terra_h

#define TERRA_NAME        "Terra"
#define TERRA_DESCRIPTION "Utility library"
#define TERRA_VERSION     "0.1.0"

void print_credits(void);

#include "debugging/debug.h"
//#include "debugging/memory_leak.h"
//#include "debugging/profiler.h"

#include "particle/particle.h"

#include "structures/slist.h"

#include "utilities/filesys.h"
#include "utilities/integer.h"
#include "utilities/math.h"
#include "utilities/rand.h"
#include "utilities/string.h"
#include "utilities/time.h"

#include "structures/array.h"
#include "structures/list.h"
//#include "structures/tree.h"
#include "structures/heap.h"
#include "structures/quadtree.h"
//#include "structures/octree.h"

#include "io/io_reader.h"
#include "io/io_writer.h"
#include "io/file_system.h"

#endif
