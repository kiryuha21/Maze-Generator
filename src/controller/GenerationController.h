#ifndef INC_3D_VIEWER_TEMPLATE_GENERATIONCONTROLLER_H
#define INC_3D_VIEWER_TEMPLATE_GENERATIONCONTROLLER_H

#include "../model/GenerationModel.h"
#include "../model/RouteModel.h"

namespace s21 {
class GenerationController {
 public:
  GenerationController() = default;
  ~GenerationController();

  void regenerate_map(int rows, int cols);
  void save_map(const std::string& filename) const;
  std::vector<std::pair<int, int>> find_route(
      int xs, int ys, int xe, int ye, const std::string& filename) const;

 private:
  GenerationModel* model_ = nullptr;
};
}  // namespace s21

#endif  // INC_3D_VIEWER_TEMPLATE_GENERATIONCONTROLLER_H
