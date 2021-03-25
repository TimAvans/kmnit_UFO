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
  // participates in collisions
  bool incorporeal() const override { return false; }
  void act(delta_time dt) override;

  void tag_as_target();
  bool targeted = false;
private:
  play::image_drawable drawable_;
  steering_behaviour* steering_;

};

} // namespace kmint::ufo

#endif /* KMINT_UFO_HUMAN_HPP */
