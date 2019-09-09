#include <log.h>
#include <regex>
#include <string>
#include <sstream>
// wip/dbg
#include <iostream>

using namespace std::literals::string_literals;

////////////////////////////////////////////////////////////////////////////////

bool Datetime::operator<(const Datetime& other) const
{
    return datetime < other.datetime || (datetime == other.datetime && microseconds < other.microseconds);
}

////////////////////////////////////////////////////////////////////////////////

Level::Level(Value value)
    : value_(value)
{}

QString Level::to_string() const
{
    static QString strs [] = { "trace", "debug", "info", "warning", "error", "fatal", "unknown" };
    if (value_ >= Unknown)
        return strs[Unknown];
    return strs[value_];
}

Level Level::from_string(const QString& str)
{
    static std::array<QString, 7> strs = { "trace", "debug", "info", "warning", "error", "fatal", "unknown" };
    auto iter = std::find(strs.begin(), strs.end(), str);
    if (iter != strs.end())
        return static_cast<Level::Value>(iter - strs.begin());
    return Level::Unknown;
}

////////////////////////////////////////////////////////////////////////////////

Log::Log(const QString& message)
: message_(message)
{
}

Log::Log(const Metadata& metadata, const QString& message)
: metadata_(metadata), message_(message)
{
}

Log::~Log ()
{
}

void Log::append_line(const QString& line)
{
    message_.append("\n" + line);
}

QString Log::to_string() const
{
    return message_;
}

// STATIC:

//#2:[2019-09-07 14:49:43.754023][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:21 'int main(int, char**)'] info - Info

bool Log::parse_line(const std::string& line, Log& log)
{
    static std::string log_regex_str = log_regex_str_();

    std::smatch match;
    if (std::regex_match(line, match, std::regex(log_regex_str, std::regex_constants::ECMAScript)))
    {
//        for (auto iter = match.begin(), end_iter = match.end(); iter != end_iter; ++iter)
//            std::cout << "MATCH: " << *iter << std::endl;
        quint64 log_number = std::stoull(match[1]);
        int year = std::stoi(match[2]);
        int month = std::stoi(match[3]);
        int day = std::stoi(match[4]);
        int hour = std::stoi(match[5]);
        int minutes = std::stoi(match[6]);
        int seconds = std::stoi(match[7]);
        quint32 microseconds = static_cast<quint32>(std::stoul(match[8]));
        QString file = QString::fromStdString(match[9]);
        quint64 line_number = std::stoull(match[10]);
        QString func = QString::fromStdString(match[11]);
        Level level = Level::from_string(QString::fromStdString(match[12]));
        QString message = QString::fromStdString(match[13]);

        Source_location source_location{file, line_number, func};
        QDate date(year, month, day);
        QTime time(hour, minutes, seconds);
        Datetime datetime{QDateTime(std::move(date), std::move(time)), microseconds};
        Metadata metadata{log_number, std::move(datetime), std::move(source_location), level};
        log.metadata_ = std::move(metadata);
        log.message_ = std::move(message);

        return true;
    }
    else
    {
        log.message_ = QString::fromStdString(line);
        return false;
    }
}

// PRIVATE:

std::string Log::log_regex_str_()
{
    // regex helpers
    const char* int_regex = "[0-9]+";
    const char* two_digits_regex = "[0-9]{2}";
    // metadata regex
    const char* log_number_regex = int_regex;
    const char* year_regex = int_regex;
    const char* month_regex = two_digits_regex;
    const char* day_regex = two_digits_regex;
    const char* hour_regex = two_digits_regex;
    const char* minutes_regex = two_digits_regex;
    const char* seconds_regex = two_digits_regex;
    const char* microseconds_regex = int_regex;
    const char* file_regex = ".*";
    const char* line_number_regex = int_regex;
    const char* func_regex = ".*";
    const char* level_regex = "\\[|trace|debug|info|warning|error|fatal|]";
    const char* message_regex = ".*";

    std::ostringstream stream;
    stream
            << "#(" << log_number_regex << "):"
           << "\\[(" << year_regex << ")-(" << month_regex << ")-(" << day_regex << ") "
           << "(" << hour_regex << "):(" << minutes_regex << "):(" << seconds_regex << ")\\.(" << microseconds_regex << ")\\]"
           << "\\[(" << file_regex << "):(" << line_number_regex << ") '(" << func_regex << ")'\\]"
           << " (" << level_regex << ") - "
           << "(" << message_regex << ")";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
