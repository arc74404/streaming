#pragma once

#include <systemd/sd-bus.h>

namespace dbus
{
struct ScreenSessionAnswer
{
    int fill(sd_bus_message* reply)
    {
        const char* request_handle;
        int ret          = sd_bus_message_read(reply, "o", &request_handle);
        m_request_handle = request_handle;
        return ret;
    }
    std::string m_request_handle;
};

struct SelectSourcesAnswer
{
    int fill(sd_bus_message* reply)
    {
        const char* sources_handle_address;
        int ret = sd_bus_message_read(reply, "o", &sources_handle_address);
        m_sources_handle_address = sources_handle_address;
        return ret;
    }
    std::string m_sources_handle_address;
};

struct StartAnswer
{
    int fill(sd_bus_message* reply)
    {
        const char* start_result;
        int ret        = sd_bus_message_read(reply, "o", &start_result);
        m_start_result = start_result;
        return ret;
    }
    std::string m_start_result;
};

} // namespace dbus
