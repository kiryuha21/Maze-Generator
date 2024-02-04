#include "GenerationController.h"

namespace s21 {
GenerationController::~GenerationController() { delete model_; }

void GenerationController::regenerate_map(int rows, int cols) {
  delete model_;
  model_ = new GenerationModel(rows, cols);
  model_->generate_maze();
}

void GenerationController::save_map(const std::string& filename) const {
  model_->save_maze_to_file(filename);
}
}  // namespace s21