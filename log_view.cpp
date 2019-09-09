#include <log_view.h>
#include <log.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
// wip/dbg
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

QFont Metadata_view::underline_font_ = build_underline_font();
QFont Metadata_view::font_("Arial", 9);

Metadata_view::Metadata_view()
{
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setMargin(3);
    vlayout->setSpacing(0);
    vlayout->addLayout(build_datetime_view());
    vlayout->addSpacing(1);
    vlayout->addLayout(build_source_location_view());
    setLayout(vlayout);
    setStyleSheet("QLabel{ color : #666666 }");
}

Metadata_view::Metadata_view(const Log::Metadata& metadata)
{
    set_model(metadata);
}

void Metadata_view::set_model(const Log::Metadata& metadata)
{
    metadata_ = &metadata;
    date_label_->setText(metadata_->datetime.datetime.date().toString());
    time_label_->setText(metadata_->datetime.datetime.time().toString() + "." + QString::number(metadata_->datetime.microseconds));
    file_label_->setText(metadata_->source_location.file_name);
    line_label_->setText(QString::number(metadata_->source_location.line));
    func_label_->setText(metadata_->source_location.function_name);
}

// PRIVATE:

QLayout* Metadata_view::build_datetime_view()
{
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setMargin(0);
    hlayout->setAlignment(Qt::AlignLeft);

    // date label
    QLabel* date_label = new QLabel("Date:");
    date_label->setMargin(0);
    date_label->setFont(underline_font_);
    hlayout->addWidget(date_label);
    hlayout->addSpacing(5);
    // log date label
    date_label_ = new QLabel();
    date_label_->setMargin(0);
    date_label_->setFont(font_);
    hlayout->addWidget(date_label_);

    // spacing
    hlayout->addSpacing(64);

    // time label
    QLabel* time_label = new QLabel("Time:");
    time_label->setMargin(0);
    time_label->setFont(underline_font_);
    hlayout->addWidget(time_label);
    hlayout->addSpacing(5);
    // log time label
    time_label_ = new QLabel();
    time_label_->setMargin(0);
    time_label_->setFont(font_);
    hlayout->addWidget(time_label_);

    return hlayout;
}

QLayout* Metadata_view::build_source_location_view()
{
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setMargin(0);
    hlayout->setAlignment(Qt::AlignLeft);

    // file label
    QLabel* file_label = new QLabel("File:");
    file_label->setMargin(0);
    file_label->setFont(underline_font_);
    hlayout->addWidget(file_label);
    hlayout->addSpacing(5);
    // log file label
    file_label_ = new QLabel();
    file_label_->setMargin(0);
    file_label_->setFont(font_);
    hlayout->addWidget(file_label_);

    // spacing
    hlayout->addSpacing(64);

    // line label
    QLabel* line_label = new QLabel("Line:");
    line_label->setMargin(0);
    line_label->setFont(underline_font_);
    hlayout->addWidget(line_label);
    hlayout->addSpacing(5);
    // log line label
    line_label_ = new QLabel();
    line_label_->setMargin(0);
    line_label_->setFont(font_);
    hlayout->addWidget(line_label_);

    // spacing
    hlayout->addSpacing(64);

    // func label
    QLabel* func_label = new QLabel("Function:");
    func_label->setMargin(0);
    func_label->setFont(underline_font_);
    hlayout->addWidget(func_label);
    hlayout->addSpacing(5);
    // log func label
    func_label_ = new QLabel();
    func_label_->setMargin(0);
    func_label_->setFont(font_);
    hlayout->addWidget(func_label_);

    return hlayout;
}

QFont Metadata_view::build_underline_font()
{
    QFont font("Arial", 9);
    font.setUnderline(true);
    return font;
}

////////////////////////////////////////////////////////////////////////////////

Log_view::Log_view()
    : log_(nullptr)
{
    log_widget_ = new QWidget();

    QHBoxLayout* line_layout = new QHBoxLayout();
    line_layout->setMargin(3);
    line_layout->setSpacing(10);

    line_layout->addSpacing(5);

    QLabel* mark_label_ = new QLabel("¤");
    mark_label_->setAlignment(Qt::AlignTop);
    mark_label_->setMaximumWidth(mark_label_->sizeHint().width());
    line_layout->addWidget(mark_label_);

    QVBoxLayout* log_layout = new QVBoxLayout();
    log_layout->setMargin(0);
    log_layout->setSpacing(0);

    metadata_view_ = new Metadata_view();
    log_layout->addWidget(metadata_view_);

    static QFont font("Monospace", 11);
    message_label_ = new QLabel(this);
    message_label_->setFont(font);
    log_layout->addWidget(message_label_);

    line_layout->addLayout(log_layout);

    log_widget_->setLayout(line_layout);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->setMargin(1);
    layout->addWidget(log_widget_);
    setLayout(layout);
}

Log_view::Log_view(const Log& log)
    : Log_view()
{
    set_model(log);
}

void Log_view::set_model(const Log& log)
{
    log_ = &log;
    metadata_view_->set_model(log.metadata());
    metadata_view_->setVisible(log.metadata().level == Level::Trace);

    message_label_->setText(log.message());
    message_label_->setVisible(log.metadata().level != Level::Trace);

    QColor background_color = Qt::GlobalColor::white;
    switch (log.metadata().level)
    {
    case Level::Trace:
        background_color = QColor(215, 255, 255);
        break;
    case Level::Info:
        background_color = QColor(255, 255, 255);
        break;
    case Level::Debug:
        background_color = QColor(200, 255, 200);
        break;
    case Level::Warning:
        background_color = QColor(250, 250, 200);
        break;
    case Level::Error:
        background_color = QColor(255, 221, 221);
        break;
    case Level::Fatal:
        background_color = QColor(255, 170, 170);
        break;
    default:
        ;
    }
    QString style_sheet = QString("background-color : %1").arg(background_color.name());
    log_widget_->setStyleSheet(style_sheet);
}

void Log_view::toggle_view()
{
    if (log_->metadata().level != Level::Trace)
    {
        metadata_view_->setVisible(!metadata_view_->isVisible());
        int max_height = maximumHeight();
        setMaximumHeight(0);
        setMaximumHeight(max_height);
    }
}

////////////////////////////////////////////////////////////////////////////////
