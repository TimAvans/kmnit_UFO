#ifndef KMINT_UFO_SAUCER_HPP
#define KMINT_UFO_SAUCER_HPP
#include "kmint/math/vector2d.hpp"
#include "kmint/play.hpp"
#include <kmint/ufo/steering_behaviour.hpp>
#include <kmint/ufo/moving_entity.hpp>
#include <kmint/ufo/state_machine.hpp>

namespace kmint::ufo {
enum class saucer_type { blue, green, beige, yellow };
class saucer : public moving_entity {
public:
  saucer(saucer_type type);
  saucer_type type() const noexcept { return type_; }
  ui::drawable const &drawable() const override { return drawable_; }
  void act(delta_time dt) override;
  bool incorporeal() const override { return false; }
  scalar collision_range() const override { return 32.0; }
  state_machine<play::free_roaming_actor>* get_state_machine();

  bool perceptive() const override { return true; }

  scalar perception_range() const override { return 200.f; }

  steering_behaviour* get_steering();
  steering_behaviour* steering_;

  void change_color(graphics::color_component r, graphics::color_component g, graphics::color_component b);
  void clear_color();
private:
  play::image_drawable drawable_;
  saucer_type type_;
  math::vector2d v_{};

  state_machine<play::free_roaming_actor>* state_machine_;


};

}
#endif
