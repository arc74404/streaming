#pragma once

#include <systemd/sd-bus.h>

#include <iostream>
#include <stdexcept>

namespace dbus
{
class DBusWrapper final
{
public:
    DBusWrapper(const DBusWrapper&) = delete;
    DBusWrapper(DBusWrapper&&)      = delete;

    DBusWrapper& operator=(const DBusWrapper&) = delete;
    DBusWrapper& operator=(DBusWrapper&&)      = delete;

    DBusWrapper()
    {
        if (sd_bus_open_user(&m_bus) < 0)
        {
            throw std::runtime_error("failed bus_open_user");
        }
    }
    ~DBusWrapper() noexcept
    {
        sd_bus_flush_close_unref(m_bus);
    }

    operator sd_bus*()
    {
        return m_bus;
    }

private:
    sd_bus* m_bus;
};
} // namespace dbus
