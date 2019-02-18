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

typedef void(*krabs_callback)(const EVENT_RECORD *);

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
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

    typedef enum {
        std_string,
        std_wstring,
        counted_wide_string,
    } krabs_string_type;

    typedef struct {
        union {
            DWORD v4;
            BYTE v6[16];
        };
        bool is_ipv6;
    } krabs_ip_address;

    typedef struct {
        krabs_status_t status;
        char msg[256];
    } krabs_status_ctx;

    DECLARE_HANDLE(krabs_filter);
    DECLARE_HANDLE(krabs_user_trace);
    DECLARE_HANDLE(krabs_user_provider);
    DECLARE_HANDLE(krabs_event_schema);
    DECLARE_HANDLE(krabs_event_parser);
    DECLARE_HANDLE(krabs_property_name);

    // Exported Functions
    FLATKRABSETW_API krabs_property_name* krabs_create_property_name(
        krabs_status_ctx *status,
        const wchar_t *name
    );

    FLATKRABSETW_API krabs_user_trace* krabs_create_user_trace(
        krabs_status_ctx *status,
        const wchar_t *name
    );

    FLATKRABSETW_API krabs_user_provider* krabs_create_user_provider(
        krabs_status_ctx *status,
        const wchar_t *const provider_name,
        ULONGLONG any_flags,
        ULONGLONG all_flags
    );

    FLATKRABSETW_API void krabs_enable_user_provider(
        krabs_status_ctx * status,
        krabs_user_trace *const trace,
        krabs_user_provider *provider
    );

    FLATKRABSETW_API krabs_filter* krabs_create_filter_for_event_ids(
        krabs_status_ctx *status,
        const USHORT *const event_ids,
        size_t event_ids_count
    );

    FLATKRABSETW_API void krabs_add_event_callback_to_user_provider(
        krabs_status_ctx *status,
        krabs_user_provider *const provider,
        krabs_callback callback
    );

    FLATKRABSETW_API void krabs_add_callback_to_event_filter(
        krabs_status_ctx *status,
        krabs_filter *const filter,
        krabs_callback callback
    );

    FLATKRABSETW_API void krabs_add_event_filter_to_user_provider(
        krabs_status_ctx *status,
        krabs_user_provider *const provider,
        krabs_filter *const filter
    );

    FLATKRABSETW_API void krabs_start_user_trace(
        krabs_status_ctx *status,
        krabs_user_trace *const trace
    );

    FLATKRABSETW_API void krabs_stop_user_trace(
        krabs_status_ctx *status,
        krabs_user_trace *const trace
    );

    FLATKRABSETW_API krabs_event_schema* krabs_get_event_schema(
        krabs_status_ctx *status,
        const EVENT_RECORD *rec
    );

    FLATKRABSETW_API krabs_event_parser* krabs_get_event_parser(
        krabs_status_ctx *status,
        krabs_event_schema *const schema
    );

    FLATKRABSETW_API wchar_t* krabs_get_string_property_from_parser(
        krabs_status_ctx *status,
        krabs_event_parser *const parser,
        krabs_property_name *const property_name,
        krabs_string_type string_type
    );

    FLATKRABSETW_API uint32_t krabs_get_u32_property_from_parser(
        krabs_status_ctx *status,
        krabs_event_parser *const parser,
        krabs_property_name *const property_name
    );

    FLATKRABSETW_API uint16_t krabs_get_u16_property_from_parser(
        krabs_status_ctx *status,
        krabs_event_parser *const parser,
        krabs_property_name *const property_name
    );

    FLATKRABSETW_API krabs_ip_address* krabs_get_ip_addr_property_from_parser(
        krabs_status_ctx *status,
        krabs_event_parser *const parser,
        krabs_property_name *const property_name
    );

    FLATKRABSETW_API void krabs_destroy_user_provider(
        krabs_user_provider *provider
    );

    FLATKRABSETW_API void krabs_destroy_user_trace(
        krabs_user_trace *trace
    );

    FLATKRABSETW_API void krabs_destroy_event_filter(
        krabs_filter *filter
    );

    FLATKRABSETW_API void krabs_destroy_event_schema(
        krabs_event_schema *schema
    );

    FLATKRABSETW_API void krabs_destroy_event_parser(
        krabs_event_parser *parser
    );

    FLATKRABSETW_API void krabs_destroy_property_name(
        krabs_property_name *property_name
    );

#ifdef __cplusplus
}
#endif

#endif