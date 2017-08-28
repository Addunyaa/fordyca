/**
 * @file foraging_fsm.hpp
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

#ifndef INCLUDE_FORDYCA_CONTROLLER_FORAGING_FSM_HPP_
#define INCLUDE_FORDYCA_CONTROLLER_FORAGING_FSM_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/rng.h>
#include "rcppsw/patterns/state_machine/simple_fsm.hpp"
#include "fordyca/params/params.hpp"
#include "fordyca/controller/sensor_manager.hpp"
#include "fordyca/controller/actuator_manager.hpp"
#include "rcsw/common/common.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, controller);
namespace fsm = rcppsw::patterns::state_machine;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class foraging_fsm : public fsm::simple_fsm {
 public:
  foraging_fsm(const struct foraging_fsm_params* params,
             std::shared_ptr<rcppsw::common::er_server> server,
             std::shared_ptr<sensor_manager> sensors,
             std::shared_ptr<actuator_manager> actuators);

  void init(void);
  bool is_exploring(void) {return current_state() == ST_EXPLORE; }
  bool is_returning(void) {return current_state() == ST_RETURN_TO_NEST; }
  bool is_avoiding_collision(void) { return current_state() == ST_COLLISION_AVOIDANCE; }
  void event_block_found(void);
  void event_start(void);
  void run(void) { generated_event(true); state_engine(); }

 protected:
  enum fsm_states {
    ST_START,
    ST_EXPLORE,
    ST_NEW_DIRECTION,
    ST_RETURN_TO_NEST,
    ST_LEAVING_NEST,
    ST_COLLISION_AVOIDANCE,
    ST_COLLISION_RECOVERY,
    ST_VECTOR_TO_GOAL,
    ST_MAX_STATES
  };

 private:
  /* types */
  struct new_direction_data : public fsm::event_data {
    explicit new_direction_data(argos::CRadians dir_) : dir(dir_) {}
    argos::CRadians dir;
  };
  struct collision_data : public fsm::event_data {
    explicit collision_data(bool do_recovery_, uint8_t prev_state_ = 0) :
        do_recovery(do_recovery_), prev_state(prev_state_) {}

    bool do_recovery;
    uint8_t prev_state;
  };

  struct goal_data : public fsm::event_data {
    explicit goal_data(argos::CVector2 goal_) : goal(goal_) {}

    argos::CVector2 goal;
  };

  struct fsm_state {
    fsm_state(void) : time_exploring_unsuccessfully(0),
                      last_collision_time(0) {}

    size_t time_exploring_unsuccessfully;
    uint last_collision_time;
  };

  /* constants */
  static int kCOLLISION_RECOVERY_TIME;
  static int kVECTOR_TO_GOAL_MIN_DIFF;

  /* member functions */
  argos::CVector2 randomize_vector_angle(argos::CVector2 vector);

  /* states */
  FSM_STATE_DECLARE(foraging_fsm, start, fsm::no_event_data);
  FSM_STATE_DECLARE(foraging_fsm, explore, fsm::no_event_data);
  FSM_STATE_DECLARE(foraging_fsm, new_direction, new_direction_data);
  FSM_STATE_DECLARE(foraging_fsm, return_to_nest, fsm::no_event_data);
  FSM_STATE_DECLARE(foraging_fsm, leaving_nest, fsm::no_event_data);
  FSM_STATE_DECLARE(foraging_fsm, collision_avoidance, collision_data);
  FSM_STATE_DECLARE(foraging_fsm, collision_recovery, collision_data);
  FSM_STATE_DECLARE(foraging_fsm, vector_to_goal, goal_data);

  FSM_ENTRY_DECLARE(foraging_fsm, entry_explore, fsm::no_event_data);
  FSM_ENTRY_DECLARE(foraging_fsm, entry_new_direction, fsm::no_event_data);
  FSM_ENTRY_DECLARE(foraging_fsm, entry_return_to_nest, fsm::no_event_data);

  FSM_ENTRY_DECLARE(foraging_fsm, entry_collision_avoidance,
                    fsm::no_event_data);
  FSM_ENTRY_DECLARE(foraging_fsm, entry_collision_recovery,
                    fsm::no_event_data);
  FSM_ENTRY_DECLARE(foraging_fsm, entry_vector_to_goal, fsm::no_event_data);
  FSM_ENTRY_DECLARE(foraging_fsm, entry_leaving_nest, fsm::no_event_data);
  FSM_EXIT_DECLARE(foraging_fsm, exit_leaving_nest);

  FSM_DEFINE_STATE_MAP_ACCESSOR(state_map_ex) {
  FSM_DEFINE_STATE_MAP_EX(state_map_ex, kSTATE_MAP) {
        FSM_STATE_MAP_ENTRY_EX_ALL(&start, NULL, NULL, NULL),
        FSM_STATE_MAP_ENTRY_EX_ALL(&explore, NULL, &entry_explore, NULL),
        FSM_STATE_MAP_ENTRY_EX_ALL(&new_direction, NULL,
                                   &entry_new_direction, NULL),
        FSM_STATE_MAP_ENTRY_EX_ALL(&return_to_nest, NULL,
                                   &entry_return_to_nest, NULL),
        FSM_STATE_MAP_ENTRY_EX_ALL(&leaving_nest, NULL,
                                   &entry_leaving_nest, &exit_leaving_nest),
        FSM_STATE_MAP_ENTRY_EX_ALL(&collision_avoidance, NULL,
                                   &entry_collision_avoidance, NULL),
        FSM_STATE_MAP_ENTRY_EX_ALL(&collision_recovery, NULL,
                                   &entry_collision_recovery, NULL),
        FSM_STATE_MAP_ENTRY_EX_ALL(&vector_to_goal, NULL,
                                   &entry_vector_to_goal, NULL),
    };
  FSM_VERIFY_STATE_MAP(state_map_ex, kSTATE_MAP);
    return &kSTATE_MAP[0];
  }

  foraging_fsm(const foraging_fsm& fsm) = delete;
  foraging_fsm& operator=(const foraging_fsm& fsm) = delete;

  /* data members */

  argos::CRandom::CRNG* m_rng;
  struct fsm_state m_state;
  std::shared_ptr<const struct foraging_fsm_params> mc_params;
  std::shared_ptr<sensor_manager> m_sensors;
  std::shared_ptr<actuator_manager> m_actuators;
};

NS_END(controller, fordyca);

#endif /* INCLUDE_FORDYCA_CONTROLLER_FORAGING_FSM_HPP_ */
