#include <gtest/gtest.h>
#include <sys/stat.h>

#include <fstream>

#include "../model/GenerationModel.h"

namespace s21 {
TEST(generation_tests, file_generation_test) {
  GenerationModel model(5, 5);
  model.generate_maze();
  model.save_maze_to_file("test.mp");

  struct stat buff {};
  ASSERT_EQ(stat("test.mp", &buff), 0);
}

TEST(generation_tests, correct_file_test) {
  GenerationModel model(5, 5);
  model.generate_maze();
  model.save_maze_to_file("test.mp");

  std::fstream fs("test.mp", std::ios::in);
  fs.exceptions(std::fstream::badbit);

  int rows, cols;
  ASSERT_NO_THROW(fs >> rows >> cols);
  ASSERT_EQ(rows, 5);
  ASSERT_EQ(cols, 5);
  for (int k = 0; k < 2; ++k) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        int x;
        ASSERT_NO_THROW(fs >> x);
        ASSERT_TRUE(x == 1 || x == 0);
      }
    }
  }
}
}  // namespace s21