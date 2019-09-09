#include "logvisu_window.h"
#include <log_view.h>
#include <logs_view.h>
#include <QFileDialog>
#include <QStatusBar>
#include <QMenuBar>
#include <QCoreApplication>
#include <QVBoxLayout>
// wip/dbg
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

Logvisu_window::Logvisu_window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString::fromUtf8(title));
    build_menu_bar_();
    build_status_bar_();
    build_central_widget_();
    showMaximized();
}

Logvisu_window::~Logvisu_window()
{
}

// SLOTS:

void Logvisu_window::on_load_log_action()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open log file"));
    if (!filename.isEmpty())
    {
        if (QFile::exists(filename))
        {
            logs_.load_from_file(filename.toStdString());
            logs_view_->set_model(logs_);
        }
    }
}

// PRIVATE:

void Logvisu_window::build_menu_bar_()
{
    menu_bar_ = new QMenuBar(this);
    menu_bar_->setObjectName(QString::fromUtf8("menu_bar"));

    QMenu* file_menu = build_file_menu_();
    menu_bar_->addMenu(file_menu);
    menu_bar_->addAction(file_menu->menuAction());

    setMenuBar(menu_bar_);
}

QMenu* Logvisu_window::build_file_menu_()
{
    QMenu* file_menu = new QMenu(menu_bar_);
    file_menu->setObjectName(QString::fromUtf8("file_menu"));
    file_menu->setTitle(QCoreApplication::translate(title, "&File", nullptr));

    load_log_action_ = new QAction(this);
    load_log_action_->setObjectName(QString::fromUtf8("load_log_action"));
    load_log_action_->setText(QCoreApplication::translate(title, "&Load log", nullptr));
    load_log_action_->setShortcut(QCoreApplication::translate(title, "Ctrl+L", nullptr));
    QObject::connect(load_log_action_, &QAction::triggered, this, &Logvisu_window::on_load_log_action);

    quit_action_ = new QAction(this);
    quit_action_->setObjectName(QString::fromUtf8("quit_action"));
    quit_action_->setText(QCoreApplication::translate(title, "&Quit", nullptr));
    quit_action_->setShortcut(QCoreApplication::translate(title, "Ctrl+Q", nullptr));
    QObject::connect(quit_action_, &QAction::triggered, this, &Logvisu_window::close);

    file_menu->addAction(load_log_action_);
    file_menu->addAction(quit_action_);

    return file_menu;
}

void Logvisu_window::build_central_widget_()
{
    logs_view_ = new Logs_view(logs_);
    setCentralWidget(logs_view_);
}

void Logvisu_window::build_status_bar_()
{
    status_bar_ = new QStatusBar(this);
    status_bar_->setObjectName(QString::fromUtf8("status_bar"));
    status_bar_->showMessage(QString::fromUtf8(qVersion()), 5000);
    setStatusBar(status_bar_);
}

////////////////////////////////////////////////////////////////////////////////
