#include <logs_view.h>
#include <logs_model.h>
#include <log_view.h>
#include <QLabel>
#include <QKeyEvent>
// wip/debug
#include <iostream>

// https://forum.qt.io/topic/17481/easiest-way-to-have-a-simple-list-with-custom-items

////////////////////////////////////////////////////////////////////////////////

Logs_view::Logs_view(const Logs_model& logs)
{
    set_model(logs);

    setSelectionMode(QListWidget::SelectionMode::ExtendedSelection);

    QObject::connect(this, &Logs_view::itemActivated, this, &Logs_view::toggle_log_view);
}

void Logs_view::set_model(const Logs_model& logs)
{
    // disconnect
    if (logs_)
    {
        QObject::disconnect(logs_, &Logs_model::rowsInserted, this, &Logs_view::rows_inserted);
        QObject::disconnect(logs_, &Logs_model::dataChanged, this, &Logs_view::data_changed);
        clear();
    }

    // set rows view
    logs_ = &logs;
    for (QModelIndex index = logs.index(0); index != logs.index(logs.rowCount()); index = logs.index(index.row() + 1))
    {
        const Log& log = logs.log(index);
        insert_log(index.row(), log);
    }

    // connect
    QObject::connect(logs_, &Logs_model::rowsInserted, this, &Logs_view::rows_inserted);
    QObject::connect(logs_, &Logs_model::dataChanged, this, &Logs_view::data_changed);
}

// SLOTS:

void Logs_view::data_changed(const QModelIndex& top, const QModelIndex& bottom, const QVector<int>&)
{
    int row = top.row();
    int end_row = bottom.row() + 1;
    for (; row != end_row; ++row)
    {
        QModelIndex index = logs_->index(row);
        const Log& log = logs_->log(index);
        QWidget* widget = itemWidget(item(row));
        Log_view* log_view = dynamic_cast<Log_view*>(widget);
        Q_ASSERT_X(log_view && false, "data_changed", "TODO Finish implementation");
//        log_view->
    }
}

void Logs_view::rows_inserted(const QModelIndex&, int first, int last)
{
    for (; first < last; ++first)
    {
        QModelIndex index = logs_->index(first);
        const Log& log = logs_->log(index);
        insert_log(first, log);
    }
}

// PROTECTED EVENTS:

//void Logs_view::keyReleaseEvent(QKeyEvent* event)
//{
//    if (event->key() == Qt::Key::Key_Space)
//        emit activated(indexFromItem(currentItem()));
//}

// PRIVATE:

void Logs_view::insert_log(int row, const Log& log)
{
    // Create list item
    QListWidgetItem* list_widget_item = new QListWidgetItem();
    insertItem(row, list_widget_item);
    // Create log view
    Log_view* log_view = new Log_view(log);
    list_widget_item->setSizeHint(log_view->minimumSizeHint());
    // Set log view as widget item
    setItemWidget(list_widget_item, log_view);
}

void Logs_view::toggle_log_view(QListWidgetItem* item)
{
    QWidget* widget = this->itemWidget(item);
    Log_view* log_view = dynamic_cast<Log_view*>(widget);
    Q_ASSERT(log_view);
    log_view->toggle_view();
    item->setSizeHint(log_view->minimumSizeHint());
}

////////////////////////////////////////////////////////////////////////////////
