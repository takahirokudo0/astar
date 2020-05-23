#pragma once

#include <memory>
#include <boost/function.hpp>
#include "WebApi.h"
#include "NetworkTest.h"

class WebApiTest {
public:
	bool Func1(boost::function<void(const Response&)> callback) {
		if (webapi) {
			webapi->SendQueue<Request, Response>(0, Request(), callback);
		}
		return true;
	}
	void SetWebApi(WebApiBase* webapi) {
		this->webapi = webapi;
	}
private:
	void OnFunc1(const Response& res) {
	}

	WebApiBase* webapi;
};