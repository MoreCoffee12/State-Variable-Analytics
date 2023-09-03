#include <math.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include "CEOSModelHarness\CBWRSHarness\bwrs.h"

__declspec(dllexport) int __cdecl ShowName(int fluidindex, char* textline);