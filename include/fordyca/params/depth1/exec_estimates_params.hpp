/**
 * @file init_estimates_params.hpp
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

#ifndef INCLUDE_FORDYCA_PARAMS_DEPTH1_INIT_ESTIMATES_PARAMS_HPP_
#define INCLUDE_FORDYCA_PARAMS_DEPTH1_INIT_ESTIMATES_PARAMS_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <argos3/core/utility/math/range.h>
#include "rcppsw/common/base_params.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, params, depth1);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/
/**
 * @struct exec_estimates_params
 * @ingroup params depth1
 *
 * @brief Parameters for initializing execution time estimates of tasks to
 * something within a certain range, which helps to speed swarm convergence a
 * fair bit.
 */
struct exec_estimates_params : public rcppsw::common::base_params {
  exec_estimates_params(void)
      : generalist_range(), harvester_range(), collector_range() {}

  bool enabled{false};
  argos::CRange<double> generalist_range;
  argos::CRange<double> harvester_range;
  argos::CRange<double> collector_range;
};

NS_END(depth1, params, fordyca);

#endif /* INCLUDE_FORDYCA_PARAMS_DEPTH1_INIT_ESTIMATES_PARAMS_HPP_ */
