#include "kmint/ufo/emp.hpp"

#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {
	emp::emp(map::map_graph& g, map::map_node& initial_node)
		: play::map_bound_actor{ initial_node },
		drawable_{ *this, graphics::image{"resources/emp.png", 0.5} } {}
} // namespace kmint::ufo
