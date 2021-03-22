#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/human.hpp"
#include <string>
#include <iostream>
#include <kmint/ufo/node_algorithm.hpp>

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
            switch (type) {
            case saucer_type::blue:
                return { 30.f, 30.f };
            case saucer_type::green:
                return { 994.f, 30.f };
            case saucer_type::beige:
                return { 994.f, 738.f };
            case saucer_type::yellow:
            default:
                return { 30.f, 738.f };
            }
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

    } // namespace

    saucer::saucer(saucer_type type)
        : moving_entity(math::vector2d(0, 0), 10, velocity_for(type), 1, 2, 50, location_for(type)),
        drawable_{ *this, image_for(type) },
        v_{ velocity_for(type) },
        type_{ type } {

        _steering = new steering_behaviour(this, 80.0, 2.0, 1.0);
    }

    steering_behaviour* saucer::get_steering() {
        return _steering;
    }

void saucer::act(delta_time dt) {
    time_elapsed_ = to_seconds(dt);

    math::vector2d old_pos = location();

    math::vector2d steering_force;

    steering_force = _steering->calculate();

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

  //location(location() + v_ * to_seconds(dt));
  //for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
  //  auto &other = colliding_actor(ix);
  //  if (dynamic_cast<human *>(&other)) {
  //    // remove human from play
	 // //std::cout << "Beam me up scotty\n";
  //    // -- CHANGE THIS TO MORE SUITABLE BEHAVIOR FOR THE ASSESSMENT
  //    other.remove();
  //  }
  //}
}
} // namespace kmint::ufo
