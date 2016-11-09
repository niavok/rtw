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

#ifndef RTW_TRACER_H
#define RTW_TRACER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define CREATE_TRACER(tracer) static RtwTracer *tracer = NULL; \
    if(tracer == NULL) tracer = rtw_tracer_new(NULL, #tracer);

typedef struct _RtwTracerContext RtwTracerContext;

typedef struct _RtwTracer RtwTracer;

/* Public API */
RtwTracerContext *rtw_tracer_context_new(void);
void rtw_tracer_context_free(RtwTracerContext *context);

RtwTracer *rtw_tracer_new(RtwTracerContext *context, const char* name);
void rtw_tracer_free(RtwTracer *tracer);

void rtw_tracer_start(RtwTracer *tracer);
void rtw_tracer_stop(RtwTracer *tracer);

#ifdef __cplusplus
}
#endif

#endif
