/**
 * @file free_block_pickup.cpp
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
#include "fordyca/events/free_block_pickup.hpp"
#include "fordyca/representation/block.hpp"
#include "fordyca/representation/arena_map.hpp"
#include "fordyca/representation/perceived_arena_map.hpp"
#include "fordyca/events/cell_empty.hpp"
#include "fordyca/controller/random_foraging_controller.hpp"
#include "fordyca/controller/memory_foraging_controller.hpp"
#include "fordyca/controller/depth1_foraging_controller.hpp"
#include "fordyca/fsm/block_to_cache_fsm.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, events);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
free_block_pickup::free_block_pickup(
    const std::shared_ptr<rcppsw::common::er_server>& server,
    representation::block* block,  size_t robot_index) :
    cell_op(block->discrete_loc().first,
            block->discrete_loc().second),
    er_client(server),
    m_robot_index(robot_index),
    m_block(block),
    m_server(server) {
  er_client::insmod("free_block_pickup",
                    rcppsw::common::er_lvl::DIAG,
                    rcppsw::common::er_lvl::NOM);
    }

/*******************************************************************************
 * Foraging Support
 ******************************************************************************/
void free_block_pickup::visit(representation::cell2D_fsm& fsm) {
  fsm.event_block_pickup();
} /* visit() */

void free_block_pickup::visit(representation::cell2D& cell) {
  cell.fsm().accept(*this);
  ER_NOM("cell2D: fb%zu block%d from (%zu, %zu)",
         m_robot_index, m_block->id(), m_block->discrete_loc().first,
         m_block->discrete_loc().second);
} /* visit() */

void free_block_pickup::visit(representation::arena_map& map) {
  ER_ASSERT(m_block->discrete_loc() == representation::discrete_coord(cell_op::x(),
                                                                      cell_op::y()),
            "FATAL: Coordinates for block/cell do not agree");
  argos::CVector2 old_r(m_block->real_loc().GetX(),
                        m_block->real_loc().GetY());
  events::cell_empty op(cell_op::x(), cell_op::y());
  map.accept(op);
  m_block->accept(*this);
  ER_NOM("arena_map: fb%zu: block%d from (%f, %f) -> (%zu, %zu)", m_robot_index,
         m_block->id(), old_r.GetX(), old_r.GetY(), cell_op::x(), cell_op::y());
} /* visit() */

/*******************************************************************************
 * Random Foraging
 ******************************************************************************/
void free_block_pickup::visit(representation::block& block) {
  block.add_carry();
  ER_ASSERT(-1 != block.id(), "FATAL: Unamed block");
  block.robot_index(m_robot_index);

  /* Move block out of sight */
  block.move_out_of_sight();
  ER_NOM("block: block%d is now carried by fb%zu",
         m_block->id(), m_robot_index);
} /* visit() */

void free_block_pickup::visit(controller::random_foraging_controller& controller) {
  controller.fsm()->accept(*this);
  controller.block(m_block);
  ER_NOM("random_foraging_controller: %s picked up block%d",
         controller.GetId().c_str(), m_block->id());
} /* visit() */

void free_block_pickup::visit(fsm::random_foraging_fsm& fsm) {
  ER_NOM("random_foraging_fsm: register free_block_pickup event");
  fsm.inject_event(controller::foraging_signal::BLOCK_PICKUP,
                   state_machine::event_type::NORMAL);
} /* visit() */

/*******************************************************************************
 * Depth0 Foraging
 ******************************************************************************/
void free_block_pickup::visit(representation::perceived_cell2D& cell) {
  cell.cell().accept(*this);
} /* visit() */

void free_block_pickup::visit(representation::perceived_arena_map& map) {
  ER_ASSERT(m_block->discrete_loc() == representation::discrete_coord(cell_op::x(),
                                                                      cell_op::y()),
            "FATAL: Coordinates for block/cell do not agree");

  events::cell_empty op(cell_op::x(), cell_op::y());
  map.accept(op);
  ER_NOM("perceived_arena_map: fb%zu: (%zu, %zu) is now empty", m_robot_index,
         cell_op::x(), cell_op::y());
} /* visit() */

void free_block_pickup::visit(fsm::memory_foraging_fsm& fsm) {
  ER_NOM("memory_foraging_fsm: register free_block_pickup event");
  fsm.inject_event(controller::foraging_signal::BLOCK_PICKUP,
                   state_machine::event_type::NORMAL);
} /* visit() */

void free_block_pickup::visit(controller::memory_foraging_controller& controller) {
  controller.map()->accept(*this);
  controller.fsm()->accept(*this);
  controller.block(m_block);
  ER_NOM("memory_foraging_controller: %s picked up block%d",
         controller.GetId().c_str(), m_block->id());
} /* visit() */


/*******************************************************************************
 * Depth1 Foraging
 ******************************************************************************/
void free_block_pickup::visit(controller::depth1_foraging_controller& controller) {
  controller.map()->accept(*this);
  controller.block(m_block);
  controller.current_task()->accept(*this);

  ER_NOM("depth1_foraging_controller: %s picked up block%d",
         controller.GetId().c_str(), m_block->id());
} /* visit() */

void free_block_pickup::visit(tasks::generalist& task) {
  static_cast<fsm::memory_foraging_fsm*>(task.mechanism())->accept(*this);
} /* visit() */

void free_block_pickup::visit(tasks::forager& task) {
  static_cast<fsm::block_to_cache_fsm*>(task.mechanism())->accept(*this);
} /* visit() */

void free_block_pickup::visit(fsm::block_to_cache_fsm& fsm) {
  ER_NOM("block_to_cache_fsm: register free_block_pickup event");
  fsm.inject_event(controller::foraging_signal::BLOCK_PICKUP,
                   state_machine::event_type::NORMAL);
} /* visit() */

void free_block_pickup::visit(fsm::block_to_nest_fsm& fsm) {
  ER_NOM("block_to_nest_fsm: register free_block_pickup event");
  fsm.inject_event(controller::foraging_signal::BLOCK_PICKUP,
                   state_machine::event_type::NORMAL);
} /* visit() */

NS_END(events, fordyca);
