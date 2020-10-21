#include <iostream>
#include <vector>
#include "solver.h"

using namespace std;

namespace nash
{
  namespace solver
  {
    Solver::Solver(const Game &game)
    {
      this->utility_function = game.utility_function;
      this->p1_agent = mwu::Agent(game.p1_action_space_size);
      this->p2_agent = mwu::Agent(game.p2_action_space_size);
      this->p1_pmf_sum = this->p1_agent.get_pmf();
      this->p2_pmf_sum = this->p2_agent.get_pmf();
    }

    Solver::~Solver() {}

    solution_t Solver::solve(size_t num_rounds)
    {
      // cout << "running " << num_rounds << " iterations..." << endl;
      // Run for num_rounds iterations.
      for (size_t i = 0; i < num_rounds; ++i) {
        this->round();
      }

      // The solution is the time-averaged history of strategy profiles.
      return this->compute_time_averaged_pmfs(num_rounds);
    }

    pair<vector<double>, vector<double>>
    Solver::compute_time_averaged_pmfs(size_t num_rounds) const
    {
      vector<double> p1_pmf, p2_pmf;
      for (size_t i = 0; i < this->p1_pmf_sum.size(); ++i) {
	p1_pmf.push_back(this->p1_pmf_sum[i] / (num_rounds + 1));
      }
      for (size_t i = 0; i < this->p2_pmf_sum.size(); ++i) {
	p2_pmf.push_back(this->p2_pmf_sum[i] / (num_rounds + 1));
      }
      return make_pair(p1_pmf, p2_pmf);
    }

    // TODO: Also try a variant of Solve that evaluates batches of
    // samples and incorporates their cost vectors into a single
    // update. Compare convergence behavior with the standard
    // algorithm.

    // Sample actions, evaluate costs, update weights.
    void Solver::round()
    {
      int p1_action = this->p1_agent.sample();
      int p2_action = this->p2_agent.sample();
      pair<vector<double>, vector<double>> regrets =
	this->compute_regrets(p1_action, p2_action);

      this->p1_agent.update_cumulative_regrets(regrets.first);
      this->p2_agent.update_cumulative_regrets(regrets.second);

      auto p1_pmf = this->p1_agent.get_pmf();
      auto p2_pmf = this->p2_agent.get_pmf();
      
      for (size_t i = 0; i < p1_pmf.size(); ++i) {
	this->p1_pmf_sum[i] += p1_pmf[i];
      }
      for (size_t i = 0; i < p2_pmf.size(); ++i) {
	this->p2_pmf_sum[i] += p2_pmf[i];
      }
    }

    pair<vector<double>, vector<double>>
    Solver::compute_regrets(int p1_action, int p2_action) const
    {
      double p1_utility = this->utility_function(p1_action, p2_action);
      double p2_utility = -p1_utility;

      vector<double> p1_regrets, p2_regrets;
      for (size_t i = 0; i < this->p1_agent.get_action_space_size(); ++i) {
	p1_regrets.push_back(this->utility_function(i, p2_action) - p1_utility);
      }
      for (size_t i = 0; i < this->p2_agent.get_action_space_size(); ++i) {
	p2_regrets.push_back(p2_utility - this->utility_function(p1_action, i));
      }
      
      return make_pair(p1_regrets, p2_regrets);
    }

    pair<double, double>
    Solver::compute_expected_utilities(size_t num_rounds) const
    {
      auto pmfs = this->compute_time_averaged_pmfs(num_rounds);
      auto p1_pmf = pmfs.first, p2_pmf = pmfs.second;

      double p1_expected_utility = 0.0, p2_expected_utility = 0.0;
      for (size_t i = 0; i < this->p1_agent.get_action_space_size(); ++i) {
	for (size_t j = 0; j < this->p2_agent.get_action_space_size(); ++j) {
	  double x = p1_pmf[i] * p2_pmf[j] * this->utility_function(i, j);
	  p1_expected_utility += x;
	  p2_expected_utility -= x;
	}
      }

      return make_pair(p1_expected_utility, p2_expected_utility);
    }
  }
}
