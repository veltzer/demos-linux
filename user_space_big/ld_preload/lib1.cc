#include "us_helper.hh"

extern "C" double sin(double x)
{
	TRACE("start");
	TRACE("end");
	return(x);
}
