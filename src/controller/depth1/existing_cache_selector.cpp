/**
 * @file existing_cache_selector.cpp
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
#include "fordyca/controller/depth1/existing_cache_selector.hpp"
#include "fordyca/math/existing_cache_utility.hpp"
#include "fordyca/representation/base_cache.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(fordyca, controller, depth1);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
existing_cache_selector::existing_cache_selector(
    const std::shared_ptr<rcppsw::er::server>& server,
    argos::CVector2 nest_loc)
    : client(server), m_nest_loc(nest_loc) {
  insmod("existing_cache_selector",
         rcppsw::er::er_lvl::DIAG,
         rcppsw::er::er_lvl::NOM);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
representation::perceived_cache existing_cache_selector::calc_best(
    const std::list<representation::perceived_cache>& existing_caches,
    argos::CVector2 robot_loc) {
  representation::perceived_cache best;
  ER_ASSERT(!existing_caches.empty(), "FATAL: no known existing caches");

  double max_utility = 0.0;
  for (auto& c : existing_caches) {
    math::existing_cache_utility u(c.ent->real_loc(), m_nest_loc);

    double utility =
        u.calc(robot_loc, c.density.last_result(), c.ent->n_blocks());
    ER_ASSERT(utility > 0.0, "FATAL: Bad utility calculation");
    ER_DIAG("Utility for existing_cache%d loc=(%zu, %zu), density=%f: %f",
            c.ent->id(),
            c.ent->discrete_loc().first,
            c.ent->discrete_loc().second,
            c.density.last_result(),
            utility);

    if (utility > max_utility) {
      best = c;
      max_utility = utility;
    }
  } /* for(existing_cache..) */

  ER_ASSERT(nullptr != best.ent, "FATAL: No best perceived cache found?");

  ER_NOM("Best utility: existing_cache%d at (%zu, %zu): %f",
         best.ent->id(),
         best.ent->discrete_loc().first,
         best.ent->discrete_loc().second,
         max_utility);
  return best;
} /* calc_best() */

NS_END(depth1, controller, fordyca);
