
#ifndef _included_terra_h
#define _included_terra_h

#define TERRA_NAME        "Terra"
#define TERRA_DESCRIPTION "Utility library"
#define TERRA_VERSION     "0.1.0"

void print_credits(void);

#define TERRA_LINE
#define TERRA_RECTANGLE
#define TERRA_CUBOID

#include <glib.h>

#include "tdefine.h"

#include "debugging/tdebug.h"
#include "debugging/tlog.h"
//#include "debugging/memory_leak.h"
//#include "debugging/profiler.h"

#include "tmath.h"
#include "trand.h"
#include "ttime.h"

#include "thread/tthread.h"

#include "structure/tlist.h"

#include "utility/tfilesys.h"
#include "utility/tinteger.h"

#include "utility/tstring.h"

#include "structure/tarray.h"
#include "structure/trbtree.h"
#include "structure/theap.h"
#include "structure/tquadtree.h"
//#include "structure/octree.h"

#include "io/treader.h"
#include "io/twriter.h"
#include "io/tio.h"

#endif
