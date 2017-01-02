#ifndef __FLATKRABSETW_H__
#define __FLATKRABSETW_H__

#include "stdafx.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FLATKRABSETW_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FLATKRABSETW_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FLATKRABSETW_EXPORTS
#define FLATKRABSETW_API __declspec(dllexport)
#else
#define FLATKRABSETW_API __declspec(dllimport)
#endif

typedef void(*krabs_callback)(const EVENT_RECORD &);

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        krabs_success = 0,
        krabs_error_unknown_error = 1,
        krabs_error_invalid_parameter = 2,
        krabs_error_invalid_trace_failure = 3,
        krabs_error_start_trace_failure = 4,
        krabs_error_needs_admin_failure = 5,
        krabs_error_unknown_schema = 6,
        krabs_error_type_mismatch = 7,
        krabs_error_trace_already_registered = 8,
    } krabs_status_t;

    typedef struct
    {
        krabs_status_t status;
        char msg[256];
    } krabs_status_ctx;

    DECLARE_HANDLE(krabs_filter);

    DECLARE_HANDLE(krabs_user_trace);
    DECLARE_HANDLE(krabs_user_provider);

    // Exported Functions
    FLATKRABSETW_API krabs_user_trace *krabs_create_user_trace(
        krabs_status_ctx *status,
        const wchar_t *name);

    FLATKRABSETW_API krabs_user_provider *krabs_create_user_provider(
        krabs_status_ctx *status,
        krabs_user_trace *trace,
        const wchar_t *provider_name,
        ULONGLONG any_flags,
        ULONGLONG all_flags);

    FLATKRABSETW_API void krabs_enable_user_provider(
        krabs_status_ctx *status,
        krabs_user_trace *trace,
        krabs_user_provider *provider);

    FLATKRABSETW_API krabs_filter *krabs_create_filter(
        krabs_status_ctx *status
    );

    FLATKRABSETW_API void krabs_add_event_callback_to_provider(
        krabs_status_ctx *status,
        krabs_user_provider* provider,
        krabs_callback callback);

    FLATKRABSETW_API void krabs_start_trace(
        krabs_status_ctx *status,
        krabs_user_trace *trace);

#ifdef __cplusplus
}
#endif

#endif