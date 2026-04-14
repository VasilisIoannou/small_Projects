#ifndef _LOG_H__
#define _LOG_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void create_log();
void write_log(const char* str);
void close_log();

#endif
