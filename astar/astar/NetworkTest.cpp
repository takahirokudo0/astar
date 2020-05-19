#include <stdio.h>
#include <iostream>
#include "NetworkTest.h"

Network::Network()
{
	AddFunction();
}
// ディスパッチ候補のメソッドを追加する
void Network::AddFunction()
{
	AddTask<HogeRequest>(PACKET_ID::HOGE, &Network::OnHoge);
	AddTask<FooRequest>(PACKET_ID::FOO, &Network::OnFoo);
}
// 本来ならクライアントにパケットを送信するメソッド
void Network::SendResponse(Response response)
{
	// テスト中に呼ばれるとエラーになってしまう
	std::cout << "Don't send response in Test." << std::endl;
}
// Hogeリクエストを受信すると実行される。
void Network::OnHoge(HogeRequest request)
{
	// ここでHogeに関する処理を実行する（略）
	// レスポンスを返すので、テスト中に呼ばれるとエラーになる
	SendResponse(HogeResponse{});
}
// Fooリクエストを受信すると実行される。
void Network::OnFoo(FooRequest request)
{
	// Fooの処理を別メソッドにしている（DoFoo）
	// レスポンスを返すので、テスト中に呼ばれるとエラーになる
	SendResponse(DoFoo(request));
}
// Fooを実行し、結果（レスポンス）を生成している
Response Network::DoFoo(FooRequest request)
{
	Response response;
	response.result_code = ResultCode::Success;
	return response;
}
// メッセージを受信した時のディスパッチ処理
void Network::Receive(PACKET_ID packet_id, const char* message, int size) {
	auto it = functions.find(packet_id);
	if (it != functions.end()) {
		it->second(packet_id, message, size);
	}
}