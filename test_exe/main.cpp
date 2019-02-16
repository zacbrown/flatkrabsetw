#include <Windows.h>
#include <stdio.h>
#include <flatkrabsetw.h>

void handle_event(const EVENT_RECORD &rec)
{
    printf("event processed - eid: %d, pid: %d, context:\n",
        rec.EventHeader.EventDescriptor.Id,
        rec.EventHeader.ProcessId);

    krabs_status_ctx status;
    auto prop_name = krabs_create_property_name(&status, L"ContextInfo");

    if (status.status != krabs_success) {
        printf("error: %s\n", status.msg);
        return;
    }

    auto schema = krabs_get_event_schema(&status, rec);
    if (status.status != krabs_success) {
        printf("error: %s\n", status.msg);
        return;
    }

    auto parser = krabs_get_event_parser(&status, schema);
    if (status.status != krabs_success) {
        printf("error: %s\n", status.msg);
        return;
    }

    auto context = krabs_get_string_property_from_parser(
        &status,
        parser,
        prop_name,
        krabs_string_type::std_wstring);
    if (status.status != krabs_success) {
        printf("error: %s\n", status.msg);
        return;
    }

    wprintf(L"%ls\n", context);

    if (context) {
        delete context;
    }
    krabs_destroy_event_parser(parser);
    krabs_destroy_event_schema(schema);
    krabs_destroy_property_name(prop_name);

    printf("\n");
}

int main(void)
{
    krabs_status_ctx status;
    auto trace = krabs_create_user_trace(&status, L"my sweet sweet trace");

    if (status.status != krabs_success)
    {
        printf("error: %s\n", status.msg);
        return 1;
    }

    auto provider = krabs_create_user_provider(
        &status,
        L"Microsoft-Windows-PowerShell",
        0xf0010000000003ff,
        0);

    if (status.status != krabs_success)
    {
        printf("error: %s\n", status.msg);
        return 1;
    }

    USHORT event_id = 7937;
    auto filter = krabs_create_filter_for_event_ids(&status, &event_id, 1);
    if (status.status != krabs_success) {
        printf("error: %s\n", status.msg);
        return 1;
    }

    krabs_add_callback_to_event_filter(&status, filter, handle_event);

    if (status.status != krabs_success)
    {
        printf("error: %s\n", status.msg);
        return 1;
    }

    krabs_add_event_filter_to_user_provider(&status, provider, filter);

    if (status.status != krabs_success) {
        printf("error: %s\n", status.msg);
        return 1;
    }

    krabs_enable_user_provider(&status, trace, provider);

    if (status.status != krabs_success)
    {
        printf("error: %s\n", status.msg);
        return 1;
    }

    krabs_start_trace(&status, trace);

    if (status.status != krabs_success)
    {
        printf("error: %s\n", status.msg);
        return 1;
    }
}