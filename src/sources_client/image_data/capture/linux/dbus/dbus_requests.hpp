#pragma once

#include <systemd/sd-bus.h>

#include <cstdint>
#include <expected>
#include <stdexcept>
#include <string>

#include "answers.hpp"
#include "bus_wrapper.hpp"
#include "message.hpp"

namespace dbus
{

std::expected<ScreenSessionAnswer, std::runtime_error>
CreateScreenPickSession(DBusWrapper& bus);

std::optional<std::string>
GetSessionHandle(DBusWrapper& bus, std::string_view request_handle);

std::expected<SelectSourcesAnswer, std::runtime_error>
SelectSources(DBusWrapper& bus, std::string_view session_handle);

bool
WaitSourcesHandle(DBusWrapper& bus, std::string_view session_handle);

std::expected<dbus::StartAnswer, std::runtime_error>
Start(DBusWrapper& bus, std::string_view session_handle);

std::optional<uint32_t>
WaitUser(DBusWrapper& bus, std::string_view start_handle);
} // namespace dbus
