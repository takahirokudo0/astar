#pragma once

#include <map>
#include <list>
#include <vector>
#include <memory>
#include <boost/optional.hpp>

void SearchRoute();

// 座標クラス
class Position {
public:
	Position() {}
	Position(int x, int y) :x(x), y(y) {}
	bool operator ==(const Position& position) const {
		return x == position.x && y == position.y;
	}
	int x = -1, y = -1;
};
// マップクラス
class Map {
public:
	bool IsInRange(const Position& pos) const {
		if (pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height) {
			return false;
		}
		return true;
	}
	int Get(int y, int x) const {
		return IsInRange(Position(x, y)) ? map[y][x] : -1;
	}
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	void SetMap(std::vector<std::vector<int>> &&data);
private:
	int height = 10;
	int width = 10;
	std::vector<std::vector<int>> map;
};
// ノードクラス
class Node {
public:
	Node() {}
	Position position;
	std::vector<Node*> edges;	// 隣接ノードはまだよくわからない
	float heuristic_cost;
	float total_cost;
};
// グラフクラス
class Graph {
public:
	void Create(const Map& map);
	void Reset();
	Node& operator()(int y, int x) {
		return graph[y][x];
	}
	Node& Get(int y, int x) {
		return graph[y][x];
	}
	// 通行可能か？
	bool IsTraversable(int current, int target) {
		// 高低差の場合などは判定を変える
		if (target == 0) {
			return false;
		}
		return true;
	}
private:
	int height = 0;
	int width = 0;
	std::vector<std::vector<Node>> graph;
};
// 経路クラス
class Route : public std::list<boost::optional<Position>> {
public:
	Route() {}
	Route(std::list<boost::optional<Position>> list);
	// 探索したルートを表示する
	void DisplayRoute(std::shared_ptr<Map> map, const Position& start, const Position& end);
	static Route Create(const Position& start, const Position& end, std::vector<std::vector<boost::optional<Position>>> last_update_positions);
};
class NodeList : public std::vector<Node*> {
public:
	enum EraseResult {
		Erased = 0,
		NotErase,
		NotFound,
	};
	// ノードの削除
	EraseResult EraseNode(Node* target_node, float cost);
};
// 探索クラス
class AStar {
public:
	AStar() {}
	AStar(std::shared_ptr<Map> map);
	Route Search(const Position& start, const Position& end, bool is_display = true);
	void SetMap(std::shared_ptr<Map> map) {
		this->map = map;
	}
	void SetGraph(std::shared_ptr<Graph> graph) {
		this->graph = graph;
	}
private:
	void Init();
	// openから次の探索ノードを取得する
	Node* GetNextNode(NodeList& open_list);
	// 隣接ノードを探索する
	void OpenAdjacent(Node* node, NodeList& open_list, NodeList& close_list, const Position& end);
	// openへノードを追加
	bool AddOpenList(NodeList& open_list, NodeList& close_list, Node* target_node, float cost);

	std::shared_ptr<Map> map;
	std::shared_ptr<Graph> graph;
	std::vector<std::vector<boost::optional<Position>>> last_update_positions;
};
