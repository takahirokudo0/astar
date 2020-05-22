#pragma once

#include <map>
#include <list>
#include <vector>
#include <memory>
#include <functional>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/function.hpp>

enum class PACKET_ID : int {
	HOGE,
	FOO,
};

class Request {
public:
	int packet_id = 0;
};
class Response {
public:
	unsigned char result_code = 0;
};
class HogeRequest : public Request {
};
class HogeResponse : public Response {
};
class FooRequest : public Request {
};
class FooResponse : public Response {
};
class Network {
public:
	enum ResultCode {
		Success = 0,
		Failure,
	};
	typedef Network SELF_TYPE;
	typedef boost::function<void(PACKET_ID, const char*, int)> Fn;
	std::map<PACKET_ID, Fn> functions;

	Network();
	void AddFunction();
	void SendResponse(Response response);
	// �e�p�P�b�g���f�B�X�p�b�`���郁�\�b�h
	void OnHoge(HogeRequest request);
	void OnFoo(FooRequest request);
	// ���ۂɋ@�\��񋟂��A�e�X�g�Ŏg�p���郁�\�b�h
	Response DoFoo(FooRequest request);

	// ��M -> Receive -> functions -> TransformPacket -> func(=OnHoge��)
	void Receive(PACKET_ID packet_id, const char* message, int size);
	// packet_id�ɑΉ��������N�G�X�g�������\�b�h�Ăяo��
	// �ϊ����\�b�h�̐����i�ŏI�I�ɌĂԂׂ����\�b�h�̃R�[�����L�q�j
	template<typename Req>
	static void TransformPacket(SELF_TYPE* network, PACKET_ID packet_id, const char* message, int size, boost::function<void(SELF_TYPE* network, Req)> func) {
		func(network, Req());	// message+size����Request�𐶐�(�֘A�R�[�h�������Ȃ�̂ŏȗ�)
	}
	// functions�̓o�^
	template<typename Req>
	void AddTask(PACKET_ID packet_id, boost::function<void(SELF_TYPE* network, Req)> func) {
		// functions��func��o�^�ł��Ȃ�(��template)�̂ŁAstd::bind�𗘗p���ă����N�b�V�������ނ��Ƃœo�^�ł���悤�ɂȂ�.
		functions.emplace(packet_id, boost::bind(&TransformPacket<Req>, this, _1, _2, _3, func));
	}
};


