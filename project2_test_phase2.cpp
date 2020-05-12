#include "WAVLTree.h"
#include "project2.h"
#include "helpers.h"
#include <iostream>
#include <utility>
#include <string>
#include <cmath>
#include <fstream>
#include <map>
#include "project2_test_phase2.h"

typedef void (*algorithm)(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space);
typedef std::map<std::string, algorithm> alg_map;

struct waste
{
  int n; // size
  double w;
};

typedef struct ProblemInstance {
  std::vector<double> items;
  std::vector<int> assignments;
  std::vector<double> free_space;
} ProblemInstance;

typedef void (*algorithm) (const std::vector<double>&, std::vector<int>&, std::vector<double>&);

std::vector<double> generateTest(int sz)
{
  std::vector<double> result;
  for (int i = 0; i < sz; i++)
  {
    result.push_back(((double)rand() / RAND_MAX) * 0.70); // 0-0.7
  }
  return result;
}

waste get_waste(std::vector<double>& test, void (*algorithm)(std::vector<int>&), int rep)
{

}

alg_map init_algs()
{
  alg_map bin_algs;
  bin_algs.insert(std::pair<std::string, algorithm>("next_fit", &next_fit));
  bin_algs.insert(std::pair<std::string, algorithm>("first_fit", &first_fit));
  bin_algs.insert(std::pair<std::string, algorithm>("best_fit", &best_fit));
  bin_algs.insert(std::pair<std::string, algorithm>("first_fit_decreasing", &first_fit_decreasing));
  bin_algs.insert(std::pair<std::string, algorithm>("best_fit_decreasing", &best_fit_decreasing));
  return bin_algs;
}

void create_empty_W_file(std::string filename)
{
  std::ofstream f;
  f.open(filename, std::ios::trunc);
  f << "Algorithm_Name, W(Algorithm)\n";
  f.close();
}

void add_W_to_file(std::string funcname, waste w, std::string filename)
{

}


int main()
{
  std::vector<std::vector<double>> tests;
  tests.push_back(generateTest(10));
  tests.push_back(generateTest(50));
  tests.push_back(generateTest(100));
  tests.push_back(generateTest(500));
  tests.push_back(generateTest(1000));
  tests.push_back(generateTest(10000));

  std::string funcname = "next_fit";
  std::string filename = funcname + ".csv";
  std::cout << filename << std::endl;

  create_empty_W_file(filename);

  int i = 0;
  for (auto test : tests)
  {
    std::cout << "Test " << i << std::endl;

    i++;
  }

  return 0;
}

