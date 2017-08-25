/**
 * @file cell2D.hpp
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

#ifndef INCLUDE_FORDYCA_REPRESENTATION_CELL2D_HPP_
#define INCLUDE_FORDYCA_REPRESENTATION_CELL2D_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <utility>
#include "rcppsw/common/common.hpp"
#include "fordyca/representation/cell2D_fsm.hpp"
#include "fordyca/representation/block.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, representation);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Base representation of a cell on a 2D grid. This class represents
 * the ACTUAL state of the grid (i.e. not whatever robots happen to think the
 * state is).
 */
class cell2D {
 public:
  explicit cell2D(
      const std::shared_ptr<rcppsw::common::er_server>& server =
      rcppsw::common::g_null_server) :
      m_entity(nullptr),
      m_fsm(server) { m_fsm.init(); }

  /* events */
  void event_unknown(void) { m_fsm.event_unknown(); m_entity = nullptr; }
  void event_empty(void) { m_fsm.event_empty(); m_entity = nullptr; }
  void event_has_block(representation::block* block) {
    m_fsm.event_has_block();
    m_entity = block;
  }

  /* state inquiry */
  bool state_is_known(void) { return m_fsm.state_is_known(); }
  bool state_has_block(void) { return m_fsm.state_has_block(); }
  bool state_is_empty(void) { return m_fsm.state_is_empty(); }

  void reset(void) { m_fsm.init(); }
  const representation::block* block(void) const {
    return static_cast<representation::block*>(m_entity); }

 protected:
  cell2D_fsm& fsm(void) { return m_fsm; }

 private:
  cell2D(const cell2D& other) = delete;
  cell2D& operator=(const cell2D& other) = delete;

  representation::cell_entity* m_entity;
  cell2D_fsm m_fsm;
};


NS_END(representation, fordyca);


#endif /* INCLUDE_FORDYCA_REPRESENTATION_CELL2D_HPP_ */