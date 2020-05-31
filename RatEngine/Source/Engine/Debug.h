#pragma once
#include <stdio.h>
#include <iostream>

#define LOG_ERROR "Error"
#define LOG_WARNING "Warning"
#define LOG_DEBUG "Debug"

#define DEBUG_LOG(category, level, message, ...) \
	fprintf(stderr, "[%s] ", category); \
	fprintf(stderr, "[%s] (%s:%d): ", level, __FILE__, __LINE__); \
	fprintf(stderr, message, ##__VA_ARGS__); \
	fprintf(stderr, "\n")