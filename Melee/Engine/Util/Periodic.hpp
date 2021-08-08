#pragma once

#include <cstdint>

class Periodic final
{
public:
    explicit    Periodic(uint32_t interval, uint32_t maxValue = 0, bool startReady = false) noexcept
        : m_interval(interval)
        , m_maxValue(maxValue)
        , m_elapsed(startReady ? interval : 0)
    {}

    void        setInterval(uint32_t interval) noexcept
    {
        m_interval = interval;
    }

    void        setMaxElapsed(uint32_t maxValue) noexcept
    {
        m_maxValue = maxValue;
    }

    void        reset() noexcept
    {
        m_elapsed = 0;
    }

    void        add(uint32_t ms) noexcept
    {
        m_elapsed += ms;
        if (m_maxValue)
            m_elapsed = std::min(m_elapsed, m_maxValue);
    }

    bool        expired() noexcept
    {
        if (m_elapsed < m_interval)
            return false;

        m_elapsed -= m_interval;
        return true;
    }

private:
    uint32_t    m_interval = 0;
    uint32_t    m_maxValue = 0;
    uint32_t    m_elapsed = 0;
};
