
#pragma once

#include <time.h>

namespace basefun {
#define ONE_DAY_SECOND (60*60*24)

	int GetSecondNowToSomeTime(int second)
	{
		int n_diff = time(NULL) % ONE_DAY_SECOND;
		if (n_diff < second) {
			return second - n_diff;
		}

		return ONE_DAY_SECOND + second - n_diff;
	}

	int GetSecondSomeTimeToSomeTime(int some_time, int second)
	{
		int n_diff = some_time % ONE_DAY_SECOND;
		if (n_diff < second) {
			return second - n_diff;
		}

		return ONE_DAY_SECOND + second - n_diff;
	}


}


