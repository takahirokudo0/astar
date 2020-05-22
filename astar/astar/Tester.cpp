#include <sstream>
#include <iostream>
#include <algorithm>
#include "Tester.h"

#include "AStar.h"
#include "NetworkTest.h"
#include "TimeTest.h"

// AStar�̃e�X�g�P�[�X���`
std::vector<TestCase> CreateAStarTestCases() {
	// ���ʓ��̓f�[�^���`
	// NOTE: static��`�������_�̃L���v�`�����Q�Ƃɂ��邱�Ƃő��ʂ̃R�s�[��h��
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
	// ����n1
	testcases.push_back(TestCase([&]() {
		AStar astar(map);													// �e�X�g�Ώې���
		Route route = astar.Search(Position(1, 1), Position(8, 8), false);	// ���s
		Route expected = Route(std::list<boost::optional<Position>>			// ���Ғl
		{
			Position(1, 1), Position(2, 1), Position(2, 0), Position(3, 0),
			Position(4, 0), Position(4, 1), Position(4, 2), Position(4, 3),
			Position(4, 4), Position(5, 4), Position(5, 5), Position(5, 6),
			Position(6, 6), Position(6, 7), Position(6, 8), Position(6, 9),
			Position(7, 9), Position(8, 9), Position(8, 8),
		});
		// �e�X�g���ʂ̔�r
		return route == expected;
	}));
	// ����n2
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
	// �𖳂��i���ǂ蒅���Ȃ��j
	testcases.push_back(TestCase([&]() {
		AStar astar(map);
		Route route = astar.Search(Position(2, 1), Position(8, 0), false);
		Route expected = Route(std::list<boost::optional<Position>>
		{
		});
		return route == expected;
	}));
	// �J�n�ƏI���̍��W������
	testcases.push_back(TestCase([&]() {
		AStar astar(map);
		Route route = astar.Search(Position(2, 1), Position(2, 1), false);
		Route expected = Route(std::list<boost::optional<Position>>
		{
			Position(2, 1),
		});
		return route == expected;
	}));
	// �P�ׂ̃m�[�h��T��
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
// �ʐM�̔�������(�Ƃ����z���)�e�X�g�P�[�X���`
std::vector<TestCase> CreateNetworkTestCases() {
	std::vector<TestCase> testcases;
	// OnHoge�̍�肾�ƁA��肪����B
	// 1.�{���ɃN���C�A���g�փp�P�b�g�𑗂낤�Ƃ��Ă��܂��B
	//   ���j�b�g�e�X�g�ł͎��ۂɐڑ�����N���C�A���g�͑��݂��Ȃ��̂ŃG���[�ɂȂ�B
	// 2.���ʂ��Ԃ��Ă��Ȃ����ߐ��ۂ̔��肪�ł��Ȃ��B
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
// ���ԂɊւ��e�X�g�P�[�X���`
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

// SampleA�̃e�X�g�P�[�X���`�i�o�͌��ʊm�F�p�̃_�~�[�e�X�g�j
std::vector<TestCase> CreateSampleATestCases() {
	std::vector<TestCase> testcases;
	// �_�~�[�Ƃ��ċ���ۂ̃e�X�g�P�[�X��40�ǉ�����
	for (int i = 0; i < 40; i++) {
		testcases.push_back(TestCase([]() {return true; }));
	}

	return testcases;
}

// �e�X�g�P�[�X���쐬
std::vector<TestCase> CreateTestCases() {
	// �e�X�g�Ώۂ̃e�X�g�P�[�X��`���\�b�h���Z�b�g
	// TODO: �O��������R���p�C���s�v�Őݒ��ύX������
	auto funcs = {
		CreateTimeTestCases,
		CreateAStarTestCases,
		CreateSampleATestCases,
		CreateNetworkTestCases,
	};

	// �e�X�g�P�[�X���쐬����
	std::vector<TestCase> testcases;
	for (auto func : funcs) {
		auto cases = func();
		if (cases.size() > 0) {
			testcases.insert(testcases.end(), cases.begin(), cases.end());
		}
	}

	return testcases;
}

// �e�X�g�̎��s
void Tester::Run() {
	// �e�X�g�P�[�X�擾�i�l�X�ȃe�X�g�P�[�X���P�̔z��ł܂Ƃ߂ĊǗ��j
	std::vector<TestCase> testcases = CreateTestCases();

	// �e�X�g�̎��s�ƌ��ʂ̏o��
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

	// TODO: �G���[����ƂȂ����e�X�g�P�[�X�̃G���[���b�Z�[�W���W�ς��Ă����āA�o�͂�����

}