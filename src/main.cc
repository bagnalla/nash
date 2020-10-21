#include <iostream>
#include <string>
#include "solver.h"
#include "game.h"

using namespace std;
using namespace nash;
using namespace solver;

#define ROCK 0
#define PAPER 1
#define SCISSORS 2

#define NUM_ROUNDS 1000000

// The following are a few example utility functions for RPS. The
// first is the standard one and results in uniform solution
// strategies. The others are weird in some way and result in non
// uniform strategies.

// Compute the utility for p1 given both players' actions.
double rps_utility(int p1_action, int p2_action)
{
  switch (p1_action) {
  case ROCK:
    switch (p2_action) {
    case ROCK:
      return 0.0;
    case PAPER:
      return -1.0;
    case SCISSORS:
      return 1.0;
    };
  case PAPER:
    switch (p2_action) {
    case ROCK:
      return 1.0;
    case PAPER:
      return 0.0;
    case SCISSORS:
      return -1.0;
    };
  case SCISSORS:
    switch (p2_action) {
    case ROCK:
      return -1.0;
    case PAPER:
      return 1.0;
    case SCISSORS:
      return 0.0;
    };
  };
  cerr << "ERROR: rps_utility. p1_action: " << p1_action <<
    ",  p2_action: " << p2_action << endl;
  return 0.0;
}

// Compute the utility for p1 given both players' actions.
double rps_utility_2(int p1_action, int p2_action)
{
  switch (p1_action) {
  case ROCK:
    switch (p2_action) {
    case ROCK:
      return -0.5;
    case PAPER:
      return -0.5;
    case SCISSORS:
      return 1.0;
    };
  case PAPER:
    switch (p2_action) {
    case ROCK:
      return 0.5;
    case PAPER:
      return 0.0;
    case SCISSORS:
      return -1.0;
    };
  case SCISSORS:
    switch (p2_action) {
    case ROCK:
      return -1.0;
    case PAPER:
      return 1.0;
    case SCISSORS:
      return 0.0;
    };
  };
  cerr << "ERROR: rps_utility_2. p1_action: " << p1_action <<
    ",  p2_action: " << p2_action << endl;
  return 0.0;
}

// Compute the utility for p1 given both players' actions.
double rps_utility_3(int p1_action, int p2_action)
{
  switch (p1_action) {
  case ROCK:
    switch (p2_action) {
    case ROCK:
      return 0.0;
    case PAPER:
      return -0.5;
    case SCISSORS:
      return 0.5;
    };
  case PAPER:
    switch (p2_action) {
    case ROCK:
      return 0.5;
    case PAPER:
      return 0.0;
    case SCISSORS:
      return -1.5;
    };
  case SCISSORS:
    switch (p2_action) {
    case ROCK:
      return -0.5;
    case PAPER:
      return 1.5;
    case SCISSORS:
      return 0.0;
    };
  };
  cerr << "ERROR: rps_utility_3. p1_action: " << p1_action <<
    ",  p2_action: " << p2_action << endl;
  return 0.0;
}

// TODO: this should result in an equilibrium with 0 expected utility
// for both players.. but it doesn't.
double rps_utility_4(int p1_action, int p2_action)
{
  switch (p1_action) {
  case ROCK:
    switch (p2_action) {
    case ROCK:
      return 0.0;
    case PAPER:
      return -0.1;
    case SCISSORS:
      return 1.2;
    };
  case PAPER:
    switch (p2_action) {
    case ROCK:
      return 0.1;
    case PAPER:
      return 0.0;
    case SCISSORS:
      return -1.0;
    };
  case SCISSORS:
    switch (p2_action) {
    case ROCK:
      return -1.2;
    case PAPER:
      return 1.0;
    case SCISSORS:
      return 0.0;
    };
  };
  cerr << "ERROR: rps_utility_4. p1_action: " << p1_action <<
    ",  p2_action: " << p2_action << endl;
  return 0.0;
}

void print_rps_solution(solution_t solution)
{
  static string action_names[] = { "ROCK", "PAPER", "SCISSORS" };
  cout << "solution:\n";
  auto p1_strat = solution.first;
  auto p2_strat = solution.second;
  cout << "player 1: \n";
  for (size_t i = 0; i < p1_strat.size(); ++i) {
    cout << action_names[i] << ": " << p1_strat[i];
    if (i < p1_strat.size() - 1) {
      cout << ", ";
    }
  }
  cout << "\nplayer 2: \n";
  for (size_t i = 0; i < p2_strat.size(); ++i) {
    cout << action_names[i] << ": " << p2_strat[i];
    if (i < p2_strat.size() - 1) {
      cout << ", ";
    }
  }
  cout << endl;
}

int main()
{
  auto utility_function = rps_utility_4;
  Game rps(3, 3, utility_function);
  Solver solver(rps);
  solution_t solution = solver.solve(NUM_ROUNDS);
  print_rps_solution(solution);
  auto utilities = solver.compute_expected_utilities(NUM_ROUNDS);

  cout << "p1 expected utility: " << utilities.first << endl;
  cout << "p2 expected utility: " << utilities.second << endl;

  return 0;
}
