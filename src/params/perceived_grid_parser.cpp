/**
 * @file perceived_grid_parser.cpp
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
#include "fordyca/params/perceived_grid_parser.hpp"
#include "rcppsw/utils/line_parser.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, params);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void perceived_grid_parser::parse(argos::TConfigurationNode& node) {
  m_params.reset(new struct perceived_grid_params);
  ticpp::Node *arena = node.Parent()->Parent()->NextSibling("arena");
  std::vector<std::string> res;

  rcppsw::utils::line_parser parser(' ');
  res = parser.parse(arena->ToElement()->GetAttribute("size"));
  m_params->grid.resolution = std::atof(argos::GetNode(node,
                                                       "grid").GetAttribute("resolution").c_str());
  m_params->grid.lower.Set(0.3, 0.3);
  m_params->grid.upper.Set(std::atoi(res[0].c_str()) - 0.3,
                      std::atoi(res[1].c_str()) - 0.3);
  m_params->cell_delta = std::atof(argos::GetNode(node,
                                                  "grid").GetAttribute("cell_decay_delta").c_str());
} /* parse() */

void perceived_grid_parser::show(std::ostream& stream) {
  stream << "====================\nPerceived grid params\n====================\n";
  stream << "cell_decay_delta=" << m_params->cell_delta << std::endl;
  stream << "resolution=" << m_params->grid.resolution << std::endl;
  stream << "lower=" << m_params->grid.lower << std::endl;
  stream << "upper=" << m_params->grid.upper << std::endl;
} /* show() */

NS_END(params, fordyca);
