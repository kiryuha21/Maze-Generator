#ifndef INC_3D_VIEWER_TEMPLATE_MAINWINDOW_H
#define INC_3D_VIEWER_TEMPLATE_MAINWINDOW_H

#include <gtkmm.h>

#include <fstream>
#include <iostream>

#include "../controller/GenerationController.h"

namespace s21 {
class MainWindow : public Gtk::Window {
 public:
  MainWindow(BaseObjectType* obj, Glib::RefPtr<Gtk::Builder> const& builder);
  ~MainWindow() override;

 private:
  static constexpr auto kCairoOffset = 5;

  void on_find_route_button_clicked() const noexcept;
  void on_generate_map_button_clicked() const noexcept;
  bool draw_file(const Cairo::RefPtr<Cairo::Context>& cairo);
  static int validate_number(const std::string& str);

  Glib::RefPtr<Gtk::Builder> builder_;

  Gtk::Label* error_label_ = nullptr;
  Gtk::FileChooserButton* file_selector_ = nullptr;
  Gtk::DrawingArea* maze_drawing_area_ = nullptr;

  Gtk::Entry* filename_entry_ = nullptr;
  Gtk::Entry* maze_rows_entry_ = nullptr;
  Gtk::Entry* maze_columns_entry_ = nullptr;
  Gtk::Button* generate_map_button_ = nullptr;

  Gtk::Entry* start_cell_x_entry_ = nullptr;
  Gtk::Entry* start_cell_y_entry_ = nullptr;
  Gtk::Entry* end_cell_x_entry_ = nullptr;
  Gtk::Entry* end_cell_y_entry_ = nullptr;
  Gtk::Button* find_route_button_ = nullptr;

  GenerationController* controller_ = nullptr;
};
}  // namespace s21

#endif  // INC_3D_VIEWER_TEMPLATE_MAINWINDOW_H
