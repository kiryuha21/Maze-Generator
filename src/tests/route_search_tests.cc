#include <gtest/gtest.h>

#include <vector>

#include "../model/RouteModel.h"

namespace s21 {
const std::string kCasesPath = "tests/routing cases/";

TEST(route_search_tests, single_cell_test) {
  RouteModel router(1, 1, 1, 1, kCasesPath + "one_cell.mp");

  auto expected = std::vector<std::pair<int, int>>{{1, 1}};
  ASSERT_EQ(router.find_route(), expected);
}

TEST(route_search_tests, no_way_test) {
  RouteModel router(1, 1, 2, 2, kCasesPath + "no_way.mp");

  auto expected = std::vector<std::pair<int, int>>{};
  ASSERT_EQ(router.find_route(), expected);
}

TEST(route_search_tests, single_way_test) {
  RouteModel router(1, 1, 4, 4, kCasesPath + "single_way.mp");

  auto expected = std::vector<std::pair<int, int>>{
      {4, 4}, {3, 4}, {3, 3}, {4, 3}, {4, 2}, {4, 1}, {3, 1}, {2, 1}, {1, 1}};
  ASSERT_EQ(router.find_route(), expected);
}

TEST(route_search_tests, multiple_way_test) {
  RouteModel router(1, 1, 1, 3, kCasesPath + "multiple_ways.mp");

  auto expected = std::vector<std::pair<int, int>>{{1, 3}, {1, 2}, {1, 1}};
  ASSERT_EQ(router.find_route(), expected);
}
}  // namespace s21
