#pragma once

#include <cstdint>

class Periodic final
{
public:
    explicit    Periodic(uint32_t interval, uint32_t maxValue = 0)
        : m_interval(interval)
        , m_maxValue(maxValue)
    {}

    void        setInterval(uint32_t interval)
    {
        m_interval = interval;
    }

    void        setMaxElapsed(uint32_t maxValue)
    {
        m_maxValue = maxValue;
    }

    void        reset()
    {
        m_elapsed = 0;
    }

    void        add(uint32_t ms)
    {
        m_elapsed += ms;
        if (m_maxValue && m_elapsed > m_maxValue)
            m_elapsed = m_maxValue;
    }

    bool        expired()
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
