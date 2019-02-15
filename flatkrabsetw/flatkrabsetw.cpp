
#include "stdafx.h"
#include <set>
#include "flatkrabsetw.h"
#include <krabs.hpp>

// Exported Functions
FLATKRABSETW_API krabs_user_trace* krabs_create_user_trace(
    krabs_status_ctx *status,
    const wchar_t *name)
{
    ZeroMemory(status, sizeof krabs_status_ctx);
    krabs::user_trace *trace = nullptr;

    try {
        trace = new krabs::user_trace(name);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }

    return (krabs_user_trace*)trace;
}

FLATKRABSETW_API krabs_user_provider* krabs_create_user_provider(
    krabs_status_ctx *status,
    const wchar_t *provider_name,
    ULONGLONG any_flags,
    ULONGLONG all_flags)
{
    ZeroMemory(status, sizeof krabs_status_ctx);
    krabs::provider<> *provider = nullptr;

    try {
        provider = new krabs::provider<>(provider_name);
        provider->any(any_flags);
        provider->all(all_flags);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
        return nullptr;
    }

    return (krabs_user_provider*)provider;
}

FLATKRABSETW_API void krabs_enable_user_provider(
    krabs_status_ctx *status,
    krabs_user_trace *trace,
    krabs_user_provider *provider)
{
    ZeroMemory(status, sizeof krabs_status_ctx);
    try {
        krabs::user_trace* unwrapped_trace = (krabs::user_trace*)trace;
        krabs::provider<>* unwrapped_provider = (krabs::provider<>*)provider;
        unwrapped_trace->enable(*unwrapped_provider);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API krabs_filter* krabs_create_filter_for_event_ids(
    krabs_status_ctx *status,
    const USHORT *event_ids,
    size_t event_ids_count)
{
    ZeroMemory(status, sizeof krabs_status_ctx);
    krabs::event_filter *filter = nullptr;

    try {
        if (event_ids == nullptr) {
            throw std::invalid_argument("event_ids parameter is null");
        }

        if (event_ids_count <= 0) {
            throw std::invalid_argument("event_ids_count parameter is less than or equal to 0");
        }

        std::set<uint16_t> event_ids_to_filter;
        for (auto ii = 0; ii < event_ids_count; ++ii) {
            event_ids_to_filter.insert(event_ids[ii]);
        }

        auto predicate = [event_ids_to_filter](const EVENT_RECORD &record) -> bool {
            return event_ids_to_filter.find(record.EventHeader.EventDescriptor.Id) != event_ids_to_filter.end();
        };

        filter = new krabs::event_filter(predicate);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }

    return (krabs_filter*)filter;
}

FLATKRABSETW_API void krabs_add_event_callback_to_user_provider(
    krabs_status_ctx *status,
    krabs_user_provider *provider,
    krabs_callback callback)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::provider<> *unwrapped_provider = (krabs::provider<>*)provider;
        unwrapped_provider->add_on_event_callback(callback);
    }
    catch (const std::exception& ex)
    {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API void krabs_add_callback_to_event_filter(
    krabs_status_ctx *status,
    krabs_filter *filter,
    krabs_callback callback)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::event_filter *unwrapped_filter = (krabs::event_filter*)filter;
        unwrapped_filter->add_on_event_callback(callback);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API void krabs_add_event_filter_to_user_provider(
    krabs_status_ctx *status,
    krabs_user_provider *provider,
    krabs_filter *filter)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::event_filter *unwrapped_filter = (krabs::event_filter*)filter;
        krabs::provider<> *unwrapped_provider = (krabs::provider<>*)provider;

        unwrapped_provider->add_filter(*unwrapped_filter);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API void krabs_start_trace(
    krabs_status_ctx *status,
    krabs_user_trace *trace)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::user_trace *unwrapped_trace = (krabs::user_trace*)trace;
        unwrapped_trace->start();
    }
    catch (const std::exception& ex)
    {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API void krabs_stop_trace(
    krabs_status_ctx *status,
    krabs_user_trace *trace)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::user_trace* unwrapped_trace = (krabs::user_trace*)trace;
        unwrapped_trace->stop();
    }
    catch (const std::exception& ex)
    {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API void krabs_destroy_user_provider(
    krabs_user_provider *provider)
{
    if (provider) {
        delete ((krabs::provider<>*)provider);
    }
}

FLATKRABSETW_API void krabs_destroy_user_trace(
    krabs_user_trace *trace)
{
    if (trace) {
        delete ((krabs::user_trace*)trace);
    }
}

FLATKRABSETW_API void krabs_destroy_event_filter(
    krabs_filter *filter)
{
    if (filter) {
        delete ((krabs::event_filter*)filter);
    }
}