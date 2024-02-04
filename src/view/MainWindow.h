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
  void on_find_route_button_clicked() const noexcept;
  void on_generate_map_button_clicked() const noexcept;
  bool draw_file(const Cairo::RefPtr<Cairo::Context>& cairo);

  Glib::RefPtr<Gtk::Builder> builder_;

  Gtk::Label* error_label_ = nullptr;
  Gtk::FileChooserButton* file_selector_ = nullptr;
  Gtk::DrawingArea* maze_drawing_area_ = nullptr;

  Gtk::Entry* maze_rows_entry_ = nullptr;
  Gtk::Entry* maze_columns_entry_ = nullptr;
  Gtk::Button* find_route_button_ = nullptr;
  Gtk::Button* generate_map_button_ = nullptr;

  GenerationController* controller_ = nullptr;
};
}  // namespace s21

#endif  // INC_3D_VIEWER_TEMPLATE_MAINWINDOW_H
