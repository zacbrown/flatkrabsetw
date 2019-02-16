
#include "stdafx.h"
#include <set>
#include <string>
#include "flatkrabsetw.h"
#include <krabs.hpp>

void throw_if_bad_alloc(void* ptr) {
    if (ptr == nullptr) {
        throw std::bad_alloc();
    }
}

// Exported Functions
FLATKRABSETW_API krabs_property_name* krabs_create_property_name(
    krabs_status_ctx *status,
    const wchar_t *property_name)
{
    ZeroMemory(status, sizeof krabs_status_ctx);
    std::wstring *name = nullptr;

    try {
        name = new std::wstring(property_name);
        throw_if_bad_alloc(name);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }

    return (krabs_property_name*)name;
}

FLATKRABSETW_API krabs_user_trace* krabs_create_user_trace(
    krabs_status_ctx *status,
    const wchar_t *const name)
{
    ZeroMemory(status, sizeof krabs_status_ctx);
    krabs::user_trace *trace = nullptr;

    try {
        trace = new krabs::user_trace(name);
        throw_if_bad_alloc(trace);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }

    return (krabs_user_trace*)trace;
}

FLATKRABSETW_API krabs_user_provider* krabs_create_user_provider(
    krabs_status_ctx *status,
    const wchar_t *const provider_name,
    ULONGLONG any_flags,
    ULONGLONG all_flags)
{
    ZeroMemory(status, sizeof krabs_status_ctx);
    krabs::provider<> *provider = nullptr;

    try {
        provider = new krabs::provider<>(provider_name);
        throw_if_bad_alloc(provider);
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
    krabs_user_trace *const trace,
    krabs_user_provider *const provider)
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
    const USHORT *const event_ids,
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
        throw_if_bad_alloc(filter);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }

    return (krabs_filter*)filter;
}

FLATKRABSETW_API void krabs_add_event_callback_to_user_provider(
    krabs_status_ctx *status,
    krabs_user_provider *const provider,
    krabs_callback callback)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::provider<> *unwrapped_provider = (krabs::provider<>*)provider;
        auto wrapper_callback = [callback](const EVENT_RECORD &rec) {
            callback(&rec);
        };
        unwrapped_provider->add_on_event_callback(wrapper_callback);
    }
    catch (const std::exception& ex)
    {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API void krabs_add_callback_to_event_filter(
    krabs_status_ctx *status,
    krabs_filter *const filter,
    krabs_callback callback)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::event_filter *unwrapped_filter = (krabs::event_filter*)filter;
        unwrapped_filter->add_on_event_callback([callback](const EVENT_RECORD &rec) {
            callback(&rec);
        });
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API void krabs_add_event_filter_to_user_provider(
    krabs_status_ctx *status,
    krabs_user_provider *const provider,
    krabs_filter *const filter)
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
    krabs_user_trace *const trace)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::user_trace *unwrapped_trace = (krabs::user_trace*)trace;
        unwrapped_trace->start();
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API void krabs_stop_trace(
    krabs_status_ctx *status,
    krabs_user_trace *const trace)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    try {
        krabs::user_trace* unwrapped_trace = (krabs::user_trace*)trace;
        unwrapped_trace->stop();
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }
}

FLATKRABSETW_API krabs_event_schema* krabs_get_event_schema(
    krabs_status_ctx *status,
    const EVENT_RECORD *rec)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    krabs::schema *schema = nullptr;

    try {
        schema = new krabs::schema(*rec);
        throw_if_bad_alloc(schema);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }

    return (krabs_event_schema*)schema;
}

FLATKRABSETW_API krabs_event_parser* krabs_get_event_parser(
    krabs_status_ctx *status,
    krabs_event_schema *const schema)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    krabs::parser *parser = nullptr;

    try {
        krabs::schema *unwrapped_schema = (krabs::schema*)schema;
        parser = new krabs::parser(*unwrapped_schema);
        throw_if_bad_alloc(parser);
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }

    return (krabs_event_parser*)parser;
}

/// Schema fetching functions
#pragma region Schema
std::wstring skinny_to_wide(const std::string &str) {
    if (str.empty()) {
        return std::wstring();
    }

    size_t chars_needed = ::MultiByteToWideChar(CP_UTF8, 0,
        str.data(), (int)str.size(), NULL, 0);
    if (chars_needed == 0) {
        throw std::runtime_error("Failed converting UTF-8 string to UTF-16");
    }

    std::vector<wchar_t> buffer(chars_needed);
    int chars_converted = ::MultiByteToWideChar(CP_UTF8, 0,
        str.data(), (int)str.size(), &buffer[0], (int)buffer.size());
    if (chars_converted == 0) {
        throw std::runtime_error("Failed converting UTF-8 string to UTF-16");
    }

    return std::wstring(&buffer[0], chars_converted);
}

FLATKRABSETW_API wchar_t* krabs_get_string_property_from_parser(
    krabs_status_ctx *status,
    krabs_event_parser *const parser,
    krabs_property_name *const property_name,
    krabs_string_type string_type)
{
    ZeroMemory(status, sizeof krabs_status_ctx);

    wchar_t *ret = nullptr;

    try {
        krabs::parser *unwrapped_parser = (krabs::parser*)parser;
        const std::wstring* unwrapped_property_name = (const std::wstring*)property_name;

        switch (string_type) {
        case krabs_string_type::std_string:
        {
            std::string skinny = unwrapped_parser->parse<std::string>(*unwrapped_property_name);
            std::wstring wide = skinny_to_wide(skinny);
            size_t alloc_size = wide.length() + 1;
            ret = new wchar_t[alloc_size];
            throw_if_bad_alloc(ret);
            wcscpy_s(ret, alloc_size, wide.c_str());
            break;
        }
        case krabs_string_type::std_wstring:
        {
            std::wstring wide = unwrapped_parser->parse<std::wstring>(*unwrapped_property_name);
            size_t alloc_size = wide.length() + 1;
            ret = new wchar_t[alloc_size];
            throw_if_bad_alloc(ret);
            wcscpy_s(ret, alloc_size, wide.c_str());
            break;
        }
        case krabs_string_type::counted_wide_string:
        {
            auto parsed = unwrapped_parser->parse<const krabs::counted_string*>(*unwrapped_property_name);
            auto alloc_size = parsed->length() + 1;
            ret = new wchar_t[alloc_size];
            throw_if_bad_alloc(ret);
            wcsncpy_s(ret, alloc_size, parsed->string(), alloc_size - 1);
            break;
        }
        }
    }
    catch (const std::exception& ex) {
        strcpy_s(status->msg, ARRAYSIZE(status->msg), ex.what());
        status->status = krabs_error_unknown_error;
    }

    return ret;
}
#pragma endregion

#pragma region DestroyFunctions

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

FLATKRABSETW_API void krabs_destroy_event_schema(
    krabs_event_schema *schema)
{
    if (schema) {
        delete ((krabs::schema*)schema);
    }
}

FLATKRABSETW_API void krabs_destroy_event_parser(
    krabs_event_parser *parser)
{
    if (parser) {
        delete ((krabs::parser*)parser);
    }
}

FLATKRABSETW_API void krabs_destroy_property_name(
    krabs_property_name *property_name)
{
    if (property_name) {
        delete ((std::wstring*)property_name);
    }
}

#pragma endregion
