/**
 * @file mwu.h
 * @author bagnalla
 * @brief MWU learner agent.
 *
 * xxx
 */

#ifndef MWU_H
#define MWU_H

#include <random>
#include <vector>

namespace nash
{
  namespace mwu
  {
    void mwu_test();

    class Agent
    {
    public:
      Agent();
      Agent(size_t action_space_size);
      std::vector<double> get_pmf() const;
      size_t get_action_space_size() const;
      int sample();
      std::vector<int> sample(size_t num_samples);
      void update_cumulative_regrets(const std::vector<double> &regrets);
    private:
      std::uniform_real_distribution<> dist;
      std::ranlux48_base gen; // supposedly faster than mt19937
      size_t action_space_size;
      std::vector<double> cumulative_regrets;
      std::vector<double> pmf;
      std::vector<double> cdf;
      void init();
    };
  }
}

#endif
