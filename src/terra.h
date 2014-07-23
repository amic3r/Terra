
#ifndef _included_terra_h
#define _included_terra_h

#define TERRA_NAME        "Terra"
#define TERRA_DESCRIPTION "Utility library"
#define TERRA_VERSION     "0.7.0"

#define TERRA_LINE
#define TERRA_RECTANGLE
#define TERRA_CUBOID

#include "tdefine.h"

#include "debugging/tdebug.h"
#include "debugging/tlog.h"
#include "debugging/tmemleak.h"
#include "debugging/tprofiler.h"

#include "tmath.h"
#include "trand.h"
#include "ttime.h"
#include "tthread.h"

#include "hardware/thardware.h"

#include "structure/tlist.h"

#include "utility/tfilesys.h"
#include "utility/tinteger.h"
#include "utility/tstring.h"

#include "structure/tarray.h"
#include "structure/trbtree.h"
#include "structure/theap.h"
#include "structure/tquadtree.h"
#include "structure/toctree.h"

#include "io/treader.h"
#include "io/twriter.h"
#include "io/tio.h"

#endif
