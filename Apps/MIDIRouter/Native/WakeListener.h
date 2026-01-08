#pragma once

#include <functional>
#include <memory>

namespace Native
{
using Callback = std::function<void()>;

class WakeListener
{
public:
    WakeListener(Callback callback);
    ~WakeListener();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
} // namespace Native
