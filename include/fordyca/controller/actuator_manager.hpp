/**
 * @file actuator_manager.hpp
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

#ifndef INCLUDE_FORDYCA_CONTROLLER_ACTUATOR_MANAGER_HPP_
#define INCLUDE_FORDYCA_CONTROLLER_ACTUATOR_MANAGER_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <argos3/core/utility/math/vector2.h>
#include "rcppsw/patterns/state_machine/simple_fsm.hpp"
#include "fordyca/params/params.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, controller);
namespace fsm = rcppsw::patterns::state_machine;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class actuator_manager: public fsm::simple_fsm {
 public:
  /* constructors */
  actuator_manager(const struct actuator_params* params,
                   argos::CCI_DifferentialSteeringActuator* const wheels,
                   argos::CCI_LEDsActuator* const leds,
                   argos::CCI_RangeAndBearingActuator* const raba);

  /* member functions */
  void leds_set_color(const argos::CColor& color) {
    m_leds->SetAllColors(color);
  }

  /*
   * Gets a direction vector as input and transforms it into wheel
   * actuation.
   */
  void set_heading(const argos::CVector2& heading,
                   bool force_hard_turn = false);

  argos::Real max_wheel_speed(void) { return mc_params->wheels.max_speed; }
  void stop_wheels(void) { m_wheels->SetLinearVelocity(0.0f, 0.0f); }
  void set_raba_data(int data) { m_raba->SetData(0, data); }
  void reset(void);

 private:
  actuator_manager(const actuator_manager& fsm) = delete;
  actuator_manager& operator=(const actuator_manager& fsm) = delete;
  void set_wheel_speeds(double speed1, double speed2, argos::CRadians heading);

  /*
   * The robot can be in three different turning states.
   */
  enum fsm_states {
    ST_NO_TURN,     /* go straight */
    ST_SOFT_TURN,   /* both wheels are turning forwards, but at different speeds */
    ST_HARD_TURN,   /* wheels are turning with opposite speeds */
    ST_MAX_STATES
  };

  struct turn_data : public fsm::event_data {
    turn_data(argos::CVector2 heading_, bool force_hard_) :
        heading(heading_), force_hard(force_hard_) {}
    argos::CVector2 heading;
    bool force_hard;
  };
  FSM_STATE_DECLARE(actuator_manager, no_turn, turn_data);
  FSM_STATE_DECLARE(actuator_manager, soft_turn, turn_data);
  FSM_STATE_DECLARE(actuator_manager, hard_turn, turn_data);
  FSM_DEFINE_STATE_MAP_ACCESSOR(state_map) {
    FSM_DEFINE_STATE_MAP(state_map, kSTATE_MAP) {
      FSM_STATE_MAP_ENTRY(&no_turn),
          FSM_STATE_MAP_ENTRY(&soft_turn),
          FSM_STATE_MAP_ENTRY(&hard_turn),
          };
    FSM_VERIFY_STATE_MAP(state_map, kSTATE_MAP);
    return &kSTATE_MAP[0];
  }

  argos::CCI_DifferentialSteeringActuator* m_wheels;  /* differential steering */
  argos::CCI_LEDsActuator*                 m_leds;    /* LEDs  */
  argos::CCI_RangeAndBearingActuator*      m_raba;    /* Range and bearing */
  std::shared_ptr<const struct actuator_params>  mc_params;

};

NS_END(controller, fordyca);

#endif /* INCLUDE_FORDYCA_CONTROLLER_ACTUATOR_MANAGER_HPP_ */