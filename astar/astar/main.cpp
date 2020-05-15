#include "AStar.h"
#include "Tester.h"

int main()
{
	// 普通にAStarを実行
	SearchRoute();

	// 作成済みテストケースに基づいてAStarのテストを実行
	Tester tester;
	tester.Run();

	system("pause");
	return 0;
}
