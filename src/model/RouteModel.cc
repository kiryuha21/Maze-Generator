#include "RouteModel.h"

#include <iostream>

namespace s21 {
RouteModel::RouteModel(int xs, int ys, int xe, int ye,
                       const std::string &filename)
    : xs_(xs), ys_(ys), xe_(xe), ye_(ye) {
  std::fstream fs(filename, std::ios::in);
  fs.exceptions(std::fstream::badbit);

  try {
    fs >> rows_ >> cols_;
  } catch (const std::fstream::failure &e) {
    throw std::logic_error("File format error");
  }

  if (xs_ < 1 || xs_ > cols_ || xe_ < 1 || xe_ > cols_ || ys_ < 1 ||
      ys_ > rows_ || ye_ < 1 || ye_ > rows_) {
    throw std::logic_error("Invalid route cells!");
  }

  vertical_walls_ =
      std::vector<std::vector<int>>(rows_ + 1, std::vector<int>(cols_ + 1, 1));
  horizontal_walls_ =
      std::vector<std::vector<int>>(rows_ + 1, std::vector<int>(cols_ + 1, 1));
  maze_ = std::vector<std::vector<int>>(rows_ + 1,
                                        std::vector<int>(cols_ + 1, INT_MAX));

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

std::vector<std::pair<int, int>> RouteModel::find_route() {
  route_bfs();
  return backtrack_route();
}

void RouteModel::route_bfs() {
  std::queue<std::pair<int, int>> bfs;
  bfs.emplace(xs_, ys_);
  int n = 0;
  while (!bfs.empty()) {
    size_t size = bfs.size();
    for (size_t i = 0; i < size; ++i) {
      auto cell = bfs.front();
      bfs.pop();
      maze_[cell.second][cell.first] = n;
      for (auto &direction : directions) {
        if (cell_access(cell.first, cell.second, direction.first,
                        direction.second) &&
            maze_[cell.second + direction.second]
                 [cell.first + direction.first] == INT_MAX) {
          bfs.emplace(cell.first + direction.first,
                      cell.second + direction.second);
        }
      }
    }
    if (maze_[ye_][xe_] != INT_MAX) {
      return;
    }
    ++n;
  }
  throw std::logic_error("No way between cells");
}

bool RouteModel::cell_access(int x, int y, int delta_x,
                             int delta_y) const noexcept {
  if (!(delta_x == 0 && std::abs(delta_y) == 1) &&
      !(delta_y == 0 && std::abs(delta_x) == 1)) {
    return false;
  }

  if (delta_x == 1 && vertical_walls_[y][x]) {
    return false;
  }
  if (delta_x == -1 && vertical_walls_[y][x - 1]) {
    return false;
  }
  if (delta_y == 1 && horizontal_walls_[y][x]) {
    return false;
  }
  if (delta_y == -1 && horizontal_walls_[y - 1][x]) {
    return false;
  }

  return true;
}

std::vector<std::pair<int, int>> RouteModel::backtrack_route() const noexcept {
  int n = maze_[ye_][xe_];
  int x = xe_, y = ye_;
  std::vector<std::pair<int, int>> res;
  while (n--) {
    for (auto &direction : directions) {
      if (cell_access(x, y, direction.first, direction.second) &&
          maze_[y + direction.second][x + direction.first] == n) {
        res.emplace_back(x, y);
        x += direction.first;
        y += direction.second;
      }
    }
  }
  res.emplace_back(x, y);
  return res;
}

}  // namespace s21
