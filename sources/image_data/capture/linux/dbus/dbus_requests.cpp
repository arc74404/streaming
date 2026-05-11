#include "dbus_requests.hpp"

#include <condition_variable>
#include <mutex>

#include "message.hpp"

std::expected<dbus::ScreenSessionAnswer, std::runtime_error>
dbus::CreateScreenPickSession(DBusWrapper& bus)
{
    try
    {
        Message message;
        int result = message.setService("org.freedesktop.portal.Desktop")
                         .setObjectPath("/org/freedesktop/portal/desktop")
                         .setInterface("org.freedesktop.portal.ScreenCast")
                         .createMessage(bus, "CreateSession");

        if (result < 0)
            return std::unexpected(std::runtime_error("failed setup message"));

        result = message.openContainer('a', "{sv}");
        result = message.append("{sv}", "session_handle_token", "s",
                                "my_cool_session_123");
        message.closeContainer();

        auto&& call_answer = message.call<ScreenSessionAnswer>(bus);

        if (false == call_answer.has_value())
        {
            if (call_answer.error() == Message::AnswerStatus::FAILED)
            {
                return std::unexpected(
                    std::runtime_error("failed get answer after message call"));
            }
            else if (call_answer.error() == Message::AnswerStatus::END)
            {
                return std::unexpected(std::runtime_error("end of answer"));
            }
        }
        return call_answer.value();
    }
    catch (std::runtime_error& err)
    {
        return std::unexpected(err);
    }
    catch (std::exception& err)
    {
        return std::unexpected(std::runtime_error(err.what()));
    }
    catch (...)
    {
        return std::unexpected(
            std::runtime_error("CreateScreenPickSession unexcpected fail"));
    }
}

namespace
{

enum class UserStatus
{
    EMPTY,
    FAILED = 0,
    CANCEL = 1,
    SUCCESS
};

struct SessionHandleData
{
    UserStatus status = UserStatus::EMPTY;
    std::string result;
};

struct StartResponseData
{
    UserStatus status         = UserStatus::EMPTY;
    uint32_t pipewire_node_id = 0;
};
} // namespace

static int
match_callbac(sd_bus_message* message, void* userdata, sd_bus_error* ret_error)
{
    SessionHandleData* data = static_cast<SessionHandleData*>(userdata);

    uint32_t status;
    int r = sd_bus_message_read(message, "u", &status);
    if (r < 0)
    {
        data->status = UserStatus::FAILED;
        return 0;
    }

    if (status != 0)
    {
        data->status = UserStatus::CANCEL;
        return 0;
    }

    r = sd_bus_message_enter_container(message, 'a', "{sv}");
    if (r < 0)
    {
        data->status = UserStatus::FAILED;
        return 0;
    }

    while ((r = sd_bus_message_enter_container(message, 'e', "sv")) > 0)
    {
        const char* key;
        sd_bus_message_read(message, "s", &key);

        if (std::string_view(key) == "session_handle")
        {
            const char* handle;
            sd_bus_message_read(message, "v", "s", &handle);
            data->result = handle;
        }
        else
        {
            sd_bus_message_skip(message, "v");
        }

        sd_bus_message_exit_container(message);
    }

    sd_bus_message_exit_container(message);

    data->status = UserStatus::SUCCESS;
    return 0;
}

std::optional<std::string>
dbus::GetSessionHandle(DBusWrapper& bus, std::string_view request_handle)
{
    std::string match = "type='signal',"
                        "sender='org.freedesktop.portal.Desktop',"
                        "interface='org.freedesktop.portal.Request',"
                        "member='Response',"
                        "path='" +
                        std::string(request_handle) + "'";

    SessionHandleData sh_data;

    sd_bus_slot* raw_slot{};

    if (sd_bus_add_match(bus, &raw_slot, match.c_str(), match_callbac,
                         &sh_data) < 0)
    {
        return std::nullopt;
    }
    std::unique_ptr<sd_bus_slot, decltype(&sd_bus_slot_unref)> slot = {
        raw_slot, sd_bus_slot_unref};

    while (sh_data.status == UserStatus::EMPTY)
    {
        if (sd_bus_wait(bus, (uint64_t)-1) < 0) return std::nullopt;
        if (sd_bus_process(bus, nullptr) < 0) return std::nullopt;
    }
    if (sh_data.status != UserStatus::SUCCESS)
    {
        return std::nullopt;
    }
    return sh_data.result;
}

std::expected<dbus::SelectSourcesAnswer, std::runtime_error>
dbus::SelectSources(DBusWrapper& bus, std::string_view session_handle)
{
    try
    {
        Message message;
        int result = message.setService("org.freedesktop.portal.Desktop")
                         .setObjectPath("/org/freedesktop/portal/desktop")
                         .setInterface("org.freedesktop.portal.ScreenCast")
                         .createMessage(bus, "SelectSources");

        if (result < 0)
            return std::unexpected(std::runtime_error("failed setup message"));

        result = message.append("o", session_handle.data());

        if (message.openContainer('a', "{sv}") < 0)
            return std::unexpected(
                std::runtime_error("failed open message container"));

        result = 0;

        result += message.append("{sv}", "multiple", "b", false);
        result += message.append("{sv}", "types", "u", 2);
        result += message.append("{sv}", "cursor_mode", "u", 1);
        result +=
            message.append("{sv}", "handle_token", "s", "select_sources_123");

        if (result != 4)
            return std::unexpected(
                std::runtime_error("failed append to message container"));

        if (message.closeContainer() < 0)
            return std::unexpected(
                std::runtime_error("failed close message container"));

        auto&& call_answer = message.call<SelectSourcesAnswer>(bus);

        if (false == call_answer.has_value())
        {
            if (call_answer.error() == Message::AnswerStatus::FAILED)
            {
                return std::unexpected(
                    std::runtime_error("failed get answer after message call"));
            }
            else if (call_answer.error() == Message::AnswerStatus::END)
            {
                return std::unexpected(std::runtime_error("end of answer"));
            }
        }
        return call_answer.value();
    }
    catch (std::runtime_error& err)
    {
        return std::unexpected(err);
    }
    catch (std::exception& err)
    {
        return std::unexpected(std::runtime_error(err.what()));
    }
    catch (...)
    {
        return std::unexpected(
            std::runtime_error("SelectSources unexcpected fail"));
    }
}

#include <iostream>

static int
select_sources_callback(sd_bus_message* message,
                        void* userdata,
                        sd_bus_error* ret_error)
{
    SessionHandleData* data = static_cast<SessionHandleData*>(userdata);

    uint32_t status;
    int r = sd_bus_message_read(message, "u", &status);

    if (r < 0 || status != 0)
    {
        data->status = (status == 1) ? UserStatus::CANCEL : UserStatus::FAILED;
        return 0;
    }

    data->status = UserStatus::SUCCESS;
    return 0;
}

bool
dbus::WaitSourcesHandle(DBusWrapper& bus, std::string_view session_handle)
{
    std::string match = "type='signal',"
                        "sender='org.freedesktop.portal.Desktop',"
                        "interface='org.freedesktop.portal.Request',"
                        "member='Response',"
                        "path='" +
                        std::string(session_handle) + "'";

    SessionHandleData sh_data;

    sd_bus_slot* raw_slot{};

    if (sd_bus_add_match(bus, &raw_slot, match.c_str(), select_sources_callback,
                         &sh_data) < 0)
    {
        return false;
    }
    std::unique_ptr<sd_bus_slot, decltype(&sd_bus_slot_unref)> slot = {
        raw_slot, sd_bus_slot_unref};

    while (sh_data.status == UserStatus::EMPTY)
    {
        if (sd_bus_wait(bus, (uint64_t)-1) < 0) return false;
        if (sd_bus_process(bus, nullptr) < 0) return false;
    }
    if (sh_data.status != UserStatus::SUCCESS)
    {
        return false;
    }

    return true;
}

std::expected<dbus::StartAnswer, std::runtime_error>
dbus::Start(DBusWrapper& bus, std::string_view session_handle)
{
    try
    {
        Message message;
        int result = message.setService("org.freedesktop.portal.Desktop")
                         .setObjectPath("/org/freedesktop/portal/desktop")
                         .setInterface("org.freedesktop.portal.ScreenCast")
                         .createMessage(bus, "Start");

        if (result < 0)
            return std::unexpected(
                std::runtime_error("failed setup Start message"));

        message.append("o", session_handle.data());

        message.append("s", "");

        if (message.openContainer('a', "{sv}") < 0)
            return std::unexpected(
                std::runtime_error("failed open options container"));

        message.append("{sv}", "handle_token", "s", "start_call_777");

        if (message.closeContainer() < 0)
            return std::unexpected(
                std::runtime_error("failed close options container"));

        auto&& call_answer = message.call<StartAnswer>(bus);

        if (!call_answer.has_value())
        {
            return std::unexpected(
                std::runtime_error("failed call Start method"));
        }

        return call_answer.value();
    }
    catch (const std::exception& err)
    {
        return std::unexpected(std::runtime_error(err.what()));
    }
}
static int
start_response_callback(sd_bus_message* m,
                        void* userdata,
                        sd_bus_error* ret_error)
{
    auto* data = static_cast<StartResponseData*>(userdata);
    uint32_t response_status;

    if (sd_bus_message_read(m, "u", &response_status) < 0)
    {
        data->status = UserStatus::FAILED;
        return 0;
    }

    if (response_status != 0)
    {
        data->status =
            (response_status == 1) ? UserStatus::CANCEL : UserStatus::FAILED;
        return 0;
    }
    if (sd_bus_message_enter_container(m, 'a', "{sv}") < 0) return 0;

    const char* key;
    while (sd_bus_message_enter_container(m, 'e', "sv") > 0)
    {
        sd_bus_message_read(m, "s", &key);

        if (std::string_view(key) == "streams")
        {
            sd_bus_message_enter_container(m, 'v', "a(ua{sv})");
            sd_bus_message_enter_container(m, 'a', "(ua{sv})");

            if (sd_bus_message_enter_container(m, 'r', "ua{sv}") > 0)
            {
                sd_bus_message_read(m, "u", &data->pipewire_node_id);
                sd_bus_message_exit_container(m);
            }

            sd_bus_message_exit_container(m);
            sd_bus_message_exit_container(m);
        }
        else
        {
            sd_bus_message_skip(m, "v");
        }
        sd_bus_message_exit_container(m);
    }
    sd_bus_message_exit_container(m);

    data->status = UserStatus::SUCCESS;
    return 0;
}

std::optional<uint32_t>
dbus::WaitUser(DBusWrapper& bus, std::string_view start_handle)
{
    std::string match = "type='signal',"
                        "sender='org.freedesktop.portal.Desktop',"
                        "interface='org.freedesktop.portal.Request',"
                        "member='Response',"
                        "path='" +
                        std::string(start_handle) + "'";
    StartResponseData res_data;

    sd_bus_slot* raw_slot{};

    if (sd_bus_add_match(bus, &raw_slot, match.c_str(), start_response_callback,
                         &res_data) < 0)
    {
        return std::nullopt;
    }
    std::unique_ptr<sd_bus_slot, decltype(&sd_bus_slot_unref)> slot = {
        raw_slot, sd_bus_slot_unref};

    while (res_data.status == UserStatus::EMPTY)
    {
        if (sd_bus_wait(bus, (uint64_t)-1) < 0) break;
        if (sd_bus_process(bus, nullptr) < 0) break;
    }

    if (res_data.status == UserStatus::SUCCESS &&
        res_data.pipewire_node_id != 0)
    {
        return res_data.pipewire_node_id;
    }
    return std::nullopt;
}
