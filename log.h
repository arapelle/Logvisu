#ifndef LOG_H
#define LOG_H

#include <source_location.h>
#include <QString>
#include <QDateTime>
#include <QObject>

class Datetime
{
public:
    QDateTime datetime;
    quint32 microseconds;

    bool operator<(const Datetime& other) const;
};

class Level
{
public:
    enum Value : quint8
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal,
        Unknown
    };

    Level(Value value);
    QString to_string() const;
    operator Value () const { return value_; }
    static Level from_string(const QString& str);

private:
    Value value_;
};

class Log
{
public:
    class Metadata
    {
    public:
        quint64 index;
        Datetime datetime;
        Source_location source_location;
        Level level = Level::Unknown;
    };

    Log(const QString& line = QString());
    Log(const Metadata& metadata, const QString& message);
    virtual ~Log();
    inline const Metadata& metadata() const { return metadata_; }
    inline const QString& message() const { return message_; }
    void append_line(const QString& line);
    QString to_string() const;

    static bool parse_line(const std::string& line, Log& current_log);

private:
    static std::string log_regex_str_();

private:
    Metadata metadata_;
    QString message_;
};

Q_DECLARE_METATYPE(Log)

#endif // LOG_H
