#include "MainWindow.h"

namespace s21 {
MainWindow::MainWindow(BaseObjectType *obj,
                       Glib::RefPtr<Gtk::Builder> const &builder)
    : Gtk::Window(obj), builder_{builder} {
  builder_->get_widget("error_label", error_label_);
  builder_->get_widget("file_selector", file_selector_);
  builder_->get_widget("maze_drawing_area", maze_drawing_area_);

  builder_->get_widget("filename_entry", filename_entry_);
  builder_->get_widget("maze_rows_entry", maze_rows_entry_);
  builder_->get_widget("maze_columns_entry", maze_columns_entry_);
  builder_->get_widget("generate_map_button", generate_map_button_);

  builder_->get_widget("start_cell_x_entry", start_cell_x_entry_);
  builder_->get_widget("start_cell_y_entry", start_cell_y_entry_);
  builder_->get_widget("end_cell_x_entry", end_cell_x_entry_);
  builder_->get_widget("end_cell_y_entry", end_cell_y_entry_);
  builder_->get_widget("find_route_button", find_route_button_);

  find_route_button_->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_find_route_button_clicked));
  generate_map_button_->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_generate_map_button_clicked));
  maze_drawing_area_->signal_draw().connect(
      sigc::mem_fun(*this, &MainWindow::draw_file));
  file_selector_->signal_file_set().connect(
      sigc::mem_fun(*this, &MainWindow::on_file_chosen));

  controller_ = new GenerationController;
}

MainWindow::~MainWindow() { delete controller_; }

void MainWindow::on_find_route_button_clicked() noexcept {
  find_path_request = true;
  maze_drawing_area_->queue_draw();
}

void MainWindow::on_file_chosen() noexcept { find_path_request = false; }

int MainWindow::validate_number(const std::string &str) {
  size_t ptr = 0;
  int res = 0;

  try {
    res = std::stoi(str, &ptr);
  } catch (std::invalid_argument &e) {
    throw std::logic_error("Conversion error!");
  }
  if (ptr != str.size()) {
    throw std::logic_error("Conversion error!");
  }

  return res;
}

void MainWindow::on_generate_map_button_clicked() const noexcept {
  std::string str_rows = maze_rows_entry_->get_text();
  std::string str_cols = maze_columns_entry_->get_text();
  std::string filename = filename_entry_->get_text();
  if (filename.empty()) {
    error_label_->set_text("Empty filename!");
    return;
  }

  int rows = 0, cols = 0;
  try {
    rows = validate_number(str_rows);
    cols = validate_number(str_cols);
    if (rows < 1 || rows > 50 || cols < 1 || cols > 50) {
      throw std::logic_error("Invalid range");
    }
  } catch (std::logic_error &e) {
    error_label_->set_text(e.what());
    return;
  }

  controller_->regenerate_map(rows, cols);
  controller_->save_map(filename);
}

bool MainWindow::draw_file(const Cairo::RefPtr<Cairo::Context> &cairo) {
  std::string filepath = file_selector_->get_filename();

  if (filepath.empty()) {
    return false;
  }

  int width = maze_drawing_area_->get_allocated_width();
  int height = maze_drawing_area_->get_allocated_height();

  cairo->set_line_width(2);
  cairo->set_source_rgb(0, 0, 0);

  std::fstream fs(filepath, std::ios::in);
  fs.exceptions(std::fstream::badbit);

  int rows = 0, cols = 0;
  auto draw_walls = [&](bool vertical) {
    cairo->move_to(kCairoOffset, kCairoOffset);
    cairo->line_to(vertical ? kCairoOffset : width - kCairoOffset,
                   vertical ? height - kCairoOffset : kCairoOffset);
    cairo->move_to(kCairoOffset, kCairoOffset);

    double x_step = (width - kCairoOffset * 2) / (double)cols;
    double y_step = (height - kCairoOffset * 2) / (double)rows;

    for (int i = 0; i < rows; ++i) {
      int y = std::round(i * y_step +
                         (vertical ? kCairoOffset : kCairoOffset + y_step));
      for (int j = 0; j < cols; ++j) {
        int x = std::round(j * x_step +
                           (vertical ? x_step + kCairoOffset : kCairoOffset));
        cairo->move_to(x, y);

        int wall;
        fs >> wall;
        if (wall) {
          if (vertical) {
            cairo->line_to(x, y + y_step);
          } else {
            cairo->line_to(x + x_step, y);
          }
        }
      }
    }
  };

  try {
    fs >> rows >> cols;
    draw_walls(true);
    draw_walls(false);
  } catch (const std::fstream::failure &e) {
    error_label_->set_text(e.what());
    return false;
  }
  cairo->stroke();

  if (find_path_request) {
    int xs, ys, xe, ye;
    std::vector<std::pair<int, int>> path;
    try {
      xs = validate_number(start_cell_x_entry_->get_text());
      ys = validate_number(start_cell_y_entry_->get_text());
      xe = validate_number(end_cell_x_entry_->get_text());
      ye = validate_number(end_cell_y_entry_->get_text());
      path = controller_->find_route(xs, ys, xe, ye, filepath);
    } catch (std::logic_error &e) {
      error_label_->set_text(e.what());
      return false;
    }

    if (path.size() <= 1) {
      return false;
    }

    double x_step = (width - kCairoOffset * 2) / (double)cols;
    double y_step = (height - kCairoOffset * 2) / (double)rows;
    double half_step_x = x_step / 2;
    double half_step_y = y_step / 2;
    for (size_t i = 0; i < path.size() - 1; ++i) {
      cairo->move_to(kCairoOffset + path[i].first * x_step - half_step_x,
                     kCairoOffset + path[i].second * y_step - half_step_y);
      cairo->line_to(kCairoOffset + path[i + 1].first * x_step - half_step_x,
                     kCairoOffset + path[i + 1].second * y_step - half_step_y);
    }

    cairo->set_source_rgb(255, 0, 0);
    cairo->stroke();
  }

  error_label_->set_text("");
  return false;
}

}  // namespace s21
