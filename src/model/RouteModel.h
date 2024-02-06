#ifndef INC_3D_VIEWER_TEMPLATE_ROUTEMODEL_H
#define INC_3D_VIEWER_TEMPLATE_ROUTEMODEL_H

#include <climits>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

namespace s21 {

class RouteModel {
 public:
  RouteModel(int xs, int ys, int xe, int ye, const std::string &filename);

  std::vector<std::pair<int, int>> find_route();

 private:
  int xs_, ys_, xe_, ye_;
  int rows_, cols_;
  std::vector<std::vector<int>> horizontal_walls_;
  std::vector<std::vector<int>> vertical_walls_;
  std::vector<std::vector<int>> maze_;

  /* Helpers */
  void route_bfs();
  std::vector<std::pair<int, int>> backtrack_route() const noexcept;
  bool cell_access(int x_from, int y_from, int x_to, int y_to) const noexcept;
};

}  // namespace s21

#endif  // INC_3D_VIEWER_TEMPLATE_ROUTEMODEL_H