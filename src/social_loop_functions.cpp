/**
 * @file social_loop_functions.cpp
 *
 * @copyright 2017 John Harwell, All rights reserved.
 *
 * This file is part of FORDYCA.
 *
 * FORDYCA is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * FORDYCA is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * FORDYCA.  If not, see <http://www.gnu.org/licenses/
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include "fordyca/social_loop_functions.hpp"
#include "fordyca/social_foraging_controller.hpp"
#include "fordyca/food_param_parser.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
social_loop_functions::social_loop_functions(void) :
    m_arena_x(-0.9f, 1.7f),
    m_arena_y(-1.7f, 1.7f),
    m_food_pos(),
    m_floor(NULL),
    m_rng(NULL),
    m_ofname(),
    m_ofile(),
    m_total_collected_blocks(0),
    m_energy(0),
    m_energy_per_moving_robot(1),
    m_food_params(),
    m_param_manager() {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void social_loop_functions::Init(argos::TConfigurationNode& t_node) {
  argos::TConfigurationNode& foraging = argos::GetNode(t_node, "foraging");
  m_param_manager.add_category("food", new food_param_parser());
  m_param_manager.parse_all(foraging);

  m_floor = &GetSpace().GetFloorEntity();
  m_rng = argos::CRandom::CreateRNG("argos");

  /*
   * Distribute food items uniformly in the arena.
   */
  for (size_t i = 0; i < m_food_params.n_items; ++i) {
    m_food_pos.push_back(
        argos::CVector2(m_rng->Uniform(m_arena_x),
                        m_rng->Uniform(m_arena_y)));
  } /* for(i..) */

  /* Open output file and truncate */
  argos::GetNodeAttribute(foraging, "output", m_ofname);
  m_ofile.open(m_ofname.c_str(), std::ios_base::trunc | std::ios_base::out);

  m_ofile << "# clock\twalking\tresting\tcollected_food\tenergy" << std::endl;

  /* Get energy loss per moving robot */
  argos::GetNodeAttribute(foraging, "energy_per_moving_robot", m_energy_per_moving_robot);
}

void social_loop_functions::Reset() {
  m_total_collected_blocks = 0;
  m_energy = 0;
  m_ofile.close();
  m_ofile.open(m_ofname.c_str(), std::ios_base::trunc | std::ios_base::out);
  m_ofile << "# clock\twalking\tresting\tcollected_food\tenergy" << std::endl;

  for (size_t i = 0; i < m_food_pos.size(); ++i) {
    m_food_pos[i].Set(m_rng->Uniform(m_arena_x),
                      m_rng->Uniform(m_arena_y));
  } /* for(i..) */
}

void social_loop_functions::Destroy() {
  m_ofile.close();
}

argos::CColor social_loop_functions::GetFloorColor(const argos::CVector2& plane_pos) {
  if (plane_pos.GetX() < -1.0f) {
    return argos::CColor::GRAY50;
  }
  for (size_t i = 0; i < m_food_pos.size(); ++i) {
    if ((plane_pos - m_food_pos[i]).SquareLength() < m_food_params.square_radius) {
      return argos::CColor::BLACK;
    }
  }
  return argos::CColor::WHITE;
} /* get_floor_color() */

void social_loop_functions::PreStep() {
  /*
   * If a robot is in the nest, drop the food item
   * If a robot is on a food item, pick it
   * Each robot can carry only one food item per time
   */
  uint n_moving = 0;
  uint n_resting = 0;
  int i = 0;

  /* Check whether a robot is on a food item */
  argos::CSpace::TMapPerType& footbots = GetSpace().GetEntitiesByType("foot-bot");

  for (argos::CSpace::TMapPerType::iterator it = footbots.begin();
       it != footbots.end();
       ++it) {
    argos::CFootBotEntity& cFootBot = *argos::any_cast<argos::CFootBotEntity*>(it->second);
    social_foraging_controller& controller = dynamic_cast<social_foraging_controller&>(cFootBot.GetControllableEntity().GetController());

    /* Count how many foot-bots are in which state */
    if (!controller.is_resting()) {
      ++n_moving;
    } else {
      ++n_resting;
    }
    /* Get the position of the foot-bot on the ground as a CVector2 */
    argos::CVector2 pos;
    pos.Set(cFootBot.GetEmbodiedEntity().GetOriginAnchor().Position.GetX(),
             cFootBot.GetEmbodiedEntity().GetOriginAnchor().Position.GetY());
    if (controller.carrying_block()) {
      printf("Robot %d carrying block\n", i);
      /* TODO: possibly change this to be autonomous, rather than just
       * informing the robot... */
      /* Check whether the foot-bot is in the nest */
      if(pos.GetX() < -1.0f) {
        controller.publish_event(social_foraging_controller::ENTERED_NEST);

        /* Drop the food item */
        controller.drop_block_in_nest();
        m_energy += m_food_params.energy_per_item;
        ++m_total_collected_blocks;

        /* Place a new food item on the ground */
        m_food_pos[controller.block_idx()].Set(m_rng->Uniform(m_arena_x),
                                                 m_rng->Uniform(m_arena_y));

        /* The floor texture must be updated */
        m_floor->SetChanged();
      }
    } else { /* The foot-bot has no food item */
      printf("Robot %d not carrying block\n", i);
      if (pos.GetX() > -1.0f) {
        /* Check whether the foot-bot is on a food item */
        for (size_t i = 0; i < m_food_pos.size(); ++i) {
          if((pos - m_food_pos[i]).SquareLength() < m_food_params.square_radius) {
            /* If so, we move that item out of sight */
            m_food_pos[i].Set(100.0f, 100.f);
            controller.pickup_block(i);

            /* The floor texture must be updated */
            m_floor->SetChanged();
            controller.publish_event(social_foraging_controller::BLOCK_FOUND);
            break;
          }
        } /* for(i..) */
      }
    }
    ++i;
  } /* for(it..) */

  /* Update Energy expediture due to moving robots */
  m_energy -= n_moving * m_energy_per_moving_robot;
  /* Output stuff to file */
  m_ofile << GetSpace().GetSimulationClock() << "\t"
          << n_moving << "\t"
          << n_resting << "\t"
          << m_total_collected_blocks << "\t"
          << m_energy << std::endl;
}
using namespace argos;
REGISTER_LOOP_FUNCTIONS(social_loop_functions, "social_loop_functions")

NS_END(fordyca);
