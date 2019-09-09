#include <logs_model.h>
#include <fstream>
// wip/dbg
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

Logs_model::Logs_model()
{
//    Log log;

//    std::array<std::string, 13> lines =
//    {
//        "#1:[2019-09-07 14:49:43.743814][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:18 'int main(int, char**)'] trace - ",
//        "#2:[2019-09-07 14:49:43.754023][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:21 'int main(int, char**)'] info - Info",
//        "#3:[2019-09-07 14:49:43.754165][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:22 'int main(int, char**)'] warning - Warning",
//        "#4:[2019-09-07 14:49:43.754221][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:23 'int main(int, char**)'] error - Error",
////        "#5:[2019-09-07 14:49:43.754275][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:24 'int main(int, char**)'] fatal - Fatal",
//        "#6:[2019-09-07 14:49:43.754329][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:11 'void tfunc(const Container&) [with Container = std::vector<int>]'] trace - ",
//        "#7:[2019-09-07 14:49:43.754385][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:32 'int main(int, char**)'] debug - Messsage( 0 )End.",
//        "#8:[2019-09-07 14:49:43.754439][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:32 'int main(int, char**)'] info - Messsage( 1 ),",
//        "8 continue..",
//        "8 continue...",
//        "8 continue....",
//        "8 End",
//        "#9:[2019-09-07 14:49:43.754496][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:32 'int main(int, char**)'] info - Messsage( 2 )End.012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789",
//        "#10:[2019-09-07 14:49:43.754550][/home/agadrion/dev/Belianpe/Rebirth/lib/program_log/demo/plog_demo_trivial.cpp:32 'int main(int, char**)'] info - Messsage( 3 )End."
//    };

//    for (const std::string& line : lines)
//    {
//        bool parse_res = Log::parse_line(line, log);
//        if (parse_res)
//            logs_.push_back(std::move(log));
//        else if (!logs_.empty())
//            logs_.back().append_line(log.message());
//    }
}

int Logs_model::rowCount(const QModelIndex& ) const
{
    return static_cast<int>(logs_.size());
}

QVariant Logs_model::data(const QModelIndex& index, int ) const
{
    std::size_t row_index = static_cast<std::size_t>(index.row());
    return QVariant::fromValue(logs_.at(row_index));
}

void Logs_model::load_from_file(const std::string& filename)
{
    std::ifstream stream(filename);
    if (!stream)
        return;

    std::string line;
    Log log;
    while (stream && !stream.eof())
    {
        std::getline(stream, line);
        bool parse_res = Log::parse_line(line, log);
        if (parse_res)
            logs_.push_back(std::move(log));
        else if (!logs_.empty())
            logs_.back().append_line(log.message());
    }
}

////////////////////////////////////////////////////////////////////////////////
