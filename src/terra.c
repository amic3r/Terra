#include "stdafx.h"

#include "terra.h"

void print_credits(void)
{
	printf(TERRA_NAME " v" TERRA_VERSION);
	printf("\nBuilt with: \n");
	printf("GLib v%d.%d.%d\n",GLIB_MAJOR_VERSION,GLIB_MINOR_VERSION,GLIB_MICRO_VERSION);
}
