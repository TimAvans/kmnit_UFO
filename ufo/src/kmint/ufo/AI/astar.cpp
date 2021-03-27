#include "kmint/ufo/astar.hpp"
#include <queue>
#include <map>
#include <iostream>

namespace kmint {
	namespace ufo {
		int astar::calculate_hcosts(int node, int target) {
			//manhatten distance
			int h = (abs(_graph[node].location().x() - _graph[target].location().x()) +
				abs(_graph[node].location().y() - _graph[target].location().y())) / 16;
			return h;
		}

		std::vector<std::vector<int>> astar::search(int source, int target) {
			//list of accumulate cost --> index is node_id
			std::vector<int> g_costs(_graph.num_nodes());
			//list of node with previous nodes
			std::vector<int> previous_nodes(_graph.num_nodes());

			//setting all distances to max
			for (int i = 0; i < g_costs.size(); ++i) {
				g_costs[i] = INT_MAX;
			}

			g_costs[source] = 0;
			std::vector<int> visited;
			std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>> unvisited;

			unvisited.push(std::make_pair(g_costs[source], source));

			while (unvisited.size() > 0) {
				//select nearest node
				int current_node = unvisited.top().second;
				unvisited.pop();

				//if target is found --> exit
				if (current_node == target) {
					while (!unvisited.empty()) {
						unvisited.pop();
					}
					break;
				}

				//iterate through all edges of current_node
				for (auto edge : _graph[current_node]) {

					//if next node is not already visited
					if (std::find(visited.begin(), visited.end(), edge.to().node_id()) == visited.end()) {

						int target_id = edge.to().node_id();

						//calculate different costs
						int g_cost = g_costs[current_node] + edge.weight();
						int h_prev_cost = calculate_hcosts(current_node, target);
						int h_cost = calculate_hcosts(target_id, target);

						//check to see if distance should be updated
						if (g_cost < g_costs[target_id]) {
							g_costs[target_id] = g_cost;
							previous_nodes[target_id] = current_node;

							//mark target_node as unvisited
							unvisited.push(std::make_pair(g_cost + h_cost, target_id));
						}
					}
				}
				visited.push_back(current_node);

			}


			//create shortest path
			int n = target;
			std::vector<int> path;

			while (n != source) {
				path.push_back(n);
				n = previous_nodes[n];
			}

			

			return { path, visited };
		}

		void astar::show_shortest_path(std::vector<int> path, std::vector<int> visited) {
			//give visited color
			for (int j : visited)
			{
				if (_graph[j].tag() != graph::node_tag::path) {
					_graph[j].tag(graph::node_tag::visited);
				}
			}
			//give path color
			for (int i : path) {
				_graph[i].tag(graph::node_tag::path);
			}

		}

		void astar::clear_path_color(std::vector<int> path, std::vector<int> visited) {

			for (int j : visited)
			{
				_graph[j].tag(graph::node_tag::normal);
			}

			for (int i : path) {
				_graph[i].tag(graph::node_tag::normal);
			}
		}

		void astar::clear_all_paths() {
			_graph.untag_all();
		}
	}
}
