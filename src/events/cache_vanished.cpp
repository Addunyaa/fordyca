/**
 * @file cache_vanished.cpp
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
#include "fordyca/events/cache_vanished.hpp"
#include "fordyca/controller/depth1/foraging_controller.hpp"
#include "fordyca/fsm/block_to_nest_fsm.hpp"
#include "fordyca/fsm/depth1/block_to_cache_fsm.hpp"

#include "fordyca/tasks/collector.hpp"
#include "fordyca/tasks/harvester.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, events);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
cache_vanished::cache_vanished(const std::shared_ptr<rcppsw::er::server>& server,
                               uint cache_id)
    : client(server), m_cache_id(cache_id) {
  client::insmod("cache_vanished",
                 rcppsw::er::er_lvl::DIAG,
                 rcppsw::er::er_lvl::NOM);
}

/*******************************************************************************
 * Depth1 Foraging
 ******************************************************************************/
void cache_vanished::visit(controller::depth1::foraging_controller& controller) {
  ER_NOM("%s abort pickup/drop from/in cache: cache%d vanished",
         controller.GetId().c_str(),
         m_cache_id);
  controller.current_task()->accept(*this);
} /* visit() */

void cache_vanished::visit(tasks::collector& task) {
  static_cast<fsm::block_to_nest_fsm*>(task.mechanism())->accept(*this);
} /* visit() */

void cache_vanished::visit(tasks::harvester& task) {
  static_cast<fsm::depth1::block_to_cache_fsm*>(task.mechanism())->accept(*this);
} /* visit() */

void cache_vanished::visit(fsm::block_to_nest_fsm& fsm) {
  fsm.inject_event(controller::foraging_signal::CACHE_VANISHED,
                   state_machine::event_type::NORMAL);
} /* visit() */

void cache_vanished::visit(fsm::depth1::block_to_cache_fsm& fsm) {
  fsm.inject_event(controller::foraging_signal::CACHE_VANISHED,
                   state_machine::event_type::NORMAL);
} /* visit() */

NS_END(events, fordyca);
