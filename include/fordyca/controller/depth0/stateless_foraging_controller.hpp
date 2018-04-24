/**
 * @file stateless_foraging_controller.hpp
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

#ifndef INCLUDE_FORDYCA_CONTROLLER_DEPTH0_STATELESS_FORAGING_CONTROLLER_HPP_
#define INCLUDE_FORDYCA_CONTROLLER_DEPTH0_STATELESS_FORAGING_CONTROLLER_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/patterns/visitor/visitable.hpp"
#include "fordyca/controller/base_foraging_controller.hpp"
#include "fordyca/metrics/fsm/distance_metrics.hpp"
#include "fordyca/metrics/fsm/block_acquisition_metrics.hpp"
#include "fordyca/metrics/fsm/block_transport_metrics.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca);

namespace visitor = rcppsw::patterns::visitor;
namespace fsm { namespace depth0 { class stateless_foraging_fsm; } }

NS_START(controller, depth0);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class stateless_foraging_controller
 * @ingroup controller depth0
 *
 * @brief The most basic form of a foraging controller: roam around randomly
 * until you find a block, and then bring it back to the nest; repeat.
 */
class stateless_foraging_controller : public base_foraging_controller,
                                      public metrics::fsm::distance_metrics,
                                      public metrics::fsm::block_acquisition_metrics,
                                      public metrics::fsm::block_transport_metrics,
                                      public visitor::visitable_any<stateless_foraging_controller> {
 public:
  stateless_foraging_controller(void);
  ~stateless_foraging_controller(void) override;

  /* CCI_Controller overrides */
  void Init(ticpp::Element& node) override;
  void ControlStep(void) override;
  void Reset(void) override;

  /* distance metrics */
  int entity_id(void) const override;
  double timestep_distance(void) const override;

  /* base FSM metrics */
  bool is_avoiding_collision(void) const override;

  /* block acquisition metrics */
  bool is_exploring_for_block(void) const override;
  bool is_acquiring_block(void) const override { return false; }
  bool is_vectoring_to_block(void) const override { return false; }

  /* block transport metrics */
  bool is_transporting_to_nest(void) const override;
  bool is_transporting_to_cache(void) const override { return false; }

  bool block_acquired(void) const;
  fsm::depth0::stateless_foraging_fsm* fsm(void) const { return m_fsm.get(); }

 private:
  std::unique_ptr<fsm::depth0::stateless_foraging_fsm> m_fsm;
};

NS_END(depth0, controller, fordyca);

#endif /* INCLUDE_FORDYCA_CONTROLLER_DEPTH0_STATELESS_FORAGING_CONTROLLER_HPP_ */
