#include <source_location.h>

QString Source_location::to_string() const
{
    return QString("%1:%2 '%3'").arg(file_name).arg(line).arg(function_name);
}
