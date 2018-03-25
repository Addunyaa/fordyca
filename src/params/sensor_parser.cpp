/**
 * @file sensor_parser.cpp
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
#include "fordyca/params/sensor_parser.hpp"
#include <argos3/core/utility/configuration/argos_configuration.h>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, params);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
constexpr char sensor_parser::kXMLRoot[];

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void sensor_parser::parse(const ticpp::Element& node) {
  ticpp::Element bnode = argos::GetNode(const_cast<ticpp::Element&>(node),
                                        kXMLRoot);
  ticpp::Element pnode = argos::GetNode(bnode, "proximity");

  XML_PARSE_PARAM(pnode, m_params.proximity, go_straight_angle_range);
  XML_PARSE_PARAM(pnode, m_params.proximity, delta);
} /* parse() */

void sensor_parser::show(std::ostream& stream) const {
  stream << emit_header()
         << XML_PARAM_STR(m_params.proximity,
                          go_straight_angle_range) << std::endl
         << XML_PARAM_STR(m_params.proximity, delta) << std::endl;
} /* show() */

__pure bool sensor_parser::validate(void) const {
  if (!(m_params.proximity.go_straight_angle_range.GetSpan().GetAbsoluteValue() >
        0)) {
    return false;
  }
  if (m_params.proximity.delta <= 0) {
    return false;
  }
  return true;
} /* validate() */

NS_END(params, fordyca);
