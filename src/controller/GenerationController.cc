#include "GenerationController.h"

namespace s21 {
GenerationController::GenerationController() {
  // TODO : move out from constructor and remove 1, 1
  model_ = new GenerationModel(1, 1);
}

GenerationController::~GenerationController() { delete model_; }
}  // namespace s21