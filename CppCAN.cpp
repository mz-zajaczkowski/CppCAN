#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <string.h>
#include <vector>

struct message
{
    timeval timestamp;
    std::string if_id;
    uint16_t msg_id;
    uint8_t data[8];

    message() : timestamp({1234567890,123456}), if_id("can0"), msg_id(0), data{0,0,0,0,0,0,0,0}{}
    message(const message& other)
    {
        if (this != &other)
        {
            *this = other;
        }
    }

    message& operator=(const message& other)
    {
        if (this != &other)
        {
            timestamp = {other.timestamp.tv_sec, other.timestamp.tv_usec};
            if_id = other.if_id;
            msg_id = other.msg_id;
            memcpy(data, other.data, 8);
        }
        return *this;
    }


    friend std::ostream& operator<<(std::ostream& stream, const message& msg)
    {
            stream << "(";
            stream << msg.timestamp.tv_sec << ".";
            stream << msg.timestamp.tv_usec << ")  ";
            stream << msg.if_id << "  ";
            stream << msg.msg_id << "#";
            for (int i = 0; i < 8; i++)
                stream << std::to_string(msg.data[i]);
            return stream;
    }

    friend std::istream& operator>>(std::istream& in_stream, message &msg)
    {
        in_stream >> msg.timestamp.tv_sec;
        in_stream >> msg.timestamp.tv_usec;
        in_stream >> msg.if_id;
        in_stream >> msg.msg_id;
        for(size_t i = 0; i < 8; i++)
            in_stream >> msg.data[i];
        return in_stream;
    }
};


int main()
{
    std::string test_str = "(1387326902.033281) can0 084#9032FFFE79608699\n";
    std::stringstream ss;
    ss << test_str;
    std::string line;
    std::getline(ss, line);
    std::vector<char> replace({'.', '#'});
    std::vector<char> remove({'(', ')'});
    for (auto rchar : replace)
    {
        size_t found = line.find(rchar, 0);
        if (found != std::string::npos)
            line.replace(found, 1, " ");
    }
    for (auto rchar : remove)
    {
        size_t found = line.find(rchar, 0);
        if (found != std::string::npos)
            line.replace(found, 1, "");
    }
        
    message one;
    ss << line;
    ss >> one;
    std::cout << one << std::endl;
}
