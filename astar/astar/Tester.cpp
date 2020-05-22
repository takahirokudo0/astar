#include <sstream>
#include <iostream>
#include <algorithm>
#include "Tester.h"

#include "AStar.h"
#include "NetworkTest.h"
#include "TimeTest.h"

// AStarのテストケースを定義
std::vector<TestCase> CreateAStarTestCases() {
	// 共通入力データを定義
	// NOTE: static定義かつラムダのキャプチャを参照にすることで多量のコピーを防ぐ
	static auto map = std::make_shared<Map>();
	map->SetMap(std::vector<std::vector<int>> {
		{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 0, 0, 0 },
		{ 1, 0, 0, 0, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 1, 1, 0, 1, 0, 0, 1, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 1, 1, 0, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	});

	std::vector<TestCase> testcases;
	// 正常系1
	testcases.push_back(TestCase([&]() {
		AStar astar(map);													// テスト対象生成
		Route route = astar.Search(Position(1, 1), Position(8, 8), false);	// 実行
		Route expected = Route(std::list<boost::optional<Position>>			// 期待値
		{
			Position(1, 1), Position(2, 1), Position(2, 0), Position(3, 0),
			Position(4, 0), Position(4, 1), Position(4, 2), Position(4, 3),
			Position(4, 4), Position(5, 4), Position(5, 5), Position(5, 6),
			Position(6, 6), Position(6, 7), Position(6, 8), Position(6, 9),
			Position(7, 9), Position(8, 9), Position(8, 8),
		});
		// テスト結果の比較
		return route == expected;
	}));
	// 正常系2
	testcases.push_back(TestCase([&]() {
		AStar astar(map);
		Route route = astar.Search(Position(2, 1), Position(8, 4), false);
		Route expected = Route(std::list<boost::optional<Position>>
		{
			Position(2, 1), Position(2, 0), Position(3, 0), Position(4, 0),
			Position(4, 1), Position(5, 1), Position(5, 2), Position(6, 2),
			Position(6, 3), Position(7, 3), Position(7, 4), Position(8, 4),
		});
		return route == expected;
	}));
	// 解無し（たどり着かない）
	testcases.push_back(TestCase([&]() {
		AStar astar(map);
		Route route = astar.Search(Position(2, 1), Position(8, 0), false);
		Route expected = Route(std::list<boost::optional<Position>>
		{
		});
		return route == expected;
	}));
	// 開始と終了の座標が同じ
	testcases.push_back(TestCase([&]() {
		AStar astar(map);
		Route route = astar.Search(Position(2, 1), Position(2, 1), false);
		Route expected = Route(std::list<boost::optional<Position>>
		{
			Position(2, 1),
		});
		return route == expected;
	}));
	// １つ隣のノードを探索
	testcases.push_back(TestCase([&]() {
		AStar astar(map);
		Route route = astar.Search(Position(2, 1), Position(2, 0), false);
		Route expected = Route(std::list<boost::optional<Position>>
		{
			Position(2, 1), Position(2, 0),
		});
		return route == expected;
	}));

	return testcases;
}
// 通信の発生する(という想定の)テストケースを定義
std::vector<TestCase> CreateNetworkTestCases() {
	std::vector<TestCase> testcases;
	// OnHogeの作りだと、問題がある。
	// 1.本当にクライアントへパケットを送ろうとしてしまう。
	//   ユニットテストでは実際に接続するクライアントは存在しないのでエラーになる。
	// 2.結果が返ってこないため成否の判定ができない。
	//testcases.push_back(TestCase([&]() {
	//	Network network;
	//	network.OnHoge(HogeRequest());
	//	return true;
	//}));
	testcases.push_back(TestCase([&]() {
		Network network;
		auto response = network.DoFoo(FooRequest());
		return response.result_code == Network::ResultCode::Success;
	}));

	return testcases;
}
// 時間に関わるテストケースを定義
std::vector<TestCase> CreateTimeTestCases() {
	std::vector<TestCase> testcases;

	testcases.push_back(TestCase([&]() {
		TimeTest timetest;
		TestTime time(boost::posix_time::time_from_string("2020-04-01 00:00:00.000"));
		timetest.SetTime(&time);
		return timetest.Func1();
	}));
	testcases.push_back(TestCase([&]() {
		TimeTest timetest;
		TestTime time(boost::posix_time::time_from_string("2020-05-01 05:00:00.000"));
		timetest.SetTime(&time);
		return timetest.Func2();
	}));

	return testcases;
}

// SampleAのテストケースを定義（出力結果確認用のダミーテスト）
std::vector<TestCase> CreateSampleATestCases() {
	std::vector<TestCase> testcases;
	// ダミーとして空っぽのテストケースを40個追加する
	for (int i = 0; i < 40; i++) {
		testcases.push_back(TestCase([]() {return true; }));
	}

	return testcases;
}

// テストケースを作成
std::vector<TestCase> CreateTestCases() {
	// テスト対象のテストケース定義メソッドをセット
	// TODO: 外部環境からコンパイル不要で設定を変更したい
	auto funcs = {
		CreateTimeTestCases,
		CreateAStarTestCases,
		CreateSampleATestCases,
		CreateNetworkTestCases,
	};

	// テストケースを作成する
	std::vector<TestCase> testcases;
	for (auto func : funcs) {
		auto cases = func();
		if (cases.size() > 0) {
			testcases.insert(testcases.end(), cases.begin(), cases.end());
		}
	}

	return testcases;
}

// テストの実行
void Tester::Run() {
	// テストケース取得（様々なテストケースを１つの配列でまとめて管理）
	std::vector<TestCase> testcases = CreateTestCases();

	// テストの実行と結果の出力
	int counter = 0;
	int max_count = testcases.size();
	for (int i = 0; i < testcases.size(); i++) {
		DisplayResult(++counter, max_count, Test(testcases[i]), i == testcases.size() - 1);
	}
}

bool Tester::Test(const TestCase& testcase) {
	if (!testcase.check) {
		return false;
	}
	return testcase.check();
}

void Tester::DisplayResult(int number, int max_count, bool result, bool is_last) {
	constexpr int width_count = 0x0014;
	std::stringstream ss;
	ss << (result ? "*" : "E");

	if (is_last) {
		for (int i = 0; i < width_count - (max_count % width_count); i++) {
			ss << " ";
		}
		ss << " " << max_count << "/" << max_count << std::endl;
	}
	else if (number % width_count == 0) {
		ss << " " << number << "/" << max_count << std::endl;
	}
	std::cout << ss.str();

	// TODO: エラー判定となったテストケースのエラーメッセージを集積しておいて、出力したい

}