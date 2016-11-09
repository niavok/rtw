/*
 * Copyright (c) 2016, Frédéric Bertolus <frederic.bertolus@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#define _GNU_SOURCE

#include <time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>

#include "rtw_tracer.h"

/* TODO: add configuration setter */
#define DEFAULT_USE_RUSAGE 0
#define DEFAULT_PRINT_DURATION 1
#define DEFAULT_PRINT_STATS 1
#define DEFAULT_SAMPLE_BUFFER_SIZE 1000

struct _RtwTracerContext
{
    /* Reserved for future usage */
};

struct _RtwTracer
{
    RtwTracerContext *context;
    /* Tracer config*/
    const char* name;
    int print_duration;
    int print_stats;
    int use_rusage;
    int sample_buffer_size;

    /* Dynamic data */
    int64_t start_time;
    int64_t stop_time;
    int sampleIndex;
    int64_t *samples;
};

RtwTracerContext *rtw_tracer_context_new(void)
{
    return malloc(sizeof(RtwTracerContext));
}

void rtw_tracer_context_free(RtwTracerContext *context)
{
    free(context);
}

RtwTracer *rtw_tracer_new(RtwTracerContext *context, const char* name)
{
    RtwTracer *tracer = malloc(sizeof(RtwTracer));
    tracer->context = context;
    tracer->name = name;
    tracer->print_duration = DEFAULT_PRINT_DURATION;
    tracer->print_stats = DEFAULT_PRINT_STATS;
    tracer->sample_buffer_size = DEFAULT_SAMPLE_BUFFER_SIZE;
    tracer->sampleIndex = 0;
    tracer->use_rusage = DEFAULT_USE_RUSAGE;

    if(tracer->print_stats)
    {
        tracer->samples = malloc(sizeof(int64_t) * DEFAULT_SAMPLE_BUFFER_SIZE);
    }
    else
    {
        tracer->samples = NULL;
    }

    return tracer;
}

void rtw_tracer_free(RtwTracer *tracer)
{
    if(tracer->samples)
    {
        free(tracer->samples);
    }

    free(tracer);
}

static int64_t rtw_tracer_get_timestamp(RtwTracer *tracer)
{
    if(tracer->use_rusage)
    {
        struct rusage usage;

        getrusage(RUSAGE_THREAD, &usage);

        printf("usage.ru_utime.tv_sec %d\n", usage.ru_utime.tv_sec);
        printf("usage.ru_utime.tv_usec %d\n", usage.ru_utime.tv_usec);
        printf("usage.ru_stime.tv_sec %d\n", usage.ru_stime.tv_sec);
        printf("usage.ru_stime.tv_usec %d\n", usage.ru_stime.tv_usec);

        int64_t user_time = (usage.ru_utime.tv_sec*1000000 + usage.ru_utime.tv_usec);
        int64_t system_time = (usage.ru_stime.tv_sec*1000000 + usage.ru_stime.tv_usec);

        return user_time + system_time;
    }
    else
    {
        struct timespec ts;

        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (ts.tv_sec*1000000 + ts.tv_nsec/1000);
    }
}

void rtw_tracer_start(RtwTracer *tracer)
{
    tracer->start_time = rtw_tracer_get_timestamp(tracer);
}

void rtw_tracer_stop(RtwTracer *tracer)
{
    tracer->stop_time = rtw_tracer_get_timestamp(tracer);

    int64_t delta_time = tracer->stop_time-tracer->start_time;

    if(tracer->print_stats)
    {
        tracer->samples[tracer->sampleIndex++] = delta_time;

        if (tracer->sampleIndex == tracer->sample_buffer_size)
        {
            int32_t i;
            int64_t total;
            int64_t mean;
            int64_t meanAbsoluteDifference;
            int64_t max_difference = 0;
            int64_t max_duration = 0;
            int64_t min_duration = 100000000;

            // Mean
            total = 0;
            for (i = 0; i < tracer->sample_buffer_size; i++)
            {
               int64_t sample = tracer->samples[i];
               total += sample;

               if (sample > max_duration)
               {
                   max_duration = sample;
               }
               if (sample < min_duration)
               {
                   min_duration = sample;
               }
            }
            mean = total / tracer->sample_buffer_size;

            total = 0;
            for (i = 0; i < tracer->sample_buffer_size; i++)
            {
               int64_t difference = tracer->samples[i] - mean;
               if(difference  < 0)
               {
                   difference = -difference;
               }
               
               total += difference;

               if (difference > max_difference)
               {
                   max_difference = difference;
               }
            }

            meanAbsoluteDifference = total / tracer->sample_buffer_size;

            printf("%s - mean duration : %lld us\n", tracer->name, mean);
            printf("%s - min: %lld us    max: %lld us\n", tracer->name, min_duration, max_duration);
            printf("%s - nean absolute difference: %lld us    max difference: %lld us\n", tracer->name, meanAbsoluteDifference, max_difference);

            tracer->sampleIndex = 0;
        }
    }

    if (tracer->print_duration)
    {
        printf("%s - duration : %lld us\n", tracer->name, delta_time);
    }
}
