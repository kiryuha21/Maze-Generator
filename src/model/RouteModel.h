#ifndef INC_3D_VIEWER_TEMPLATE_ROUTEMODEL_H
#define INC_3D_VIEWER_TEMPLATE_ROUTEMODEL_H

#include <string>
#include <vector>
#include <fstream>

namespace s21 {

  class RouteModel {
  public:
    RouteModel(int xs, int ys, int xe, int ye, const std::string &filename);

    std::vector<std::pair<int, int>> find_route() const;

  private:
    int xs_, ys_, xe_, ye_;
    int rows_, cols_;
    std::vector<std::vector<int>> horizontal_walls_;
    std::vector<std::vector<int>> vertical_walls_;

    bool recursive_search(int x, int y, int lx, int ly, std::vector<std::pair<int, int>> &res) const;
  };

}  // namespace s21

#endif  // INC_3D_VIEWER_TEMPLATE_ROUTEMODEL_H