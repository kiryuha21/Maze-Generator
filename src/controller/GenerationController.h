#ifndef INC_3D_VIEWER_TEMPLATE_GENERATIONCONTROLLER_H
#define INC_3D_VIEWER_TEMPLATE_GENERATIONCONTROLLER_H

#include "../model/GenerationModel.h"

namespace s21 {
class GenerationController {
 public:
  GenerationController();
  ~GenerationController();

 private:
  GenerationModel* model_ = nullptr;
};
}  // namespace s21

#endif  // INC_3D_VIEWER_TEMPLATE_GENERATIONCONTROLLER_H
