
#ifndef __terra_test_test_utils_h
#define __terra_test_test_utils_h

#include "debugging/tdebug.h"

void test_create_file(const char *filename, const char *data);

static inline void testReport(unsigned char v, const char *testmsg) {
	TLogStartReport(T_LOG_PROGRESS,0);
	TLogWrite(testmsg);
	TLogWrite(v ? "Success\n" : "Failed\n");
}

static inline unsigned char testEqual(void *v1,void *v2, const char *testmsg) {
	testReport(v1 == v2,testmsg);

	return v1 == v2;
}

static inline unsigned char testNotEqual(void *v1,void *v2, const char *testmsg) {
	testReport(v1 != v2,testmsg);

	return v1 != v2;
}

static inline unsigned char testNull(void *v, const char *testmsg) {
	return testEqual(v,0,testmsg);
}

static inline unsigned char testNotNull(void *v, const char *testmsg) {
	return testNotEqual(v,0,testmsg);
}

#endif
