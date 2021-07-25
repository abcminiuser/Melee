#pragma once

#include <cstdint>
#include <iterator>

namespace Melee
{
	template <typename T>
	constexpr inline T		LinearInterpolate(T min, T max, float val)
	{
		if (val <= 0.0f)
			return min;
		else if (val >= 1.0f)
			return max;
		else
			return static_cast<T>(min + (val * (max - min)));
	}

	constexpr inline auto	LinearInterpolateIndex(size_t size, float val)
	{
		return LinearInterpolate<size_t>(0, size - 1, val);
	}

	template<typename T>
	constexpr inline auto	LinearInterpolateIndex(const T& container, float val)
	{
		return LinearInterpolateIndex(std::size(container), val);
	}
}
