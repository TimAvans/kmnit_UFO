#include "kmint/ufo/door.hpp"

#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {
	door::door(map::map_graph& g, math::vector2d location)
		: play::static_actor{location},
		drawable_{ *this, graphics::image{"resources/free-shield-icon-2.jpg", 0.0} } {}

}
