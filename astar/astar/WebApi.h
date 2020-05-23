#pragma once

#include <queue>
#include <boost/function.hpp>
#include <boost/thread.hpp>

class WebApiBase {
public:
	WebApiBase() {
		Run();			// �������ɃX���b�h���J�n
	}
	~WebApiBase() {
		thread.join();	// �j�����ɃX���b�h���I��
	}
	// template���\�b�h�͒��ۉ��ł��Ȃ�
	template <typename Req, typename Res>
	void SendQueue(int puuid, const Req& req, boost::function<void(const Res&)> callback) {
		// ���N�G�X�g�𑗐M
		SendPacket(puuid, "", 0);
		// �R�[���o�b�N�̐ݒ�
		queue.push(boost::bind(&Task<Req, Res>, callback));
	}
protected:
	std::queue<std::function<void(void)>> queue;
	virtual void Update() = 0;
	virtual void SendPacket(int puuid, const char* message, int size) {
		// WebApi�Ƀ��N�G�X�g���M����
	}
private:
	template <typename Req, typename Res>
	static void Task(boost::function<void(const Res&)> callback) {
		callback(Res());
	}
	boost::thread thread;

	void Run() {
		// �ʃX���b�h�ŏ펞Queue�̑���M�������s��
		thread = boost::thread(&WebApiBase::Update, this);
	}
};
class WebApi : public WebApiBase {
protected:
	virtual void Update() override {
		while (true) {
			// WebApi�����M������Callback���Ăяo���Aqueue����pop����B
			break;
		}
	}
};
class TestWebApi : public WebApiBase {
protected:
	virtual void SendPacket(int puuid, const char* message, int size) override {
		// WebApi�Ƀ��N�G�X�g���M����
		// ���b�N�Ȃ̂ŉ������炸������override����
	}
	virtual void Update() override {
		while (true) {
			Sleep(10);
			if (queue.size() > 0) {
				// webapi���烌�X�|���X���͂����Ƃ����z���callback���Ăяo��
				auto callback = queue.front();
				callback();
				queue.pop();
				break;
			}
		}
	}
};
