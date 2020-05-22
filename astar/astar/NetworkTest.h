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
	// 各パケットをディスパッチするメソッド
	void OnHoge(HogeRequest request);
	void OnFoo(FooRequest request);
	// 実際に機能を提供し、テストで使用するメソッド
	Response DoFoo(FooRequest request);

	// 受信 -> Receive -> functions -> TransformPacket -> func(=OnHoge等)
	void Receive(PACKET_ID packet_id, const char* message, int size);
	// packet_idに対応したリクエスト生成メソッド呼び出し
	// 変換メソッドの生成（最終的に呼ぶべきメソッドのコールも記述）
	template<typename Req>
	static void TransformPacket(SELF_TYPE* network, PACKET_ID packet_id, const char* message, int size, boost::function<void(SELF_TYPE* network, Req)> func) {
		func(network, Req());	// message+sizeからRequestを生成(関連コードが長くなるので省略)
	}
	// functionsの登録
	template<typename Req>
	void AddTask(PACKET_ID packet_id, boost::function<void(SELF_TYPE* network, Req)> func) {
		// functionsにfuncを登録できない(※template)ので、std::bindを利用してワンクッション挟むことで登録できるようになる.
		functions.emplace(packet_id, boost::bind(&TransformPacket<Req>, this, _1, _2, _3, func));
	}
};


