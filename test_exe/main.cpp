#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <flatkrabsetw.h>

#pragma comment(lib, "Ws2_32.lib")

void handle_event(const EVENT_RECORD *rec)
{
    auto eid = rec->EventHeader.EventDescriptor.Id;

    krabs_status_ctx status;

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

    // Get the size.
    uint32_t pid = 0;
    {
        auto prop_name = krabs_create_property_name(&status, L"PID");
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        pid = krabs_get_u32_property_from_parser(
            &status,
            parser,
            prop_name);
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        krabs_destroy_property_name(prop_name);
    }

    // Get the size.
    uint32_t size = 0;
    {
        auto prop_name = krabs_create_property_name(&status, L"size");
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        size = krabs_get_u32_property_from_parser(
            &status,
            parser,
            prop_name);
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        krabs_destroy_property_name(prop_name);
    }

    // Get the Dport.
    uint16_t dport = 0;
    {
        auto prop_name = krabs_create_property_name(&status, L"dport");
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        dport = krabs_get_u16_property_from_parser(
            &status,
            parser,
            prop_name);
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        krabs_destroy_property_name(prop_name);
    }

    // Get the Dport.
    uint16_t sport = 0;
    {
        auto prop_name = krabs_create_property_name(&status, L"sport");
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        sport = krabs_get_u16_property_from_parser(
            &status,
            parser,
            prop_name);
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        krabs_destroy_property_name(prop_name);
    }

    // Get the Daddr.
    krabs_ip_address *daddr = nullptr;
    {
        auto prop_name = krabs_create_property_name(&status, L"daddr");
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        daddr = krabs_get_ip_addr_property_from_parser(
            &status,
            parser,
            prop_name);
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        krabs_destroy_property_name(prop_name);
    }

    // Get the Saddr.
    krabs_ip_address *saddr = nullptr;
    {
        auto prop_name = krabs_create_property_name(&status, L"saddr");
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        saddr = krabs_get_ip_addr_property_from_parser(
            &status,
            parser,
            prop_name);
        if (status.status != krabs_success) {
            printf("error: %s\n", status.msg);
            return;
        }

        krabs_destroy_property_name(prop_name);
    }

    switch (eid) {
    case 10:
        printf("PID: %d - TCPv4 (%d): ", pid, eid);
        break;
    case 26:
        printf("PID: %d - TCPv6 (%d): ", pid, eid);
        break;
    case 42:
        printf("PID: %d - UDPv4 (%d): ", pid, eid);
        break;
    case 58:
        printf("PID: %d - UDPv6 (%d): ", pid, eid);
        break;
    default:
        return;
    }

    char saddr_str[48] = { 0 };
    if (saddr->is_ipv6) {
        inet_ntop(AF_INET6, &saddr->v6, &saddr_str[0], ARRAYSIZE(saddr_str));
    } else {
        inet_ntop(AF_INET, &saddr->v4, &saddr_str[0], ARRAYSIZE(saddr_str));
    }

    char daddr_str[48] = { 0 };
    if (daddr->is_ipv6) {
        inet_ntop(AF_INET6, &daddr->v6, &daddr_str[0], ARRAYSIZE(saddr_str));
    } else {
        inet_ntop(AF_INET, &daddr->v4, &daddr_str[0], ARRAYSIZE(saddr_str));
    }

    printf("%d bytes transmitted from %s:%d to %s:%d",
        size,
        saddr_str,
        sport,
        daddr_str,
        dport);

    if (daddr) {
        delete daddr;
    }
    if (saddr) {
        delete saddr;
    }
    krabs_destroy_event_parser(parser);
    krabs_destroy_event_schema(schema);

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
        L"Microsoft-Windows-Kernel-Network",
        0x8000000000000000,
        0);

    if (status.status != krabs_success)
    {
        printf("error: %s\n", status.msg);
        return 1;
    }

    USHORT event_ids[] = {
        10,
        26,
        42,
        58
    };
    auto filter = krabs_create_filter_for_event_ids(&status, &event_ids[0], ARRAYSIZE(event_ids));
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