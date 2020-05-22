#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>

class ITime {
public:
	virtual boost::posix_time::ptime Now() = 0;
	virtual long long Millisecond() = 0;
};
// 通常のタイマークラス
class Time : public ITime {
public:
	Time() {
		epoch = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
	}
	virtual long long Millisecond() {
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		return (now - epoch).total_milliseconds();
	}
	virtual boost::posix_time::ptime Now() {
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		return now;
	}
private:
	boost::posix_time::ptime epoch;
};
// 指定した時間を返却するだけのクラス
class TestTime : public ITime {
public:
	TestTime(boost::posix_time::ptime now) : now(now) {
		epoch = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
	}
	virtual long long Millisecond() {
		return (now - epoch).total_milliseconds();
	}
	virtual boost::posix_time::ptime Now() {
		return now;
	}
private:
	boost::posix_time::ptime epoch;
	boost::posix_time::ptime now;
};
