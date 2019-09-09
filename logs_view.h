#ifndef LOGS_VIEW_H
#define LOGS_VIEW_H

#include <QListWidget>

class Log;
class Logs_model;

class Logs_view : public QListWidget
{
public:
    Logs_view(const Logs_model& logs);
    void set_model(const Logs_model& logs);

public slots:
    void data_changed(const QModelIndex& top, const QModelIndex& bottom, const QVector<int>& = QVector<int>());
    void rows_inserted(const QModelIndex& parent, int first, int last);

protected:
//    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    void insert_log(int row, const Log& log);
    void toggle_log_view(QListWidgetItem* item);

private:
    const Logs_model* logs_ = nullptr;
};

#endif // LOGS_VIEW_H
