#include "MainWindow.h"

namespace s21 {
MainWindow::MainWindow(BaseObjectType *obj,
                       Glib::RefPtr<Gtk::Builder> const &builder)
    : Gtk::Window(obj), builder_{builder} {
  builder_->get_widget("error_label", error_label_);
  builder_->get_widget("file_selector", file_selector_);
  builder_->get_widget("maze_drawing_area", maze_drawing_area_);

  builder_->get_widget("load_map_button", load_map_button_);
  builder_->get_widget("find_route_button", find_route_button_);
  builder_->get_widget("generate_map_button", generate_map_button_);

  load_map_button_->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_load_map_button_clicked));
  find_route_button_->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_find_route_button_clicked));
  generate_map_button_->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_generate_map_button_clicked));
  maze_drawing_area_->signal_draw().connect(
      sigc::mem_fun(*this, &MainWindow::draw_file));

  controller_ = new GenerationController;
}

MainWindow::~MainWindow() { delete controller_; }

void MainWindow::on_load_map_button_clicked() const noexcept {
  maze_drawing_area_->queue_draw();
}

void MainWindow::on_find_route_button_clicked() const noexcept {}

void MainWindow::on_generate_map_button_clicked() const noexcept {}

bool MainWindow::draw_file(const Cairo::RefPtr<Cairo::Context> &cairo) {
  std::string filepath = file_selector_->get_filename();

  if (filepath.empty()) {
    return true;
  }

  int width = maze_drawing_area_->get_allocated_width();
  int height = maze_drawing_area_->get_allocated_height();

  cairo->set_line_width(2);
  cairo->set_source_rgb(0, 0, 0);

  std::fstream fs(filepath, std::ios::in);
  int rows, cols;
  fs >> rows >> cols;

  // 5 - offset, change to constant while moving business-logic to model
  cairo->move_to(5, 5);
  cairo->line_to(5, height - 5);
  cairo->move_to(5, 5);

  int x_step = (width - 10) / cols;
  int y_step = (height - 10) / rows;

  for (int i = 0; i < rows; ++i) {
    int y = i * y_step + 5;
    for (int j = 0; j < cols; ++j) {
      int x = j * x_step + x_step + 5;
      cairo->move_to(x, y);

      int wall;
      fs >> wall;
      if (wall) {
        cairo->line_to(x, y + y_step);
      }
    }
  }

  cairo->move_to(5, 5);
  cairo->line_to(width - 5, 5);
  cairo->move_to(5, 5);
  for (int i = 0; i < rows; ++i) {
    int y = i * y_step + y_step + 5;
    for (int j = 0; j < cols; ++j) {
      int x = j * x_step + 5;
      cairo->move_to(x, y);

      int wall;
      fs >> wall;
      if (wall) {
        cairo->line_to(x + x_step, y);
      }
    }
  }
  cairo->stroke();

  return true;
}

}  // namespace s21
