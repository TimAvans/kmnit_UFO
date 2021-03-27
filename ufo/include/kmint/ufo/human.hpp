#ifndef KMINT_UFO_HUMAN_HPP
#define KMINT_UFO_HUMAN_HPP

#include "kmint/play.hpp"
#include "kmint/ufo/moving_entity.hpp"
#include "kmint/ufo/steering_behaviour.hpp"
#include <tuple>
#include <vector>

namespace kmint::ufo {

class human : public moving_entity {
public:
  human();
  const ui::drawable& drawable() const override { return drawable_; }
  bool incorporeal() const override { return incorporeal_; }
  scalar collision_range() const override { return 32.0; }

  bool must_draw() const override  { return should_draw_; } ;

  void act(delta_time dt) override;

  bool perceptive() const override { return perceptive_; }
  bool perceivable() const override { return perceivable_; }
  scalar perception_range() const override { return 100.f; }

  void remove_human();

  void tag_as_target();
  void clear_tag();
  bool should_draw_ = true;
  bool perceptive_ = true;
  bool perceivable_ = true;
  bool incorporeal_ = false;
  bool targeted = false;
private:
  play::image_drawable drawable_;
  steering_behaviour* steering_;

  double door_willingness_;
  double greentank_willingness_;
  double redtank_willingness_;
};

}

#endif /* KMINT_UFO_HUMAN_HPP */
