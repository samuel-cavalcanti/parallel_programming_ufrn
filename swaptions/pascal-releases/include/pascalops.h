/*
    Copyright (C) 2017 Marcio Jales, Vitor Ramos

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _PASCALOPS_H_
#define _PASCALOPS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

/***TODO
 * s_info with variable len for filename and complete_mark
***/
struct s_info
{
    //long s_mark;
    int8_t s_mark[128];
    double s_start_time, s_stop_time;
    int s_start_line;
    int s_stop_line;
    int s_thread_id;
    //int s_cpu_id;
    char s_filename[64];
};

#define GET_TIME(now)                       \
{                                           \
    struct timeval t;                       \
    gettimeofday(&t, NULL);                 \
    now = t.tv_sec + t.tv_usec / 1000000.0; \
}

#define MAX_THREADS 128
#define MAX_ANNOTATIONS 128

#define RED "\x1b[31m"
#define RESET "\x1b[0m"

#if __GLIBC__ == 2 && __GLIBC_MINOR__ < 30
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#define getcpu(cpu) syscall(SYS_getcpu, cpu, NULL, NULL)
#endif

void _pascal_start(long id, int start_line, const char *filename);
void _pascal_stop(long id, int stop_line, const char *filename);


#define pascal_start(id) _pascal_start(id, __LINE__, __FILE__);
#define pascal_stop(id) _pascal_stop(id, __LINE__, __FILE__);


#ifdef __cplusplus
}
#endif

#endif // _PASCALOPS_H_
