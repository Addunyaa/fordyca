/**
 * @file grid2D.cpp
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
#include <argos3/core/utility/math/range.h>
#include "fordyca/representation/grid2D.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, representation);

/*******************************************************************************
 * Constructors/Destructors
 ******************************************************************************/
grid2D::grid2D(const grid_params* params) :
    m_resolution(params->resolution),
    m_upper(params->upper),
    m_lower(params->lower),
    m_cells(boost::extents
            [(std::fabs(params->upper.GetX()) - params->lower.GetX())/params->resolution]
            [(params->upper.GetY() - params->lower.GetY())/params->resolution]) {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
std::list<const cell2D*> grid2D::with_blocks(void) {
  std::list<const cell2D*> cells;
  for (auto i = m_cells.origin();
       i < m_cells.origin() + m_cells.num_elements(); ++i) {
    if (i->current_state() == cell2D_fsm::ST_HAS_BLOCK) {
      cells.push_back(i);
    }
  } /* for(i..) */
  return cells;
} /* with_blocks() */

NS_END(representation, fordyca);
