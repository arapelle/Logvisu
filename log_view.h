#ifndef LOG_VIEW_H
#define LOG_VIEW_H

#include <log.h>
#include <QWidget>

class QLabel;

class Metadata_view : public QWidget
{
public:
    Metadata_view();
    Metadata_view(const Log::Metadata& metadata);
    void set_model(const Log::Metadata& metadata);

private:
    QLayout* build_datetime_view();
    QLayout* build_source_location_view();

    static QFont build_underline_font();

private:
    const Log::Metadata* metadata_ = nullptr;
    QLabel* date_label_ = nullptr;
    QLabel* time_label_ = nullptr;
    QLabel* file_label_ = nullptr;
    QLabel* line_label_ = nullptr;
    QLabel* func_label_ = nullptr;

    static QFont underline_font_;
    static QFont font_;
};

class Log_view : public QWidget
{
public:
    Log_view();
    Log_view(const Log& log);
    void set_model(const Log& log);
    const QLabel* label() const { return message_label_; }
    void toggle_view();

//private:
    const Log* log_ = nullptr;
    QWidget* log_widget_ = nullptr;
    Metadata_view* metadata_view_ = nullptr;
    QLabel* message_label_ = nullptr;
};

#endif // LOG_VIEW_H
