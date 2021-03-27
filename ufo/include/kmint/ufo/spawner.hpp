#pragma once
#include "kmint/play.hpp"
#include <kmint/map/map.hpp>
#include <kmint\ufo\resources.hpp>
#include "andre.hpp"

namespace kmint {
	namespace ufo {
		class spawner {
		public:
			static spawner* Instance();
			void spawn_actors();
			void set_world(play::stage& stage, map::map& map, map::map_graph& graph);
			void remove_actors();
		private:
			play::stage* stage;
			map::map* map;
			map::map_graph* graph;
			spawner() {}
		};
	}
}