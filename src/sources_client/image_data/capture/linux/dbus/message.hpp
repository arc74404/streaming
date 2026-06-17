#pragma once

#include <systemd/sd-bus.h>

#include <expected>
#include <optional>
#include <string>
#include <string_view>

#include "answers.hpp"
#include "err_wrapper.hpp"

namespace dbus
{
template <typename T>
concept Answerable =
    requires(T& t) { t.fill(std::declval<sd_bus_message*>()); };

class Message final
{
public:
    Message(Message&&)                 = delete;
    Message(const Message&)            = delete;
    Message& operator=(Message&&)      = delete;
    Message& operator=(const Message&) = delete;

    Message();

    ~Message() noexcept;

    Message& setService(std::string_view s);

    Message& setObjectPath(std::string_view op);

    Message& setInterface(std::string_view i);

    int createMessage(sd_bus* bus, std::string_view method);

    int openContainer(char c_t, std::string_view el_t);

    int closeContainer();

    template <typename... Args>
    int append(Args&&... args)
    {
        return sd_bus_message_append(m_message, std::forward<Args>(args)...);
    }

    enum class AnswerStatus
    {
        FAILED = 0,
        END    = 1
    };

    operator sd_bus_message*() const;

    template <Answerable Answer>
    std::expected<Answer, AnswerStatus> call(sd_bus* bus)
    {
        Message reply;

        ErrWrapper error;
        if (sd_bus_call(bus, m_message, 0, error, &reply.m_message) < 0)
        {
            return std::unexpected(AnswerStatus::FAILED);
        }
        Answer answer;
        int result = answer.fill(reply);
        if (result < 0)
        {
            return std::unexpected(AnswerStatus::FAILED);
        }
        else if (result == 0)
        {
            return std::unexpected(AnswerStatus::END);
        }
        return answer;
    }

private:
    sd_bus_message* m_message;
    std::string m_service;
    std::string m_object_path;
    std::string m_interface;
};
} // namespace dbus
