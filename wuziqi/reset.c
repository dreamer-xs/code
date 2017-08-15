#include <config.h>
#include <pmu.h>

void reset(void)
{
	set_one(SWRESET, 0);
}


