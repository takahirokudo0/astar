#include <stdio.h>
#include <iostream>
#include "NetworkTest.h"

Network::Network()
{
	AddFunction();
}
// �f�B�X�p�b�`���̃��\�b�h��ǉ�����
void Network::AddFunction()
{
	AddTask<HogeRequest>(PACKET_ID::HOGE, &Network::OnHoge);
	AddTask<FooRequest>(PACKET_ID::FOO, &Network::OnFoo);
}
// �{���Ȃ�N���C�A���g�Ƀp�P�b�g�𑗐M���郁�\�b�h
void Network::SendResponse(Response response)
{
	// �e�X�g���ɌĂ΂��ƃG���[�ɂȂ��Ă��܂�
	std::cout << "Don't send response in Test." << std::endl;
}
// Hoge���N�G�X�g����M����Ǝ��s�����B
void Network::OnHoge(HogeRequest request)
{
	// ������Hoge�Ɋւ��鏈�������s����i���j
	// ���X�|���X��Ԃ��̂ŁA�e�X�g���ɌĂ΂��ƃG���[�ɂȂ�
	SendResponse(HogeResponse{});
}
// Foo���N�G�X�g����M����Ǝ��s�����B
void Network::OnFoo(FooRequest request)
{
	// Foo�̏�����ʃ��\�b�h�ɂ��Ă���iDoFoo�j
	// ���X�|���X��Ԃ��̂ŁA�e�X�g���ɌĂ΂��ƃG���[�ɂȂ�
	SendResponse(DoFoo(request));
}
// Foo�����s���A���ʁi���X�|���X�j�𐶐����Ă���
Response Network::DoFoo(FooRequest request)
{
	Response response;
	response.result_code = ResultCode::Success;
	return response;
}
// ���b�Z�[�W����M�������̃f�B�X�p�b�`����
void Network::Receive(PACKET_ID packet_id, const char* message, int size) {
	auto it = functions.find(packet_id);
	if (it != functions.end()) {
		it->second(packet_id, message, size);
	}
}