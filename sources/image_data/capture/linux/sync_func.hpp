#pragma once
#include <pipewire/pipewire.h>

namespace stream::image::lin_impl
{
void
lock(pw_thread_loop*);
void
unlock(pw_thread_loop*);
void
wait(pw_thread_loop*);
void
signal(pw_thread_loop*);
} // namespace stream::image::lin_impl
