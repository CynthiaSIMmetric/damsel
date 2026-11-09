#pragma once

#include <format>
#include <print>
#include <utility>
namespace obrc
{
    template <typename... Args>
    void log([[maybe_unused]] std::format_string<Args...> fmt,
             [[maybe_unused]] Args && ...args)
    {
#ifdef DEBUG
        std::println(fmt, std::forward<Args>(args)...);
#endif
        return;
    }
} // namespace 1brc
