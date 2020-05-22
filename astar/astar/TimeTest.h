#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "Time.h"

class TimeTest {
public:

	void SetTime(ITime* time) {
		this->time = time;
	}
	bool Func1() {
		auto epoch = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
		auto date = boost::posix_time::time_from_string("2020-05-01 00:00:00.000");
		// 2020年5月1日以前なら正常
		auto result = time->Millisecond() < (date - epoch).total_milliseconds();
		return result;
	}
	bool Func2() {
		// 現在時刻(ptime)から時分秒を取得
		auto day = time->Now().time_of_day();
		// 午前なら正常、午後ならエラー
		if (day.hours() < 12) {
			return true;
		}
		return false;
	}
private:
	ITime* time;
};
