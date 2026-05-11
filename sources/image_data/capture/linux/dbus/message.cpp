#include "message.hpp"

namespace dbus
{
Message::Message() : m_message(nullptr)
{
}
Message::~Message() noexcept
{
    sd_bus_message_unref(m_message);
}

Message&
Message::setService(std::string_view s)
{
    m_service = s;
    return *this;
}
Message&
Message::setObjectPath(std::string_view op)
{
    m_object_path = op;
    return *this;
}
Message&
Message::setInterface(std::string_view i)
{
    m_interface = i;
    return *this;
}
int
Message::createMessage(sd_bus* bus, std::string_view method)
{
    return sd_bus_message_new_method_call(bus, &m_message, m_service.data(),
                                          m_object_path.data(),
                                          m_interface.data(), method.data());
}
int
Message::openContainer(char c_t, std::string_view el_t)
{
    return sd_bus_message_open_container(m_message, c_t, el_t.data());
}
int
Message::closeContainer()
{
    return sd_bus_message_close_container(m_message);
}

Message::
operator sd_bus_message*() const
{
    return m_message;
}
} // namespace dbus
