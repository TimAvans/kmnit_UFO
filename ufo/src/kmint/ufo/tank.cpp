#include "kmint/ufo/tank.hpp"
#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"

#include "kmint/ufo/global_tank_state.hpp"

#include <iostream>
#include <kmint/ufo/saucer.hpp>
#include <kmint/ufo/astar.hpp>

namespace kmint::ufo {

	namespace {
		graphics::image tank_image(tank_type t) {
			constexpr scalar scale = 0.35;
			switch (t) {
			case tank_type::red:
				return graphics::image{ "resources/tank_red.png", scale };
			case tank_type::green:
				return graphics::image{ "resources/tank_green.png", scale };
			}
			return graphics::image{ "resources/tank_green.png", scale };
		}
	} // namespace

	tank::tank(map::map_graph& g, map::map_node& initial_node, tank_type t, play::stage& s, andre& andre) : state_user{ g, initial_node, graphics::image{tank_image(t)} }, type_{ t }, graph_{ &g }, stage_{ &s }, andre_{&andre}
	{
		state_machine_->SetGlobalState(global_tank_state::Instance());
		state_machine_->SetCurrentState(wander_state::Instance());

		damage_ = 0;

		emp_shield = "";
		curr_action_ = actions::FLEE;
	}

	void tank::act(delta_time dt) {
		t_since_move_ += dt;
		t_since_hit += dt;

		if (to_seconds(t_since_hit) >= 1) {
			been_hit = false;
			t_since_hit = from_seconds(0);
		}

		if (to_seconds(t_since_move_) >= waiting_time(node())) {
			get_state_machine()->Update();
			t_since_move_ = from_seconds(0);
		}


		// laat ook zien wat hij ziet
		for (auto i = begin_perceived(); i != end_perceived(); ++i) {
			auto& a = *i;
			if (dynamic_cast<saucer*>(&a)) {
				get_state_machine()->ChangeState();
			}
		}
	}

	void tank::take_damage() {
		int added_damage = 0;

		if (!been_hit) {
			been_hit = true;
			if (emp_shield == "SHIELD") {
				added_damage = 20;
				get_state_machine()->change_chances(actions::SHIELD, added_damage);
			}
			else if (emp_shield == "EMP") {
				added_damage = 0;
				get_state_machine()->change_chances(actions::EMP, added_damage);
			}
			else {
				added_damage = 50;
				get_state_machine()->change_chances(actions::FLEE, added_damage);
			}

			emp_shield = "";
			damage_ += added_damage;

			std::cout << "took " << added_damage << " damage: total: " << damage_<< std::endl;

		}
	}

	play::map_bound_actor* tank::find_closest_target(std::string target_type) {
		//zoeken naar alle items en nodes van items opslaan in vector
		std::vector<play::map_bound_actor*> actors;

		actors = find_actors(*stage_, target_type);
		astar a{ *graph_ };

		int shortest_length = INT_MAX;
		std::vector<int> shortest_path = {};
		play::map_bound_actor* shortest_actor = nullptr;

		//voor elke node in vector, doe astar
		for (play::map_bound_actor* actor : actors) {
			std::vector<int> path = a.search(node().node_id(), actor->node().node_id())[0];

			int length = 0;
			for (int node : path) {
				length += graph_[0][node][0].weight();
			}

			if (length < shortest_length) {
				shortest_length = length;
				shortest_path = path;
				shortest_actor = actor;
			}
		}

		set_path(shortest_path);
		a.show_shortest_path(path_, visited_);

		return shortest_actor;
	}

	void tank::pick_up_item(play::map_bound_actor& object, std::string type) {
		emp_shield = type;
		object.remove();
	}

} // namespace kmint::ufo
