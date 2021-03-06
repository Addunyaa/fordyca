/**
 * @file metrics_parser.cpp
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
#include "fordyca/params/metrics_parser.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, params);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void metrics_parser::parse(argos::TConfigurationNode& node) {
  m_params = rcppsw::make_unique<struct metrics_params>();

  argos::GetNodeAttribute(node, "output_dir", m_params->output_dir);

  argos::GetNodeAttribute(node, "stateless_fname", m_params->stateless_fname);
  argos::GetNodeAttribute(node, "distance_fname", m_params->distance_fname);
  argos::GetNodeAttribute(node, "stateful_fname", m_params->stateful_fname);
  argos::GetNodeAttribute(node, "depth1_fname", m_params->depth1_fname);
  argos::GetNodeAttribute(node, "block_fname", m_params->block_fname);
  argos::GetNodeAttribute(node,
                          "task_execution_fname",
                          m_params->task_execution_fname);
  argos::GetNodeAttribute(node,
                          "task_management_fname",
                          m_params->task_management_fname);
  argos::GetNodeAttribute(node, "cache_fname", m_params->cache_fname);
  argos::GetNodeAttribute(node, "collect_interval", m_params->collect_interval);
} /* parse() */

void metrics_parser::show(std::ostream& stream) {
  stream << "====================\nMetrics params\n====================\n";
  if (nullptr != m_params) {
    stream << "output_dir=" << m_params->output_dir << std::endl;

    stream << "stateless_fname=" << m_params->stateless_fname << std::endl;
    stream << "stateful_fname=" << m_params->stateful_fname << std::endl;
    stream << "distance_fname=" << m_params->distance_fname << std::endl;
    stream << "depth1_fname=" << m_params->depth1_fname << std::endl;
    stream << "block_fname=" << m_params->block_fname << std::endl;
    stream << "task_execution_fname=" << m_params->task_execution_fname
           << std::endl;
    stream << "task_management_fname=" << m_params->task_management_fname
           << std::endl;
    stream << "collect_interval=" << m_params->collect_interval << std::endl;
  }
} /* show() */

__pure bool metrics_parser::validate(void) {
  return (0 != m_params->collect_interval);
} /* validate() */

NS_END(params, fordyca);
