#ifndef KMINT_UFO_TANK_HPP
#define KMINT_UFO_TANK_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"

#include "kmint/ufo/state_user.hpp"
#include <kmint/ufo/andre.hpp>

namespace kmint {
namespace ufo {

enum class tank_type { red, green };

class tank : public state_user {
public:
  tank(map::map_graph& g, map::map_node& initial_node, tank_type t, play::stage& s, andre& andre);
  void act(delta_time dt) override;
  scalar collision_range() const override { return 16.0; }
  bool perceptive() const override { return true; }
  scalar perception_range() const override { return 200.f; }

  play::map_bound_actor* find_closest_target(std::string target_type);
  void pick_up_item(play::map_bound_actor& object, std::string type);
  tank_type type_;

  void take_damage();
  int damage_;
  actions curr_action_;
  bool been_hit = false;

  play::map_bound_actor* current_target_object_;
  char current_target_;
  play::stage* stage_;

  andre* andre_;
private:
	delta_time t_since_move_{};
	delta_time t_since_hit{};
  map::map_graph* graph_;

  std::string emp_shield;
};

} 
} 

#endif /* KMINT_UFO_TANK_HPP */