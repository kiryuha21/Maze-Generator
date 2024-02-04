#ifndef INC_3D_VIEWER_TEMPLATE_GENERATIONMODEL_H
#define INC_3D_VIEWER_TEMPLATE_GENERATIONMODEL_H

#include <chrono>
#include <fstream>
#include <random>
#include <stdexcept>
#include <vector>

namespace s21 {
class GenerationModel {
 public:
  GenerationModel(int rows, int cols);
  void generate_maze();
  void save_maze_to_file(const std::string &filepath) const;

 private:
  int rows_, cols_;
  int set_number_ = 1;

  std::vector<std::vector<int>> horizontal_walls_;
  std::vector<std::vector<int>> vertical_walls_;
  std::vector<std::vector<int>> maze_;

  static constexpr int kEmptyCell = 0;
  static constexpr int kWallCell = 1;
  static constexpr unsigned long kWallSpawningChance = 30;

  /* Helpers */
  void validate_walls();
  static bool random_chance(unsigned long chance) noexcept;
  void join_sets(int row);
  void build_vertical_walls(int row);
  void build_horizontal_walls(int row);
  void prepare_next_row(int row);
};
}  // namespace s21

#endif  // INC_3D_VIEWER_TEMPLATE_GENERATIONMODEL_H
