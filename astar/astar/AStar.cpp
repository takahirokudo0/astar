#include <vector>
#include <memory>
#include <list>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <limits>
#include "AStar.h"

// 二点間の距離を計算
float CalcDistance(const Position& pos1, const Position& pos2) {
	return sqrtf(static_cast<float>(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y)));
}

void SearchRoute()
{
	// マップ作成
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
	// マップからグラフを作成
	auto graph = std::make_shared<Graph>();
	graph->Create(*map);
	// 探索クラスを作成
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
	// コストなどを初期化
	graph->Reset();
}
Route AStar::Search(const Position& start, const Position& end, bool is_display)
{
	Init();

	NodeList open_list, close_list;
	// 開始ノードをopenに追加
	open_list.push_back(&(graph->Get(start.y, start.x)));
	Node* node = nullptr;
	// 探索 openが空の場合、探索は失敗
	while (!open_list.empty()) {
		// open_listから次に探索するノードを取得する
		node = GetNextNode(open_list);
		// ゴールに到達したか？
		if (!node && node->position == end) {
			// 最終ノードもクローズリストに追加する
			close_list.push_back(node);
			break;
		}
		// 上下左右の隣接ノードをopenする
		OpenAdjacent(node, open_list, close_list, end);
		// 探索が終了したのでcloseする
		close_list.push_back(node);
	}
	// 経路復元
	auto route = Route::Create(start, end, last_update_positions);
	// 経路表示
	if (is_display) {
		route.DisplayRoute(map, start, end);
	}
	return route;
}
// openから次の探索ノードを取得する
Node* AStar::GetNextNode(NodeList& open_list)
{
	// openをコストの昇順でソート
	std::sort(open_list.begin(), open_list.end(),
		[](Node* a, Node* b) { return a->total_cost > b->total_cost; });
	// openから最小コストのノードを取得
	auto node = open_list.front();
	// openから削除
	open_list.erase(open_list.begin());
	return node;
}
// 隣接ノードを探索する
void AStar::OpenAdjacent(Node* node, NodeList& open_list, NodeList& close_list, const Position& end) {
	static int OpenAdjacentCount = 0;
	OpenAdjacentCount++;
	// 上下左右の隣接ノードをopenする
	for (auto edge : node->edges) {
		// エッジへの移動コスト
		float edge_cost = static_cast<float>(map->Get(edge->position.y, edge->position.x));
		// エッジの予測コスト
		if (edge->heuristic_cost == std::numeric_limits<float>::max()) {
			edge->heuristic_cost = CalcDistance(end, edge->position);
		}
		// ここまでのトータルコスト
		float total_cost = node->total_cost;
		// 対象ノードへ進んだ時のゴールまでの想定コスト
		float cost = total_cost + edge_cost + edge->heuristic_cost;
		if (AddOpenList(open_list, close_list, edge, cost)) {
			edge->total_cost = cost;
			// 経路復元用にこのノードを最後に更新した座標を保存する。
			last_update_positions[edge->position.y][edge->position.x] = node->position;
		}
	}
}
// openへノードを追加
bool AStar::AddOpenList(NodeList& open_list, NodeList& close_list, Node* target_node, float cost)
{
	bool can_update = true;
	// 対象ノードへの最短経路であればopen/closeから削除
	if (close_list.EraseNode(target_node, cost) == NodeList::EraseResult::NotErase) {
		can_update = false;
	}
	if (open_list.EraseNode(target_node, cost) == NodeList::EraseResult::NotErase) {
		can_update = false;
	}
	// ここまでの最短経路であればopenする
	if (can_update) {
		open_list.push_back(target_node);
		return true;
	}
	return false;
}

// グラフの作成
void Graph::Create(const Map& map) {
	graph.clear();
	height = map.GetHeight();
	width = map.GetWidth();
	// 配列のサイズ確保
	for (int y = 0; y < height; y++) {
		graph.resize(height);
		for (int x = 0; x < width; x++) {
			graph[y].resize(width);
		}
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// 座標の設定
			graph[y][x].position = Position(x, y);
			// エッジの設定
			Position adjacent_positions[] = {
				Position(x, y - 1), //上
				Position(x, y + 1),	//下
				Position(x - 1, y),	//左
				Position(x + 1, y),	//右
			};
			for (auto pos : adjacent_positions) {
				if (map.IsInRange(pos) && IsTraversable(map.Get(y, x), map.Get(pos.y, pos.x))) {
					graph[y][x].edges.push_back(&graph[pos.y][pos.x]);
				}
			}
		}
	}
}
// グラフのリセット
void Graph::Reset() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// ヒューリスティックコストの初期化（必要な時に一度だけ計算）
			graph[y][x].heuristic_cost = std::numeric_limits<float>::max();
			// トータルコストの初期化
			graph[y][x].total_cost = 0;
		}
	}
}

Route::Route(std::list<boost::optional<Position>> list)
{
	insert(end(), list.begin(), list.end());
}
// 探索したルートを表示する
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
			// ゴールの場合はGを出力
			if (end == Position(x, y)) {
				std::cout << " G";
			}
			// スタートの場合はSを出力
			else if (start == Position(x, y)) {
				std::cout << " S";
			}
			// 経路の場合は*を出力
			else if (std::find(begin(), this->end(), Position(x, y)) != this->end()) {
				std::cout << " *";
			}
			// その他の場合は地形を出力
			else {
				std::cout << " " << map->Get(y, x);
			}
		}
		std::cout << std::endl;
	}
}
// 経路復元
Route Route::Create(const Position& start, const Position& end, std::vector<std::vector<boost::optional<Position>>> last_update_positions) {
	Route route;
	route.push_back(end);
	while (1) {
		auto pos = route.front();
		// 終了判定 startまで到達したか？
		if (pos == start) {
			break;		// 終了
		}
		if (pos != boost::none) {
			route.push_front(last_update_positions[pos->y][pos->x]);
		}
		else {
			route.clear();
			break;	// 経路が無かったため終了
		}
	}
	return route;
}

// ノード削除
NodeList::EraseResult NodeList::EraseNode(Node* target_node, float cost) {
	for (auto it = begin(); it != end(); it++) {
		auto node = *it;
		if (node->position == target_node->position)
		{
			if (target_node->total_cost > cost) {
				erase(it);	//削除
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
