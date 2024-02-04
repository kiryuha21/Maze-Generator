#include "GenerationModel.h"

namespace s21 {
GenerationModel::GenerationModel(int rows, int cols)
    : rows_(rows),
      cols_(cols),
      horizontal_walls_(std::vector<std::vector<int>>(
          rows, std::vector<int>(cols, kEmptyCell))),
      vertical_walls_(std::vector<std::vector<int>>(
          rows, std::vector<int>(cols, kEmptyCell))),
      maze_(std::vector<std::vector<int>>(rows,
                                          std::vector<int>(cols, kEmptyCell))) {
  if (std::min(rows, cols) <= 0 || std::max(rows, cols) > 50) {
    throw std::out_of_range("Rows, cols must be in [1; 50]");
  }
  validate_walls();
};

void GenerationModel::validate_walls() {
  for (int i = 0; i < rows_; ++i) {
    vertical_walls_[i][cols_ - 1] = kWallCell;
  }
  for (int i = 0; i < cols_; ++i) {
    horizontal_walls_[rows_ - 1][i] = kWallCell;
  }
}

bool GenerationModel::random_chance(unsigned long chance) noexcept {
  static std::random_device rd;
  static std::mt19937 rng_{rd()};
  return rng_() % 100 < chance;
}

void GenerationModel::join_sets(int row) {
  for (auto &cell : maze_[row]) {
    if (cell == kEmptyCell) {
      cell = set_number_++;
    }
  }
}

void GenerationModel::build_vertical_walls(int row) {
  for (int i = 0; i < cols_ - 1; ++i) {
    if (random_chance(kWallSpawningChance) ||
        maze_[row][i] == maze_[row][i + 1]) {
      vertical_walls_[row][i] = kWallCell;
    } else {
      int set_to_delete = maze_[row][i + 1];
      for (int j = i + 1; j < cols_ && maze_[row][j] == set_to_delete; ++j) {
        maze_[row][j] = maze_[row][i + 1];
      }
    }
  }
}

void GenerationModel::build_horizontal_walls(int row) {
  for (int i = 0; i < cols_; ++i) {
    if (random_chance(kWallSpawningChance)) {
      horizontal_walls_[row][i] = kWallCell;
    }
  }

  for (int i = 0; i < cols_; ++i) {
    bool no_exit = true;
    for (int j = 0; j < cols_ && no_exit; ++j) {
      if (maze_[row][i] == maze_[row][j] &&
          horizontal_walls_[row][j] == kEmptyCell) {
        no_exit = false;
      }
    }

    if (no_exit) {
      horizontal_walls_[row][i] = kEmptyCell;
    }
  }
}

void GenerationModel::prepare_next_row(int row) {
  for (int i = 0; i < cols_; ++i) {
    if (horizontal_walls_[row][i] == kEmptyCell) {
      maze_[row + 1][i] = maze_[row][i];
    }
  }
}

void GenerationModel::generate_maze() {
  for (int i = 0; i < rows_; ++i) {
    join_sets(i);
    build_vertical_walls(i);
    build_horizontal_walls(i);
    if (i != rows_ - 1) {
      prepare_next_row(i);
    }
  }

  validate_walls();
}

void GenerationModel::save_maze_to_file(const std::string &filepath) const {
  std::fstream fs(filepath, std::ios::out);
  fs << rows_ << ' ' << cols_;
  for (auto &vec : vertical_walls_) {
    for (auto &cell : vec) {
      fs << cell << ' ';
    }
    fs << '\n';
  }

  fs << '\n';

  for (auto &vec : horizontal_walls_) {
    for (auto &cell : vec) {
      fs << cell << ' ';
    }
    fs << '\n';
  }
}

}  // namespace s21
