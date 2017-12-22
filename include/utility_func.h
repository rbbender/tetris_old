#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) do {printf(fmt, ##args);} while(false)
#define DEBUG_VAR(fmt, var) do {printf("%s:%s:%s:" fmt, __FILE__, __func__, #var, var);} while (false)
#define DEBUG_TRACE do {printf("%s:%s\n", __FILE__, __func__);} while (false)
#else
#define DEBUG_PRINT(fmt, args...) while (false) {}
#define DEBUG_VAR(fmt, var) while (false) {}
#define DEBUG_TRACE while (false) {}
#endif


#ifndef _UTILITY_FUNC_H_
#define _UTILITY_FUNC_H_

#include <stdio.h>
#include <utility>

typedef std::pair<unsigned short, unsigned short> FieldAddr_t;

unsigned get_time();

unsigned long long get_current_time_ms();

#endif
