#pragma once
#include <kmint/ufo/moving_entity.hpp>
#include <kmint/ufo/wall.hpp>
#include <kmint/ufo/geometrics.hpp>

namespace kmint {
	namespace ufo {
		class steering_behaviour {
		private:
			moving_entity *agent_;

			math::vector2d steering_force_;

			std::vector<math::vector2d> feelers_;


			play::actor* seek_target_{};
			play::actor* flee_target_{};

			geometrics geo{};


			void create_feelers();


			//this returns a steering force which will keep the agent away from any
			//walls it may encounter

			float random_clamped();
		
		public:
			steering_behaviour(moving_entity* agent);

			math::vector2d wall_avoidance();
			math::vector2d wander();
			math::vector2d pursuit(play::actor* evader);
			math::vector2d seek(math::vector2d target_location);
			math::vector2d separation(const std::vector<moving_entity*>& neighbours);
			math::vector2d alignment(const std::vector<moving_entity*>& neighbours);
			math::vector2d cohesion(const std::vector<moving_entity*>& neighbours);
			math::vector2d flee(math::vector2d target_location);

			math::vector2d calculate();

			void set_seek_target(play::actor* target);
			void set_flee_target(play::actor* target);
		};
	}
}