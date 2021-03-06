/**
 * @file depth0/foraging_sensors.hpp
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

#ifndef INCLUDE_FORDYCA_CONTROLLER_DEPTH0_FORAGING_SENSORS_HPP_
#define INCLUDE_FORDYCA_CONTROLLER_DEPTH0_FORAGING_SENSORS_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fordyca/controller/base_foraging_sensors.hpp"
#include "fordyca/representation/line_of_sight.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, controller, depth0);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/*
 * @class foraging_sensors
 * @ingroup controller
 *
 * @brief The sensors used by depth0 (\ref stateless_foraging_controller,
 * \ref stateful_foraging_controller) controllers.
 */
class foraging_sensors : public base_foraging_sensors {
 public:
  foraging_sensors(
      const struct params::sensor_params* c_params,
      argos::CCI_RangeAndBearingSensor* rabs,
      argos::CCI_FootBotProximitySensor* proximity,
      argos::CCI_FootBotLightSensor* light,
      argos::CCI_FootBotMotorGroundSensor* ground);

  foraging_sensors(const foraging_sensors& sensors) = delete;
  foraging_sensors& operator=(const foraging_sensors& fsm) = delete;

  /**
   * @brief Get the robot's current line-of-sight (LOS)
   *
   * Not used by \ref stateless_foraging_controller.
   */
  const representation::line_of_sight* los(void) const { return m_los.get(); }

  /**
   * @brief Set the robots LOS for the next timestep.
   *
   * This is a hack to make it easy for me to run simulations, as I can computer
   * the line of sight for a robot within the loop functions, and just pass it
   * in here. In real robots this routine would be MUCH messier and harder to
   * work with.
   *
   * @param los The new los
   */
  void los(std::unique_ptr<representation::line_of_sight>& los) {
    m_los = std::move(los);
  }

 private:
  std::unique_ptr<representation::line_of_sight> m_los;
};

NS_END(depth0, controller, fordyca);

#endif /* INCLUDE_FORDYCA_CONTROLLER_DEPTH0_FORAGING_SENSORS_HPP_ */
