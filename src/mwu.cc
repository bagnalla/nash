#include <algorithm>
#include <iostream>
#include <random>
#include "mwu.h"

using namespace std;

namespace nash
{
  namespace mwu
  {
    Agent::Agent() {}

    // Agent constructor
    Agent::Agent(size_t action_space_size)
    {
      this->gen = ranlux48_base(random_device()());
      this->dist = uniform_real_distribution<>(0.0, 1.0);
      this->action_space_size = action_space_size;
      this->cumulative_regrets.resize(action_space_size);
      this->pmf.resize(action_space_size);
      this->cdf.resize(action_space_size);
      this->init();
    }

    void Agent::init()
    {
      // Initialize all regrets to 0.
      for (size_t i = 0; i < this->action_space_size; ++i) {
        this->cumulative_regrets[i] = 0.0;
      }
      // Initialize PMF and CDF.
      this->update_cumulative_regrets(vector<double>(this->action_space_size, 0.0));
    }

    size_t Agent::get_action_space_size() const
    {
      return this->action_space_size;
    }

    vector<double> Agent::get_pmf() const
    {
      return this->pmf;
    }

    // This function can't be const because it modifies the agent's
    // internal PRNG state.
    int Agent::sample()
    {
      // Binary search the CDF (inverse transform sampling).
      
      // Generate a double in [0.0, 1.0].
      double x = this->dist(this->gen);
      // cout << "x: " << x << endl;

      size_t l = 0;
      size_t r = action_space_size;

      while (r - l > 1) {
	size_t i = (l + r) / 2;
	// cout << l << " " << r << " " << i << endl;
	if (x < cdf[i]) r = i;
	else l = i;
      }

      if (l == r) {
	cerr << "Agent::sample warning: l == r" << endl;
      }

      // cout << "x: " << x << ", l: " << l << ", cdf[l]: " << cdf[l] <<
      // 	", r: " << r << ", cdf[r]: " << cdf[r] << endl;

      return x < cdf[l] ? l : r;
    }

    vector<int> Agent::sample(size_t num_samples)
    {
      vector<int> samples(num_samples);
      for (size_t i = 0; i < num_samples; ++i) {
	samples[i] = this->sample();
      }
      return samples;
    }

    void Agent::update_cumulative_regrets(const vector<double> &regrets)
    {
      double regret_sum = 0.0;

      // Update regrets.
      for (size_t i = 0; i < this->action_space_size; ++i) {
	this->cumulative_regrets[i] += regrets[i];
	// cout << regrets[i] << endl;
	regret_sum += max(0.0, this->cumulative_regrets[i]);
      }

      // cout << "action_space_size: " << this->action_space_size << endl;
      // cout << "regret_sum: " << regret_sum << endl;
      // regret_sum = regret_sum > 0.0 ? regret_sum : this->action_space_size;
      // cout << "regret_sum: " << regret_sum << endl;

      double cdf_acc = 0.0;

      // Recompute PMF and CDF.
      if (regret_sum > 0.0) {
	for (size_t i = 0; i < this->action_space_size; ++i) {
	  this->pmf[i] = max(0.0, this->cumulative_regrets[i]) / regret_sum;
	  cdf_acc = cdf_acc + this->pmf[i];
	  this->cdf[i] = cdf_acc;
	}
      }
      else {
	for (size_t i = 0; i < this->action_space_size; ++i) {
	  this->pmf[i] = 1.0 / this->action_space_size;
	  cdf_acc = cdf_acc + this->pmf[i];
	  this->cdf[i] = cdf_acc;
	}
      }
    }
  }
}
