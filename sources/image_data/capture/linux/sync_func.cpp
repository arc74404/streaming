#include "sync_func.hpp"

namespace stream::image
{
void
lin_impl::lock(pw_thread_loop* loop)
{
    pw_thread_loop_lock(loop);
}
void
lin_impl::unlock(pw_thread_loop* loop)
{
    pw_thread_loop_unlock(loop);
}
void
lin_impl::wait(pw_thread_loop* loop)
{
    pw_thread_loop_wait(loop);
}
void
lin_impl::signal(pw_thread_loop* loop)
{
    pw_thread_loop_signal(loop, false);
}

} // namespace stream::image
