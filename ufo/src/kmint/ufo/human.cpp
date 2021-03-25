#include "kmint/ufo/human.hpp"
#include "kmint/random.hpp"
#include <string>

namespace kmint::ufo {

namespace {
constexpr char const *image_path = "resources/human.png";

graphics::image human_image() { return graphics::image{image_path}; }

math::vector2d random_location() {
  return {random_scalar(60, 900), random_scalar(60, 700)};
}

} // namespace
human::human()
	: moving_entity(math::vector2d(0, 0), 10, math::vector2d(-20, -20), 1, 2, 50, random_location()),
	drawable_{ *this, human_image() } {

	steering_ = new steering_behaviour(this, 80.0, 2.0, 1.0);
}

void human::tag_as_target() {
	graphics::color color{ 255,0,0 };
	drawable_.set_tint(color);
}

void human::act(delta_time dt) {
	time_elapsed_ = to_seconds(dt);
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


} // namespace kmint::ufo
