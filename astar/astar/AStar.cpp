#include <vector>
#include <memory>
#include <list>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <limits>
#include "AStar.h"

// ��_�Ԃ̋������v�Z
float CalcDistance(const Position& pos1, const Position& pos2) {
	return sqrtf(static_cast<float>(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y)));
}

void SearchRoute()
{
	// �}�b�v�쐬
	auto map = std::make_shared<Map>();
	map->SetMap(std::vector<std::vector<int>> {
		{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 1, 1, 0, 1, 0, 0, 1, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 1, 1, 0, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	});
	// �}�b�v����O���t���쐬
	auto graph = std::make_shared<Graph>();
	graph->Create(*map);
	// �T���N���X���쐬
	AStar astar;
	astar.SetMap(map);
	astar.SetGraph(graph);
	astar.Search(Position(1, 1), Position(8, 8));
	//astar.Search(Position(3, 2), Position(0, 4));
	//astar.Search(Position(1, 5), Position(7, 1));
	//astar.Search(Position(3, 3), Position(2, 8));
	std::cout << "--------------------" << std::endl;
}
AStar::AStar(std::shared_ptr<Map> map) : map(map)
{
	graph = std::make_shared<Graph>();
	graph->Create(*map);
}
void AStar::Init()
{
	last_update_positions.clear();
	last_update_positions.resize(map->GetHeight());
	for (int y = 0; y < map->GetHeight(); y++) {
		last_update_positions[y].resize(map->GetWidth());
	}
	// �R�X�g�Ȃǂ�������
	graph->Reset();
}
Route AStar::Search(const Position& start, const Position& end, bool is_display)
{
	Init();

	NodeList open_list, close_list;
	// �J�n�m�[�h��open�ɒǉ�
	open_list.push_back(&(graph->Get(start.y, start.x)));
	Node* node = nullptr;
	// �T�� open����̏ꍇ�A�T���͎��s
	while (!open_list.empty()) {
		// open_list���玟�ɒT������m�[�h���擾����
		node = GetNextNode(open_list);
		// �S�[���ɓ��B�������H
		if (!node && node->position == end) {
			// �ŏI�m�[�h���N���[�Y���X�g�ɒǉ�����
			close_list.push_back(node);
			break;
		}
		// �㉺���E�̗אڃm�[�h��open����
		OpenAdjacent(node, open_list, close_list, end);
		// �T�����I�������̂�close����
		close_list.push_back(node);
	}
	// �o�H����
	auto route = Route::Create(start, end, last_update_positions);
	// �o�H�\��
	if (is_display) {
		route.DisplayRoute(map, start, end);
	}
	return route;
}
// open���玟�̒T���m�[�h���擾����
Node* AStar::GetNextNode(NodeList& open_list)
{
	// open���R�X�g�̏����Ń\�[�g
	std::sort(open_list.begin(), open_list.end(),
		[](Node* a, Node* b) { return a->total_cost > b->total_cost; });
	// open����ŏ��R�X�g�̃m�[�h���擾
	auto node = open_list.front();
	// open����폜
	open_list.erase(open_list.begin());
	return node;
}
// �אڃm�[�h��T������
void AStar::OpenAdjacent(Node* node, NodeList& open_list, NodeList& close_list, const Position& end) {
	static int OpenAdjacentCount = 0;
	OpenAdjacentCount++;
	// �㉺���E�̗אڃm�[�h��open����
	for (auto edge : node->edges) {
		// �G�b�W�ւ̈ړ��R�X�g
		float edge_cost = static_cast<float>(map->Get(edge->position.y, edge->position.x));
		// �G�b�W�̗\���R�X�g
		if (edge->heuristic_cost == std::numeric_limits<float>::max()) {
			edge->heuristic_cost = CalcDistance(end, edge->position);
		}
		// �����܂ł̃g�[�^���R�X�g
		float total_cost = node->total_cost;
		// �Ώۃm�[�h�֐i�񂾎��̃S�[���܂ł̑z��R�X�g
		float cost = total_cost + edge_cost + edge->heuristic_cost;
		if (AddOpenList(open_list, close_list, edge, cost)) {
			edge->total_cost = cost;
			// �o�H�����p�ɂ��̃m�[�h���Ō�ɍX�V�������W��ۑ�����B
			last_update_positions[edge->position.y][edge->position.x] = node->position;
		}
	}
}
// open�փm�[�h��ǉ�
bool AStar::AddOpenList(NodeList& open_list, NodeList& close_list, Node* target_node, float cost)
{
	bool can_update = true;
	// �Ώۃm�[�h�ւ̍ŒZ�o�H�ł����open/close����폜
	if (close_list.EraseNode(target_node, cost) == NodeList::EraseResult::NotErase) {
		can_update = false;
	}
	if (open_list.EraseNode(target_node, cost) == NodeList::EraseResult::NotErase) {
		can_update = false;
	}
	// �����܂ł̍ŒZ�o�H�ł����open����
	if (can_update) {
		open_list.push_back(target_node);
		return true;
	}
	return false;
}

// �O���t�̍쐬
void Graph::Create(const Map& map) {
	graph.clear();
	height = map.GetHeight();
	width = map.GetWidth();
	// �z��̃T�C�Y�m��
	for (int y = 0; y < height; y++) {
		graph.resize(height);
		for (int x = 0; x < width; x++) {
			graph[y].resize(width);
		}
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// ���W�̐ݒ�
			graph[y][x].position = Position(x, y);
			// �G�b�W�̐ݒ�
			Position adjacent_positions[] = {
				Position(x, y - 1), //��
				Position(x, y + 1),	//��
				Position(x - 1, y),	//��
				Position(x + 1, y),	//�E
			};
			for (auto pos : adjacent_positions) {
				if (map.IsInRange(pos) && IsTraversable(map.Get(y, x), map.Get(pos.y, pos.x))) {
					graph[y][x].edges.push_back(&graph[pos.y][pos.x]);
				}
			}
		}
	}
}
// �O���t�̃��Z�b�g
void Graph::Reset() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// �q���[���X�e�B�b�N�R�X�g�̏������i�K�v�Ȏ��Ɉ�x�����v�Z�j
			graph[y][x].heuristic_cost = std::numeric_limits<float>::max();
			// �g�[�^���R�X�g�̏�����
			graph[y][x].total_cost = 0;
		}
	}
}

Route::Route(std::list<boost::optional<Position>> list)
{
	insert(end(), list.begin(), list.end());
}
// �T���������[�g��\������
void Route::DisplayRoute(std::shared_ptr<Map> map, const Position& start, const Position& end) {
	std::cout << "--------------------" << std::endl;
	if (empty()) {
		std::cout << "route not found." << std::endl;
		return;
	}
	//for (auto a : *this) {
	//	std::cout << "Position(" << a->x << ", " << a->y << "),"<< std::endl;
	//}
	for (int y = 0; y < map->GetHeight(); y++) {
		for (int x = 0; x < map->GetWidth(); x++) {
			// �S�[���̏ꍇ��G���o��
			if (end == Position(x, y)) {
				std::cout << " G";
			}
			// �X�^�[�g�̏ꍇ��S���o��
			else if (start == Position(x, y)) {
				std::cout << " S";
			}
			// �o�H�̏ꍇ��*���o��
			else if (std::find(begin(), this->end(), Position(x, y)) != this->end()) {
				std::cout << " *";
			}
			// ���̑��̏ꍇ�͒n�`���o��
			else {
				std::cout << " " << map->Get(y, x);
			}
		}
		std::cout << std::endl;
	}
}
// �o�H����
Route Route::Create(const Position& start, const Position& end, std::vector<std::vector<boost::optional<Position>>> last_update_positions) {
	Route route;
	route.push_back(end);
	while (1) {
		auto pos = route.front();
		// �I������ start�܂œ��B�������H
		if (pos == start) {
			break;		// �I��
		}
		if (pos != boost::none) {
			route.push_front(last_update_positions[pos->y][pos->x]);
		}
		else {
			route.clear();
			break;	// �o�H�������������ߏI��
		}
	}
	return route;
}

// �m�[�h�폜
NodeList::EraseResult NodeList::EraseNode(Node* target_node, float cost) {
	for (auto it = begin(); it != end(); it++) {
		auto node = *it;
		if (node->position == target_node->position)
		{
			if (target_node->total_cost > cost) {
				erase(it);	//�폜
				return EraseResult::Erased;
			}
			else {
				return EraseResult::NotErase;
			}
		}
	}
	return EraseResult::NotFound;
}

void Map::SetMap(std::vector<std::vector<int>>&& data)
{
	map = std::move(data);
	height = map.size();
	width = height > 0 ? map[0].size() : 0;
}
