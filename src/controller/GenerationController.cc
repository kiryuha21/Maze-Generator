#include "GenerationController.h"

namespace s21 {
GenerationController::GenerationController() { model_ = new GenerationModel; }

GenerationController::~GenerationController() { delete model_; }
}  // namespace s21