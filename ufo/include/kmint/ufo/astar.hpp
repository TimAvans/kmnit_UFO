#pragma once

#include "kmint/map/map.hpp"
#include <map>
#include <queue>

namespace kmint {
	namespace ufo {
		class astar {
		public:
			astar(map::map_graph& graph) :_graph(graph) {}
			std::vector<std::vector<int>> search(int source, int target);
			void show_shortest_path(std::vector<int> path, std::vector<int> visited);
			void clear_path_color(std::vector<int> path, std::vector<int> visited);

		private:
			typedef std::pair<int, int> iPair;

			map::map_graph& _graph;

			//Calculate Distance between current_node and target_node in steps
			int calculate_hcosts(int node, int _target);
			//Get location of given node
			kmint::math::vector2d GetNodeLocation(int node_id);

		};
	}
}