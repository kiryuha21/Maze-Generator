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
                                          std::vector<int>(cols, kEmptyCell))),
      line_(std::vector<int>(cols, kEmptyCell)) {
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

void GenerationModel::join_sets() {
  for (auto &cell : line_) {
    if (cell == kEmptyCell) {
      cell = set_number_++;
    }
  }
}

void GenerationModel::build_vertical_walls(int row) {
  for (int i = 0; i < cols_ - 1; ++i) {
    if (random_chance(kWallSpawningChance) || line_[i] == line_[i + 1]) {
      vertical_walls_[row][i] = kWallCell;
    } else {
      int set_to_delete = line_[i + 1];
      for (int j = 0; j < cols_; ++j) {
        if (line_[j] == set_to_delete) {
          line_[j] = line_[i];
        }
      }
    }
  }
  validate_walls();
}

int GenerationModel::count(int elem) const {
  int res = 0;
  for (auto &i : line_) {
    if (i == elem) {
      ++res;
    }
  }
  return res;
}

int GenerationModel::count_horizontal_walls(int elem, int row) const {
  int res = 0;
  for (int i = 0; i < cols_; ++i) {
    if (line_[i] == elem && horizontal_walls_[row][i] == kEmptyCell) {
      ++res;
    }
  }
  return res;
}

void GenerationModel::build_horizontal_walls(int row) {
  for (int i = 0; i < cols_; ++i) {
    if (random_chance(kWallSpawningChance) && count(line_[i]) != 1) {
      horizontal_walls_[row][i] = kWallCell;
    }
  }

  for (int i = 0; i < cols_; ++i) {
    if (count_horizontal_walls(line_[i], row) == 0) {
      horizontal_walls_[row][i] = kEmptyCell;
    }
  }
}

void GenerationModel::prepare_next_row(int row) {
  for (int i = 0; i < cols_; ++i) {
    if (horizontal_walls_[row][i] == kWallCell) {
      line_[i] = kEmptyCell;
    }
  }
}

void GenerationModel::generate_maze() {
  for (int i = 0; i < rows_ - 1; ++i) {
    join_sets();
    build_vertical_walls(i);
    build_horizontal_walls(i);
    prepare_next_row(i);
  }

  join_sets();
  build_vertical_walls(rows_ - 1);
  fix_last_row();
  validate_walls();
}

void GenerationModel::save_maze_to_file(const std::string &filepath) const {
  std::fstream fs(filepath, std::ios::out);
  fs << rows_ << ' ' << cols_ << "\n\n";
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

void GenerationModel::fix_last_row() {
  for (int i = 0; i < cols_ - 1; ++i) {
    if (line_[i] != line_[i + 1]) {
      vertical_walls_[rows_ - 1][i] = kEmptyCell;
      int set_to_delete = line_[i + 1];
      for (int j = 0; j < cols_; ++j) {
        if (line_[j] == set_to_delete) {
          line_[j] = line_[i];
        }
      }
    }
  }
}

}  // namespace s21
