/**
 * @file fsm_params.hpp
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

#ifndef INCLUDE_FORDYCA_PARAMS_FSM_PARAMS_HPP_
#define INCLUDE_FORDYCA_PARAMS_FSM_PARAMS_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <argos3/core/utility/math/vector2.h>
#include "rcppsw/common/base_params.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, params);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/
/**
 * @struct threshold_times
 * @ingroup params
 */
struct threshold_times {
  /*
   * The number of exploration steps without finding block after which the
   * footboot will randomly change direction.
   */
  uint unsuccessful_explore_dir_change;

  /*
   * The number of time steps between two successive collisions that will be
   * considered excessive, and result in a random direction being added to the
   * avoidance heading to help avoid collisions in the immediate future.
   */
  uint frequent_collision_thresh;
};

/**
 * @struct speed_throttling
 * @ingroup params
 */
struct speed_throttling {
  double block_carry{0.0};
};

/**
 * @struct fsm_params
 * @ingroup params
 */
struct fsm_params : public rcppsw::common::base_params {
  fsm_params(void) : times(), speed_throttling(), nest_center() {}
  struct threshold_times times;
  struct speed_throttling speed_throttling;
  argos::CVector2 nest_center;
};

NS_END(params, fordyca);

#endif /* INCLUDE_FORDYCA_PARAMS_FSM_PARAMS_HPP_ */
