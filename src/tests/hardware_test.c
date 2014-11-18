#include "stdafx.h"

#include "hardware_test.h"

#include "test_utils.h"

#define TERRA_RECTANGLE
#include "tmath.h"

#include "hardware/thardware.h"

void hardware_test_screen_inf(void)
{
	TScreens *scrs = TScreensGetInf();
	size_t i = 0;

	printf("  Screens: amount: %zd\n",scrs->numscreens);

	for(; i < scrs->numscreens; ++i)
		printf("    screen %zd: width: %zd, height: %zd\n", i, scrs->screens[i].dimensions.w, scrs->screens[i].dimensions.h);

	TScreensFree(scrs);
	scrs = 0;
}

void hardware_test_mouse_inf(void)
{
	TMouse *m = TMouseGetInf();

	printf("  Mouse: NumButtons: %zd, Speed: %zd\n",m->numButtons, m->speed);

	TMouseFree(m);
	m = 0;

	/*TMouseChangeSpeed(20);

	m = TMouseGetInf();

	printf("  Mouse: NumButtons: %d, Speed: %d\n",m->numButtons, m->speed);

	TMouseFree(m);
	m = 0;

	TMouseChangeSpeed(10);*/
}


void hardware_test(void)
{
	TLogReport(T_LOG_PROGRESS,0,"Testing Hardware analyser...\n");

	//printHardware();

	hardware_test_screen_inf();

	hardware_test_mouse_inf();

	TLogReport(T_LOG_PROGRESS,0,"hardware analyser tests completed.\n");
}
