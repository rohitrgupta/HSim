#include "common.h"
/*
int print_debug(int level,const char * fmt, ...){
	va_list args;

	if (level <= DEBUG_LEVEL) {
		fprintf(stdout, "[Debug %d]%s | line %d | ",level,__FILE__,__LINE__); 
		//fprintf(stdout, fmt, __VA_ARGS__); 
		va_start(args, fmt);
		vfprintf(stdout,fmt, args);
		va_end(args);
		fflush(stdout);
	}
	
	
}
*/