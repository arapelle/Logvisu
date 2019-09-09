#ifndef SOURCE_LOCATION_H
#define SOURCE_LOCATION_H

#include <QString>

class Source_location
{
public:
    QString file_name;
    quint64 line;
    QString function_name;

    QString to_string() const;
};

#endif // SOURCE_LOCATION_H
