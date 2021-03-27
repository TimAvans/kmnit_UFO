#include "kmint/ufo/moving_entity.hpp"
#include <kmint/ufo/human.hpp>

namespace kmint {
	namespace ufo {
		std::vector<moving_entity*> moving_entity::find_neighbours() {
			std::vector<moving_entity*> neighbours;

			for (auto i = begin_perceived(); i != end_perceived(); ++i) {
				auto& a = *i;
				if (auto h = dynamic_cast<human*>(&a)) {
					neighbours.push_back(h);
				}
			}
			return neighbours;
		}
	}
}