#ifndef KMINT_UFO_ANDRE_HPP
#define KMINT_UFO_ANDRE_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"


namespace kmint::ufo {

class andre : public play::map_bound_actor {
public:
	andre(map::map_graph& g, map::map_node& initial_node);
	void act(delta_time dt) override;
	ui::drawable const& drawable() const override { return drawable_; }
	bool incorporeal() const override { return false; }
	scalar collision_range() const override { return 16.0; }
	bool perceptive() const override { return true; }
	char current_target_ = '1';
	void set_path(std::vector<int> path);
private:
	delta_time t_since_move_{};
	play::image_drawable drawable_;
	map::map_graph* graph_;
	
	std::vector<int> path_;
	std::vector<int> initial_path_;
	std::vector<int> visited_;

};

} 

#endif
