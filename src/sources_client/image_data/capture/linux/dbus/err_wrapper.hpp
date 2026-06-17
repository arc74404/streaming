#pragma once

#include <systemd/sd-bus.h>

namespace dbus
{
class ErrWrapper final
{
public:
    ErrWrapper(const ErrWrapper&) = delete;
    ErrWrapper(ErrWrapper&&)      = delete;

    ErrWrapper& operator=(const ErrWrapper&) = delete;
    ErrWrapper& operator=(ErrWrapper&&)      = delete;

    ErrWrapper() = default;

    ~ErrWrapper() noexcept
    {
        sd_bus_error_free(&m_err);
    }

    operator sd_bus_error*()
    {
        return &m_err;
    }

private:
    sd_bus_error m_err = SD_BUS_ERROR_NULL;
};
} // namespace dbus
