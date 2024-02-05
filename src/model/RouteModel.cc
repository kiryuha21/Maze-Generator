#include "RouteModel.h"

namespace s21 {
RouteModel::RouteModel(int xs, int ys, int xe, int ye,
                       const std::string &filename)
    : xs_(xs), ys_(ys), xe_(xe), ye_(ye) {
  std::fstream fs(filename, std::ios::in);
  fs.exceptions(std::fstream::badbit);

  try {
    fs >> rows_ >> cols_;
  } catch (const std::fstream::failure &e) {
    throw std::out_of_range("File format error");
  }

  vertical_walls_ =
      std::vector<std::vector<int>>(rows_ + 1, std::vector<int>(cols_ + 1, 1));
  horizontal_walls_ =
      std::vector<std::vector<int>>(rows_ + 1, std::vector<int>(cols_ + 1, 1));

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      fs >> vertical_walls_[i + 1][j + 1];
    }
  }

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      fs >> horizontal_walls_[i + 1][j + 1];
    }
  }
}

std::vector<std::pair<int, int>> RouteModel::find_route() const {
  std::vector<std::pair<int, int>> res;
  if (!recursive_search(xs_, ys_, -1, -1, res)) {
    throw std::out_of_range("No way found :c");
  }
  return res;
}

bool RouteModel::recursive_search(int x, int y, int lx, int ly,
                                  std::vector<std::pair<int, int>> &res) const {
  if (x == xe_ && y == ye_) {
    res.emplace_back(x, y);
    return true;
  }

  for (auto &direction :
       std::vector<std::pair<int, int>>{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}) {
    if (x + direction.first == lx && y + direction.second == ly) {
      continue;
    }

    if ((direction.first == 1 && horizontal_walls_[x][y]) ||
        (direction.first == -1 && horizontal_walls_[x - 1][y])) {
      continue;
    }

    if ((direction.second == 1 && vertical_walls_[x][y]) ||
        (direction.second == -1 && vertical_walls_[x][y - 1])) {
      continue;
    }

    if (recursive_search(x + direction.first, y + direction.second, x, y,
                         res)) {
      res.emplace_back(x, y);
      return true;
    }
  }

  return false;
}

}  // namespace s21
