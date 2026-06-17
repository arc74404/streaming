#pragma once

#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

namespace stream
{

template <typename T, typename... ParamsTypes>
concept IsOneOfPack =
    (std::same_as<T, std::remove_cvref_t<ParamsTypes>> || ...);

template <typename... ParamsTypes>
class GlobalParams
{
public:
    using iterator =
        std::unordered_map<std::string,
                           std::variant<std::unique_ptr<ParamsTypes>...>>;

    template <IsOneOfPack<ParamsTypes...> T>
    auto addParam(const std::string& key, T&& param)
    {
        std::variant<std::unique_ptr<ParamsTypes>...> tmp =
            std::make_unique<T>(param);

        return m_params.try_emplace(key, std::move(tmp));
    }

    template <IsOneOfPack<ParamsTypes...> T>
    const T& getParam(const std::string& key) const noexcept
    {
        return m_params.at(key);
    }
    template <IsOneOfPack<ParamsTypes...> T>
    T& getParam(const std::string& key) noexcept
    {
        return *std::get<std::unique_ptr<T>>(m_params.at(key));
    }

private:
    std::unordered_map<std::string,
                       std::variant<std::unique_ptr<ParamsTypes>...>>
        m_params;
};
} // namespace stream
