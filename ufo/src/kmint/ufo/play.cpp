#include "kmint/ufo/play.hpp"
#include "kmint/main.hpp" // voor de main loop
#include "kmint/play.hpp"
#include "kmint/map/map.hpp"

#include "kmint/ufo/resources.hpp"
#include "kmint/ufo/door.hpp"
#include "kmint/ui.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

#include "kmint/ufo/spawner.hpp"
#include <kmint/ufo/human.hpp>
#include <kmint/ufo/node_algorithm.cpp>
#include <kmint/ufo/tank.hpp>

namespace kmint::ufo {

int play() {
  // een app object is nodig om
  ui::app app{};

  //  maak een venster aan
  ui::window window{app.create_window({1024, 768}, "kmint", 1.0)};

  // maak een podium aan
  play::stage s{ {1024, 768} };

  auto m = map();
  auto& graph = m.graph();

  s.build_actor<play::background>(math::size(1024, 768),
                                  graphics::image{m.background_image()});
  s.build_actor<play::map_actor>(math::vector2d{0.f, 0.f}, m.graph());

  std::vector<math::vector2d> doors = {
      {680, 120},
      {696, 120},
      {600, 136},
      {616, 312},
      {632, 312},
      {632, 488},
      {648, 488},
      {456, 616},
      {392, 632},
      {408, 632}
  };

  for (std::size_t i{ 0 }; i < doors.size(); ++i) {
      s.build_actor<ufo::door>(graph, doors[i]);
  }

  auto& dre = s.build_actor<andre>(graph, random_node_of_kind(m, 'R'));
  s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'), tank_type::red, s, dre);
  s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'), tank_type::green, s, dre);


  ufo::spawner::Instance()->set_world(s, m, graph);
  ufo::spawner::Instance()->spawn_actors();

  bool should_spawn = false;

  // Maak een event_source aan (hieruit kun je alle events halen, zoals
  // toetsaanslagen)
  ui::events::event_source event_source{};
  delta_time time_passed{};
  // main_loop stuurt alle actors aan.
  main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
      time_passed += dt;
    // gebruik dt om te kijken hoeveel tijd versterken is
    // sinds de vorige keer dat deze lambda werd aangeroepen
    // loop controls is een object met eigenschappen die je kunt gebruiken om de
    // main-loop aan te sturen.

      if (should_spawn) {
          ufo::spawner::Instance()->spawn_actors();
          should_spawn = false;
      }

    if (to_seconds(time_passed) >= 200) {
        ufo::spawner::Instance()->remove_actors();
        time_passed = from_seconds(0);
        should_spawn = true;
    }


    for (ui::events::event &e : event_source) {
      // event heeft een methode handle_quit die controleert
      // of de gebruiker de applicatie wilt sluiten, en zo ja
      // de meegegeven functie (of lambda) aanroept om met het
      // bijbehorende quit_event
      //
      
       
      e.handle_quit([&ctl](ui::events::quit_event qe) { ctl.quit = true; });
      e.handle_key_up([&](auto ke) {
        switch (ke.key) {
        case ui::events::key::p:
          ctl.pause = !ctl.pause;
          break;
        case ui::events::key::r:
          ctl.render = !ctl.render;
          break;
        case ui::events::key::opening_bracket:
          ctl.time_scale /= 2.0;
          break;
        case ui::events::key::closing_bracket:
          ctl.time_scale *= 2.0;
          break;
        default:
          break;
        }
      });
    }
  });
  return 0;
}

}
