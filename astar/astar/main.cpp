#include "AStar.h"
#include "Tester.h"

int main()
{
	// ���ʂ�AStar�����s
	SearchRoute();

	// �쐬�ς݃e�X�g�P�[�X�Ɋ�Â���AStar�̃e�X�g�����s
	Tester tester;
	tester.Run();

	system("pause");
	return 0;
}
