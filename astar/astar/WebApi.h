#pragma once

#include <queue>
#include <boost/function.hpp>
#include <boost/thread.hpp>

class WebApiBase {
public:
	WebApiBase() {
		Run();			// 生成時にスレッドを開始
	}
	~WebApiBase() {
		thread.join();	// 破棄時にスレッドを終了
	}
	// templateメソッドは抽象化できない
	template <typename Req, typename Res>
	void SendQueue(int puuid, const Req& req, boost::function<void(const Res&)> callback) {
		// リクエストを送信
		SendPacket(puuid, "", 0);
		// コールバックの設定
		queue.push(boost::bind(&Task<Req, Res>, callback));
	}
protected:
	std::queue<std::function<void(void)>> queue;
	virtual void Update() = 0;
	virtual void SendPacket(int puuid, const char* message, int size) {
		// WebApiにリクエスト送信処理
	}
private:
	template <typename Req, typename Res>
	static void Task(boost::function<void(const Res&)> callback) {
		callback(Res());
	}
	boost::thread thread;

	void Run() {
		// 別スレッドで常時Queueの送受信処理を行う
		thread = boost::thread(&WebApiBase::Update, this);
	}
};
class WebApi : public WebApiBase {
protected:
	virtual void Update() override {
		while (true) {
			// WebApiから受信したらCallbackを呼び出し、queueからpopする。
			break;
		}
	}
};
class TestWebApi : public WebApiBase {
protected:
	virtual void SendPacket(int puuid, const char* message, int size) override {
		// WebApiにリクエスト送信処理
		// モックなので何も送らず処理でoverrideする
	}
	virtual void Update() override {
		while (true) {
			Sleep(10);
			if (queue.size() > 0) {
				// webapiからレスポンスが届いたという想定でcallbackを呼び出す
				auto callback = queue.front();
				callback();
				queue.pop();
				break;
			}
		}
	}
};
