#include "kmint/ufo/human.hpp"
#include "kmint/random.hpp"
#include <string>
#include <kmint/ufo/saucer.hpp>
#include <kmint/ufo/door.hpp>
#include <kmint/ufo/tank.hpp>

namespace kmint::ufo {

	namespace {
		constexpr char const* image_path = "resources/human.png";

		graphics::image human_image() { return graphics::image{ image_path }; }

		math::vector2d random_location() {
			std::vector<math::vector2d> boxes = {
				{random_scalar(60, 568), random_scalar(0, 456)},
				{random_scalar(60, 568), random_scalar(0, 456)},
				{random_scalar(60, 568), random_scalar(0, 456)},
				{random_scalar(60, 568), random_scalar(0, 456)},
				{random_scalar(60, 568), random_scalar(0, 456)},
				{random_scalar(60, 312), random_scalar(456, 700)},
				{random_scalar(740, 900), random_scalar(0, 144)},
				{random_scalar(676, 900), random_scalar(144, 700)},
				{random_scalar(676, 900), random_scalar(144, 700)},
				{random_scalar(676, 900), random_scalar(144, 700)},
				{random_scalar(536, 696), random_scalar(504, 696)},
				{random_scalar(312, 696), random_scalar(632, 696)},
			};

			return boxes[random_int(0, boxes.size())];
		}

	} // namespace
	human::human()
		: moving_entity(math::vector2d(0, 0), 30, math::vector2d(-20, -20), 1, 10, 70, random_location()),
		drawable_{ *this, human_image() } {


		wander_jitter_ = 10;
		wander_radius_ = 25;
		wander_distance_ = 1;

		separation_weight_ = random_scalar(0.0, 1.0);
		cohesion_weight_ = random_scalar(0.0, 1.0);
		alignment_weight_ = random_scalar(0.0, 1.0);
		wander_weight_ = 1;
		wall_avoidance_weight_ = 500;
		seek_weight_ = 1;
		flee_weight_ = 1;
		pursuit_weight_ = 1;

		feeler_length_ = 40;

		door_willingness_ = random_scalar(-1.0, 1.0);
		greentank_willingness_ = random_scalar(-1.0, 1.0);
		redtank_willingness_ = random_scalar(-1.0, 1.0);

		walls_ = {
			wall({0.0f, 768.0f}, {0.0f, 0.0f}),
			wall({1024.0f, 768.0f}, {0.0f, 768.0f}),
			wall({1024.0f, 0.0f}, {1024.0f, 768.0f}),
			wall({0.0f, 0.0f}, {1024.0f, 0.0f}),

			//gebouwcomplex 1
			wall({584, 72}, {584, 136}),
			wall({616, 136}, {616, 120}),
			wall({616, 120}, {664, 120}),
			wall({712, 120}, {728, 120}),
			wall({728, 120}, {728, 72}),
			wall({782, 72}, {584, 72}),

			//gebouwcomplex 2
			wall({584, 216}, {584, 312}),
			wall({584, 312}, {600, 312}),
			wall({648, 312}, {664, 312}),
			wall({664, 312}, {664, 216}),
			wall({664, 216}, {584, 216}),

			//gebouwcomplex 3
			wall({584, 408}, {584, 488}),
			wall({584, 488}, {616, 488}),
			wall({654, 488}, {680, 488}),
			wall({680, 488}, {680, 408}),
			wall({680, 408}, {584, 408}),

			//gebouwcomplex 4
			wall({328, 520}, {328, 632}),
			wall({328, 632}, {376, 632}),
			wall({424, 632}, {424, 616}),
			wall({424, 616}, {440, 616}),
			wall({472, 616}, {504, 616}),
			wall({504, 616}, {504, 472}),
			wall({504, 472}, {440, 472}),
			wall({440, 472}, {440, 520}),
			wall({440, 520}, {328, 520})
		};

		steering_ = new steering_behaviour(this);
	}

	void human::tag_as_target() {
		graphics::color color{ 255,0,0 };
		drawable_.set_tint(color);
	}

	void human::clear_tag() {
		drawable_.remove_tint();
	}

	void human::act(delta_time dt) {
		time_elapsed_ = to_seconds(dt);

		for (auto i = begin_perceived(); i != end_perceived(); ++i) {
			auto& a = *i;
			if (auto s = dynamic_cast<saucer*>(&a)) {
				steering_->set_flee_target(s);
			}
			else if (auto d = dynamic_cast<door*>(&a)) {
				seek_weight_ = door_willingness_;
				steering_->set_seek_target(d);
			}
			else if (auto t = dynamic_cast<tank*>(&a)) {
				if (t->type_ == tank_type::green) {
					seek_weight_ = greentank_willingness_;
				}
				else if (t->type_ == tank_type::red) {
					seek_weight_ = redtank_willingness_;
				}
				steering_->set_seek_target(t);
			}
		}

		for (auto i = begin_collision(); i != end_collision(); ++i) {
			auto& a = *i;
			if (dynamic_cast<door*>(&a)) {
				remove_human();
			}
		}

		math::vector2d old_pos = location();
		math::vector2d steering_force;
		steering_force = steering_->calculate();
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

	void human::remove_human() {
		perceivable_ = false;
		perceptive_ = false;
		incorporeal_ = false;
		should_draw_ = false;
		steering_->set_flee_target(NULL);
		steering_->set_seek_target(NULL);
	}

} // namespace kmint::ufo
