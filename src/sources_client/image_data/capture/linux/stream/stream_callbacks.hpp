#pragma once

#include <pipewire/pipewire.h>

#include <cstdint>

namespace stream::image::lin_impl::pw_stream_callbacks
{
pw_stream_events
standartCallbacks();

void
on_process(void* user_data);

void
on_param_changed(void* user_data, uint32_t id, const struct spa_pod* param);

void
on_state_changed(void* user_data,
                 enum pw_stream_state old,
                 enum pw_stream_state new_state,
                 const char* error_message);
} // namespace stream::image::lin_impl::pw_stream_callbacks
