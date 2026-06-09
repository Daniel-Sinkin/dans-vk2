// src/dans/dev.hpp
#pragma once
// Internals
#include <dans/format.hpp>
// Externals
#include <dans/development_markers.hpp>
// StdLib
#include <cstdio>
#include <exception>
#include <format>
#include <ranges>
#include <utility>
//

namespace dans::dev
{
namespace r = std::ranges;
namespace rv = std::ranges::views;

struct PanicLocation
{
    const char* file;
    int line;
    const char* function;
};

namespace detail
{
inline void dump_current_exception_for_panic() noexcept
{
    const std::exception_ptr exception = std::current_exception();
    if (!exception)
    {
        return;
    }

    try
    {
        std::rethrow_exception(exception);
    }
    catch (const std::exception& error)
    {
        // NOLINTNEXTLINE(modernize-use-std-print): panic handler must not throw
        std::fprintf(stderr, "fatal: %s\n", error.what());
    }
    catch (...)  // NOLINT
    {
        // NOLINTNEXTLINE(modernize-use-std-print): panic handler must not throw
        std::fprintf(stderr, "fatal: non-std exception\n");
    }
}
}  // namespace detail

[[noreturn]] inline void panic(PanicLocation location, const char* message) noexcept
{
    // NOLINTNEXTLINE(modernize-use-std-print): panic handler must not throw
    std::fprintf(
        stderr,
        "fatal: %s at %s:%d in %s\n",
        message == nullptr ? "(null panic message)" : message,
        location.file,
        location.line,
        location.function
    );

    detail::dump_current_exception_for_panic();

    // NOLINTNEXTLINE(modernize-use-std-print): panic handler must not throw
    std::fflush(stderr);

    std::terminate();
}

template <typename... Args>
[[noreturn]] void panic_fmt(
    PanicLocation location,
    std::format_string<::dans::detail::format_arg_t<Args>...> fmt,
    Args&&... args
) noexcept
{
    try
    {
        panic(
            location,
            std::format(fmt, ::dans::detail::to_format_arg(std::forward<Args>(args))...).c_str()
        );
    }
    catch (...)  // NOLINT
    {
        panic(location, "(panic message formatting failed)");
    }
}

template <typename F>
class Defer
{
  public:
    explicit Defer(F fn, PanicLocation location) : fn_{std::move(fn)}, location_{location}
    {
    }

    ~Defer() noexcept
    {
        try
        {
            fn_();
        }
        catch (...)  // NOLINT
        {
            panic(location_, "exception thrown from DANS_DEFER cleanup");
        }
    }

    Defer(const Defer&) = delete;
    Defer(Defer&&) = delete;
    def operator=(const Defer&)->Defer& = delete;
    def operator=(Defer&&)->Defer& = delete;

  private:
    F fn_;
    PanicLocation location_;
};

template <typename F>
Defer(F, PanicLocation) -> Defer<F>;
}  // namespace dans::dev

#define DANS_DETAIL_DEFER_CONCAT_(a, b) a##b
#define DANS_DETAIL_DEFER_CONCAT(a, b) DANS_DETAIL_DEFER_CONCAT_(a, b)

#define DANS_PANIC(message)                                                                        \
    ::dans::dev::panic(::dans::dev::PanicLocation{__FILE__, __LINE__, __func__}, message)

#define DANS_PANIC_FMT(...)                                                                        \
    ::dans::dev::panic_fmt(::dans::dev::PanicLocation{__FILE__, __LINE__, __func__}, __VA_ARGS__)

#define DANS_DEFER(...)                                                                            \
    const ::dans::dev::Defer DANS_DETAIL_DEFER_CONCAT(dans_defer_, __LINE__)                       \
    {                                                                                              \
        __VA_ARGS__, ::dans::dev::PanicLocation                                                    \
        {                                                                                          \
            __FILE__, __LINE__, __func__                                                           \
        }                                                                                          \
    }
