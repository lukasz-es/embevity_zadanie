#include "utest.h"
#include "accelerometer.h"
#include "freefall.h"

UTEST_STATE();

FreeFallDataCache cache;
AccelData data;

UTEST(cache, init) 
{
  freefall_init(&cache, 3, 1.0);
  ASSERT_EQ(0, cache.already_triggered);
  ASSERT_EQ(3, cache.min_samples_no);
  ASSERT_EQ(1.0, cache.threshold);
  ASSERT_EQ(0, cache.samples_buffer);
  ASSERT_NE(0, cache.samples_buffer_mask);
}

/* All axes above trigger level */
UTEST(freefall, no_trigger)
{
	freefall_init(&cache, 3, 1.0);
	
	data.accel.x = 1.5;
	data.accel.y = 1.5;
	data.accel.z = 1.5;
	
	int retval, i;
	
	for (i=0; i<10; i++)
	{
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
	}
}

/* All axes below trigger level */
UTEST(freefall, trigger)
{
	freefall_init(&cache, 3, 1.0);
	
	data.accel.x = 0.5;
	data.accel.y = 0.5;
	data.accel.z = 0.5;
	
	int retval;
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(0, retval);
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(0, retval);
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(1, retval);
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(1, retval);
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(1, retval);
}

/* One axis above trigger level */
UTEST(freefall, 1_above)
{
	freefall_init(&cache, 3, 1.0);
	
	data.accel.x = 1.5;
	data.accel.y = 0.5;
	data.accel.z = 0.5;
	
	int retval, i;
	
	for (i=0; i<10; i++)
	{
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
	}
}

/* Two axes above trigger level */
UTEST(freefall, 2_above)
{
	freefall_init(&cache, 3, 1.0);
	
	data.accel.x = 1.5;
	data.accel.y = 0.5;
	data.accel.z = 1.5;
	
	int retval, i;
	
	for (i=0; i<10; i++)
	{
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
	}
}

/* Triggered fall is sustained */
UTEST(freefall, sustain)
{
	freefall_init(&cache, 3, 1.0);
	
	data.accel.x = 0.5;
	data.accel.y = 0.5;
	data.accel.z = 0.5;
	
	int retval, i;
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(0, retval);
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(0, retval);
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(1, retval);
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(1, retval);
	retval = freefall_check(&cache, &data);
	ASSERT_EQ(1, retval);
	
	data.accel.x = 1.5;
	data.accel.y = 1.5;
	data.accel.z = 1.5;
	
	for (i=0; i<5; i++)
	{
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(1, retval);
	}
}

/* Erratic data won't trigger detection */
UTEST(freefall, erratic)
{
	int retval, i;
		
	freefall_init(&cache, 3, 1.0);
	
	for (i=0; i<5; i++)
	{
		data.accel.x = 0.5;
		data.accel.y = 0.5;
		data.accel.z = 0.5;
		
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
		
		data.accel.x = 1.5;
		data.accel.y = 1.5;
		data.accel.z = 1.5;
		
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
	}
}

/* Test with minimum 63 matching samples required */
UTEST(freefall, large_samples_no_63) 
{
	int retval, i;
	
	freefall_init(&cache, 63, 1.0);

	data.accel.x = 0.5;
	data.accel.y = 0.5;
	data.accel.z = 0.5;
	
	for (i=0; i<62; i++)
	{
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
	}
	
	for (i=0; i<5; i++)
	{
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(1, retval);
	}
}

/* Test with minimum 64 matching samples required */
UTEST(freefall, large_samples_no_64) 
{
	int retval, i;
	
	freefall_init(&cache, 64, 1.0);

	data.accel.x = 0.5;
	data.accel.y = 0.5;
	data.accel.z = 0.5;
	
	for (i=0; i<63; i++)
	{
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(0, retval);
	}
	
	for (i=0; i<5; i++)
	{
		retval = freefall_check(&cache, &data);
		ASSERT_EQ(1, retval);
	}
}

int main(int argc, const char *const argv[])
{
  return utest_main(argc, argv);
}