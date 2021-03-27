#include "kmint/ufo/shield.hpp"

#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {
	shield::shield(map::map_graph& g, map::map_node& initial_node)
		: play::map_bound_actor{ initial_node },
		drawable_{ *this, graphics::image{"resources/free-shield-icon-2.jpg", 0.1} } {}

} // namespace kmint::ufo
