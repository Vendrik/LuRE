#pragma once

#include <stdio.h>
#include <stdlib.h>

#define LUMI_ASSERT(x, m) \
	do { \
		if(!(x)) \
		{ \
			char* c = nullptr, *file = __FILE__; \
			for(; *file!='\0'; file++) \
				if (*file == '\\' || *file=='/') \
					c = file; \
			if (*c == '\\') \
				c++; \
			printf("******FATAL ERROR*******\n"); \
			printf("%s\n", #x); \
			printf("%s:%d\n", c, __LINE__); \
			printf("%s\n", m); \
			getchar(); \
			exit(-1); \
		} \
	} while (false)

#define LUMI_LOG_LEVEL_FATAL 0
#define LUMI_LOG_LEVEL_ERROR 1
#define LUMI_LOG_LEVEL_WARN  2
#define LUMI_LOG_LEVEL_INFO  3

#ifndef LUMI_LOG_LEVEL
#define LUMI_LOG_LEVEL LUMI_LOG_LEVEL_INFO
#endif // !LUMI_LOG_LEVEL

#if LUMI_LOG_LEVEL >= LUMI_LOG_LEVEL_FATAL
#define LUMI_FATAL(x, ...) do { printf("[LUMI][FATAL]: "); printf(x, __VA_ARGS__); printf("\n"); LUMI_ASSERT(false, ""); } while(false)
#else
define LUMI_FATAL(x, ...)
#endif // LUMI_LOG_LEVEL

#if LUMI_LOG_LEVEL >= LUMI_LOG_LEVEL_ERROR
#define LUMI_ERROR(x, ...) do {printf("[LUMI][ERROR]: "); printf(x, __VA_ARGS__); printf("\n"); } while(false)
#else
#define LUMI_ERROR(x, ...)
#endif // LUMI_LOG_LEVEL

#if LUMI_LOG_LEVEL >= LUMI_LOG_LEVEL_WARN
#define LUMI_WARN(x, ...) do {printf("[LUMI][WARN]: "); printf(x, __VA_ARGS__); printf("\n"); } while(false)
#else
#define LUMI_WARN(x, ...)
#endif // LUMI_LOG_LEVEL

#if LUMI_LOG_LEVEL >= LUMI_LOG_LEVEL_INFO
#define LUMI_INFO(x, ...) do {printf("[LUMI][INFO]: "); printf(x, __VA_ARGS__); printf("\n"); } while(false)
#else
#define LUMI_INFO(x, ...)
#endif // LUMI_LOG_LEVEL