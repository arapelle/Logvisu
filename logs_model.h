#ifndef LOGS_MODEL_H
#define LOGS_MODEL_H

#include <log.h>
#include <QAbstractListModel>
#include <vector>

class Logs_model : public QAbstractListModel
{
public:
    Logs_model();
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    inline const Log& log(const QModelIndex& index) const { return logs_[static_cast<std::size_t>(index.row())]; }
    void load_from_file(const std::string& filename);

private:
    std::vector<Log> logs_;
};

#endif // LOGS_MODEL_H
