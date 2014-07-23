#include "stdafx.h"

#define TERRA_RECTANGLE
#include "tmath.h"

#include "thardware.h"

#include "talloc.h"

//--- Screens ------------------------------//

#ifdef _WINDOWS
int monitorEnum(HMONITOR hm, HDC hdc, LPRECT rect, LPARAM p)
{
	struct Data {
		TScreen *screens;
		int idx;
	} *data = (struct Data *) p;

	TScreen *s = &data->screens[data->idx++];
	s->monitorHandle = hm;
	s->dimensions.x = rect->left;
	s->dimensions.y = rect->top;
	s->dimensions.w = rect->right-rect->left;
	s->dimensions.h = rect->bottom-rect->top;

	return 1;
}
#endif

TScreens *TScreensGetInf(void)
{
#ifdef _WINDOWS
	TScreens *scrs = (TScreens *) TAlloc(sizeof(TScreens));
	struct Data {
		TScreen *screens;
		int idx;
	} data;
	
	scrs->numscreens = GetSystemMetrics(SM_CMONITORS);

	scrs->screens = data.screens = (TScreen *) TAlloc(sizeof(TScreen) * scrs->numscreens);
	data.idx = 0;

	EnumDisplayMonitors(NULL,NULL,(MONITORENUMPROC) monitorEnum,(LPARAM) &data);

	return scrs;
#endif

	return 0;
}

void TScreensFree(TScreens *scrs)
{
	free(scrs->screens);
	free(scrs);
}

//--- Mouse --------------------------------//

TMouse *TMouseGetInf(void)
{
#ifdef _WINDOWS
	TMouse *m = (TMouse *) TAlloc(sizeof(TMouse));
	if(m) {
		int mouseInf[3];

		m->numButtons = GetSystemMetrics(SM_CMOUSEBUTTONS);
		SystemParametersInfo(SPI_GETMOUSESPEED,0,&m->speed,0);


		SystemParametersInfo(SPI_GETMOUSE,0,&mouseInf,0);
		mouseInf[2] *= 2;
		SystemParametersInfo(SPI_SETMOUSE,0,&mouseInf,SPIF_SENDCHANGE);
	}
	return m;
#endif

	return 0;
}

void TMouseFree(TMouse *m)
{
	free(m);
}

/*void printMouse(void)
{
	int numMouseButton = GetSystemMetrics(SM_CMOUSEBUTTONS);

	printf("  Mouse: Button amount : %d.\n",numMouseButton);
}

void printRAM(void)
{
	unsigned long long mem;
	GetPhysicallyInstalledSystemMemory(&mem);
	printf("  RAM: %ld Available.\n",mem);

}

void printCPU(void)
{
	const char *archs[] = {"x86","x64","ARM","Itanium","Unknown"};
	int arch = 4;

	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);

	if (siSysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		arch = 1;
	else if (siSysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM)
		arch = 2;
	else if (siSysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		arch = 3;
	else if (siSysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
		arch = 0;

	printf("  CPU: Architecture : %s, Number Of Processors : %ld.\n", archs[arch], siSysInfo.dwNumberOfProcessors);
}

void printDrive(const char *drivename)
{
	const char *sizes[] = {"Bytes","KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
	ULARGE_INTEGER available, total;
	int bestsize = 0;
	unsigned long long cavail, ctotal;
	const char *size;

	GetDiskFreeSpaceEx("C:\\", &available, &total, 0);

	cavail = available.QuadPart;
	ctotal = total.QuadPart;

	while((float) (ctotal / 1024) >= 1) {
		bestsize++;
		cavail = (unsigned long long) (cavail / 1024);
		ctotal = (unsigned long long) (ctotal / 1024);
	}
	size = sizes[bestsize];

	printf("  Disk %s : Available %llu%s, Total %llu%s.\n", drivename, cavail, size, ctotal, size);
}

void printHardware(void)
{
	printf("System Information:\n");
	//printRAM();
	//printCPU();
	//printDrive("C:\\");
	printMouse();
}*/
