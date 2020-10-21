/**
 * @file solver.h
 * @author bagnalla
 * @brief Mixed Nash equilibrium solver.
 *
 * Compute approximate mixed nash equilibria by running MWU
 * agents. Specialized to two-player games. Generalization to n-player
 * games is straightforward but unnecessary.
 */

#ifndef SOLVER_H
#define SOLVER_H

#include <functional>
#include "game.h"
#include "mwu.h"

namespace nash
{
  typedef std::pair<std::vector<double>, std::vector<double>> solution_t;

  // We could just allocate agents locally within Solve, but for now
  // we keep them as mutable state of the solver because we don't
  // really intend to run the same solver from scratch multiple
  // times. This setup might make it easier to run for some iterations
  // and then resume later from where we left off.

  namespace solver
  {
    class Solver
    {
    public:
      Solver(const Game &game);
      ~Solver();
      solution_t solve(size_t num_rounds);
      std::pair<double, double>
      compute_expected_utilities(size_t num_rounds) const;
    private:
      mwu::Agent p1_agent, p2_agent;
      std::vector<double> p1_pmf_sum, p2_pmf_sum;
      utility_fun_t utility_function;
      void round();
      std::pair<std::vector<double>, std::vector<double>>
      compute_regrets(int p1_action, int p2_action) const;
      std::pair<std::vector<double>, std::vector<double>>
      compute_time_averaged_pmfs(size_t num_rounds) const;
    };
  }
}

#endif
