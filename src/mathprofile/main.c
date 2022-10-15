#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "crtime.h"

const int NUM_ITERATIONS = 1000000000;

float int32_test()
{
	uint64_t start = GetTicks();

	for (int i = 0; i < NUM_ITERATIONS; i++)
	{
		volatile int32_t add = 123 + 654;
		volatile int32_t sub = 123 - 654;
		volatile int32_t mul = 123 * 654;
		volatile int32_t div = 123 / 654;
	}

	float time = GetTimeInMS(GetTicks() - start);

	return GetTimeInMS(GetTicks() - start);
}

float int64_test()
{
	uint64_t start = GetTicks();

	for (int i = 0; i < NUM_ITERATIONS; i++)
	{
		volatile int64_t add = 123 + 654;
		volatile int64_t sub = 123 - 654;
		volatile int64_t mul = 123 * 654;
		volatile int64_t div = 123 / 654;
	}

	float time = GetTimeInMS(GetTicks() - start);

	return GetTimeInMS(GetTicks() - start);
}

float float_test()
{
	uint64_t start = GetTicks();

	for (int i = 0; i < NUM_ITERATIONS; i++)
	{
		volatile float add = 123 + 654;
		volatile float sub = 123 - 654;
		volatile float mul = 123 * 654;
		volatile float div = 123 / 654;
	}

	float time = GetTimeInMS(GetTicks() - start);

	return GetTimeInMS(GetTicks() - start);
}

float double_test()
{
	uint64_t start = GetTicks();

	for (int i = 0; i < NUM_ITERATIONS; i++)
	{
		volatile double add = 123 + 654;
		volatile double sub = 123 - 654;
		volatile double mul = 123 * 654;
		volatile double div = 123 / 654;
	}

	float time = GetTimeInMS(GetTicks() - start);

	return GetTimeInMS(GetTicks() - start);
}

int main(int argc, char* argv)
{
	printf("Starting tests...\n\n");

	printf("int32_t time:\t%f ms\n", int32_test());
	printf("int64_t time:\t%f ms\n", int64_test());
	printf("float time:\t%f ms\n", float_test());
	printf("double time:\t%f ms\n", double_test());

	printf("\nTests complete\n");
	getchar();

	return EXIT_SUCCESS;
}