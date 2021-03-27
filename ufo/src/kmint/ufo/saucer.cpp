#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/human.hpp"
#include <string>
#include <iostream>
#include <kmint/ufo/node_algorithm.hpp>
#include "kmint/ufo/global_saucer_state.hpp"
#include "kmint/ufo/wander_steering_state.hpp"
#include "kmint/ufo/pursuit_steering_state.hpp"
#include "kmint/ufo/seek_steering_state.hpp"
#include <kmint/ufo/tank.hpp>
#include <kmint/random.hpp>

namespace kmint::ufo {

	namespace {

		char const* color_for(saucer_type type) {
			switch (type) {
			case saucer_type::blue:
				return "Blue";
			case saucer_type::green:
				return "Green";
			case saucer_type::beige:
				return "Beige";
			case saucer_type::yellow:
			default:
				return "Yellow";
			}
		}

		math::vector2d location_for(saucer_type type) {
			return { random_scalar(60, 900), random_scalar(60, 700) };
		}

		graphics::image image_for(saucer_type type) {
			scalar scale = 0.3;
			std::string prefix{ "resources/ship" };
			std::string fn = prefix + color_for(type) + "_manned.png";
			return graphics::image{ fn, scale };
		}

		math::vector2d velocity_for(saucer_type type) {
			switch (type) {
			case saucer_type::blue:
				return { 20.f, 20.f };
			case saucer_type::green:
				return { -20.f, 20.f };
			case saucer_type::beige:
				return { -20.f, -20.f };
			case saucer_type::yellow:
			default:
				return { 20.f, -20.f };
			}
		}
	}

	saucer::saucer(saucer_type type)
		: moving_entity(math::vector2d(0, 0), 20, velocity_for(type), 1, 2, 20, location_for(type)),
		drawable_{ *this, image_for(type) },
		v_{ velocity_for(type) },
		type_{ type } {

		state_machine_ = new state_machine<play::free_roaming_actor>(this);
		state_machine_->SetCurrentState(wander_steering_state::Instance());
		state_machine_->SetGlobalState(global_saucer_state::Instance());


		wander_jitter_ = 0.3;
		wander_radius_ = 30;
		wander_distance_ = 10;

		wander_weight_ = 1;
		wall_avoidance_weight_ = 50;
		seek_weight_ = 1;
		pursuit_weight_ = 1;

		feeler_length_ = 40;

		target_ = nullptr;

		walls_ = {
			wall({0.0f, 768.0f}, {0.0f, 0.0f}),
			wall({1024.0f, 768.0f}, {0.0f, 768.0f}),
			wall({1024.0f, 0.0f}, {1024.0f, 768.0f}),
			wall({0.0f, 0.0f}, {1024.0f, 0.0f})
		};

		steering_ = new steering_behaviour(this);

	}

	steering_behaviour* saucer::get_steering() {
		return steering_;
	}

	void saucer::change_color(graphics::color_component r, graphics::color_component g, graphics::color_component b) {
		graphics::color color{ r,g,b };
		drawable_.set_tint(color);
	}

	void saucer::clear_color() {
		drawable_.remove_tint();
	}

	void saucer::act(delta_time dt) {
		time_elapsed_ = to_seconds(dt);
		math::vector2d old_pos = location();
		math::vector2d steering_force;
		steering_force = state_machine_->Update();
		math::vector2d acceleration = steering_force / mass_;
		velocity_ += acceleration * time_elapsed_;

		if (sqrt(velocity_.x() * velocity_.x() + velocity_.y() * velocity_.y()) > max_force_)
		{
			velocity_ = math::normalize(velocity_);
			velocity_ *= max_force_;
		}

		location(location() + velocity_ * time_elapsed_);

		if ((velocity_.x() * velocity_.x()) + (velocity_.y() * velocity_.y()) > 0) {
			heading_ = math::normalize(velocity_);
			side_ = math::perp(side_);
		}
	}

	state_machine<play::free_roaming_actor>* saucer::get_state_machine() {
		return state_machine_;
	}


}