/**
 * @file harvester.hpp
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

#ifndef INCLUDE_FORDYCA_TASKS_HARVESTER_HPP_
#define INCLUDE_FORDYCA_TASKS_HARVESTER_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fordyca/tasks/foraging_task.hpp"
#include "rcppsw/patterns/visitor/visitable.hpp"
#include "rcppsw/task_allocation/abort_probability.hpp"
#include "rcppsw/task_allocation/polled_task.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, tasks);
namespace task_allocation = rcppsw::task_allocation;

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/
/**
 * @class harvester
 * @ingroup tasks
 *
 * @brief Task in which robots locate a free block and bring it to a known
 * cache. It is abortable, and has one task interface.
 */
class harvester : public task_allocation::polled_task, public foraging_task {
 public:
  harvester(const struct task_allocation::task_params* params,
            std::unique_ptr<task_allocation::taskable>& mechanism);

  /* event handling */
  void accept(events::cache_block_drop& visitor) override;
  void accept(events::free_block_pickup& visitor) override;
  void accept(events::cache_vanished& visitor) override;
  void accept(events::cached_block_pickup&) override {}
  void accept(events::nest_block_drop&) override {}

  /* stateless metrics */
  bool is_exploring_for_block(void) const override;
  bool is_avoiding_collision(void) const override;
  bool is_transporting_to_nest(void) const override { return false; }

  /* stateful metrics */
  bool is_acquiring_block(void) const override;
  bool is_vectoring_to_block(void) const override;

  /* depth1 metrics */
  bool is_exploring_for_cache(void) const override;
  bool is_vectoring_to_cache(void) const override;
  bool is_acquiring_cache(void) const override;
  bool is_transporting_to_cache(void) const override;

  /* task metrics */
  bool at_interface(void) const override;

  bool cache_acquired(void) const override;
  bool block_acquired(void) const override;

  void task_start(const task_allocation::taskable_argument*) override;
  double current_time(void) const override;
  double calc_abort_prob(void) override;
  double calc_interface_time(double start_time) override;

 private:
  // clang-format off
  bool                               m_interface_complete{false};
  task_allocation::abort_probability m_abort_prob;
  // clang-format on
};

NS_END(tasks, fordyca);

#endif /* INCLUDE_FORDYCA_TASKS_HARVESTER_HPP_ */
