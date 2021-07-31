#pragma once

#include <cstdint>
#include <iterator>
#include <random>
#include <stdexcept>

namespace Melee
{
	inline float			NormalizedRandom()
	{
		static std::mt19937								random(std::random_device{}());
		static std::uniform_real_distribution<float>	randomDistribution(0.0f, 1.0f);

		return randomDistribution(random);
	}

	template <typename T>
	constexpr inline T		LinearInterpolate(T min, T max, float val) noexcept
	{
		if (min > max)
			return min;

		if (val <= 0.0f)
			return min;
		else if (val >= 1.0f)
			return max;
		else
			return static_cast<T>(min + (val * (max - min)));
	}

	template <typename T>
	constexpr inline auto   LinearInterpolateRandom(T min, T max) noexcept
	{
		return LinearInterpolate(min, max, NormalizedRandom());
	}

	constexpr inline auto	LinearInterpolateIndex(size_t size, float val) noexcept
	{
		if (!size)
			return size_t(0);

		return LinearInterpolate<size_t>(0, size - 1, val);
	}

	template<typename T>
	constexpr inline auto	LinearInterpolateIndex(const T& container, float val) noexcept
	{
		return LinearInterpolateIndex(std::size(container), val);
	}
}
