#include "kmint/ufo/spawner.hpp"
#include <random>

#include "kmint/ufo/emp.hpp"
#include "kmint/ufo/shield.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/resources.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"
#include "kmint/ufo/door.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include <kmint/ufo/astar.hpp>

namespace kmint {
	namespace ufo {
		spawner* spawner::Instance()
		{
			static spawner instance;
			return &instance;
		}

		void spawner::remove_actors()
		{
			for (play::actor& a : *stage) {
				if (auto h = dynamic_cast<human*>(&a)) {
					h->remove_human();
				}
			}

			for (play::actor& a : *stage)
			{
				if (auto s = dynamic_cast<saucer*>(&a))
				{
					s->target_ = NULL;
					s->remove();
				}
			}

			for (play::actor& a : *stage)
			{
				if (dynamic_cast<human*>(&a) ||
					dynamic_cast<emp*>(&a) || dynamic_cast<shield*>(&a))
				{
					a.remove();
				}
			}

			for (play::actor& a : *stage) {
				if (auto t = dynamic_cast<tank*>(&a)) {
					t->node(random_node_of_kind(*map, 'T'));
					t->damage_ = 0;
					t->get_state_machine()->ChangeState(wander_state::Instance());
				}
			}

			astar a{ *graph };
			a.clear_all_paths();
		}

		void spawner::spawn_actors() {
			for (std::size_t h{ 0 }; h < 100; ++h) {
				stage->build_actor<human>();
			}

			stage->build_actor<emp>(*graph, random_node_of_kind(*map, 'R'));
			stage->build_actor<emp>(*graph, random_node_of_kind(*map, 'R'));
			stage->build_actor<emp>(*graph, random_node_of_kind(*map, 'R'));

			stage->build_actor<shield>(*graph, random_node_of_kind(*map, 'R'));
			stage->build_actor<shield>(*graph, random_node_of_kind(*map, 'R'));
			stage->build_actor<shield>(*graph, random_node_of_kind(*map, 'R'));
			

			stage->build_actor<saucer>(saucer_type::blue);
			stage->build_actor<saucer>(saucer_type::green);
			stage->build_actor<saucer>(saucer_type::beige);
			stage->build_actor<saucer>(saucer_type::yellow);
		}

		void spawner::set_world(play::stage& stage, map::map& map, map::map_graph& graph)
		{
			this->stage = &stage;
			this->map = &map;
			this->graph = &map.graph();
		}
	}
}