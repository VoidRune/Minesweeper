#pragma once
#include <cstdint>

class Random
{
public:
	static uint32_t seed;
	static int32_t Range(int32_t min, int32_t max)
	{
		return Lehmer32() % (max - min + 1) + min;
	}
	static bool Chance(float chance)
	{
		return (Range(0, 100000) / 100000.0f) <= chance;
	}
	/* https://www.youtube.com/watch?v=ZZY9YE7rZJw */
	static uint32_t Lehmer32()
	{
		seed += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)seed * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;
	}
};