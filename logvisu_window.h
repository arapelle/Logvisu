#ifndef LOGVISU_WINDOW_H
#define LOGVISU_WINDOW_H

#include <QMainWindow>
#include <logs_model.h>

class Logs_view;

class Logvisu_window : public QMainWindow
{
    Q_OBJECT

public:
    static constexpr const char* title = "Logvisu";

    Logvisu_window(QWidget *parent = nullptr);
    ~Logvisu_window();

public slots:
    void on_load_log_action();

private:
    void build_menu_bar_();
    QMenu* build_file_menu_();
    void build_central_widget_();
    void build_status_bar_();

private:
    QMenuBar* menu_bar_ = nullptr;
    QAction* quit_action_ = nullptr;
    QAction* load_log_action_ = nullptr;
    QStatusBar* status_bar_ = nullptr;

    Logs_model logs_;
    Logs_view* logs_view_ = nullptr;
};

#endif // LOGVISU_WINDOW_H
