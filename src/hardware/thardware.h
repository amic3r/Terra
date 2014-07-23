
#ifndef __included_terra_hardware_h
#define __included_terra_hardware_h

#ifdef _WINDOWS
#include <Windows.h>
#endif

//--- Screens ------------------------------//

typedef struct {
#ifdef _WINDOWS
	HMONITOR monitorHandle;
#endif
	TRectangle dimensions;
} TScreen;

typedef struct {
	size_t numscreens;
	TScreen *screens;
} TScreens;

TScreens *TScreensGetInf(void);
void TScreensFree(TScreens *scrs);

//--- Mouse --------------------------------//

typedef struct {
	size_t numButtons;
	size_t speed;
} TMouse;

TMouse *TMouseGetInf(void);
void TMouseFree(TMouse *m);

//void printHardware(void);

#endif
