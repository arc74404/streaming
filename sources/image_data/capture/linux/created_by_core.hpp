#pragma once
#include <pipewire/pipewire.h>

#include <utility>
#include <functional>

#include "sync_func.hpp"

namespace stream::image::lin_impl
{
class CreatedByCore final
{
public:
    CreatedByCore(pw_thread_loop* loop) : m_loop(loop)
    {
    }

    template <typename Pred, typename... Args>
    decltype(auto) call(Pred&& pred, Args&&... args)
    {
        lock(m_loop);
        if constexpr (std::is_void_v<std::invoke_result_t<Pred, Args...>>)
        {
            std::invoke(std::forward<Pred>(pred), std::forward<Args>(args)...);
            unlock(m_loop);
        }
        else
        {
            auto result = std::invoke(std::forward<Pred>(pred),
                                      std::forward<Args>(args)...);
            unlock(m_loop);
            return result;
        }
    }

private:
    pw_thread_loop* m_loop;
};
} // namespace stream::image::lin_impl
