#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <sys/time.h>

struct message
{
    timeval timestamp;
    std::string if_id;
    uint16_t msg_id;
    uint16_t data[8];

    message() : timestamp({1234567890,123456}), if_id("can0"), msg_id(0), data{0,0,0,0,0,0,0,0}{}

    friend std::ostream& operator<<(std::ostream& stream, const message& msg)
    {
            stream << "(";
            stream << msg.timestamp.tv_sec << ".";
            stream << msg.timestamp.tv_usec << ")  ";
            stream << msg.if_id << "  ";
            stream << msg.msg_id << "#";
            for (int i = 0; i < 8; i++)
                stream << std::hex << msg.data[i];
            return stream;
    }

    friend std::istream& operator>>(std::istream& in_stream, message &msg)
    {
        in_stream >> msg.timestamp.tv_sec;
        in_stream >> msg.timestamp.tv_usec;
        in_stream >> msg.if_id;
        in_stream >> msg.msg_id;
        for(size_t i = 0; i < 8; i++)
            in_stream >> std::hex >> msg.data[i];
        return in_stream;
    }
};

void parseFile(std::string& infile_name, std::vector<message>& v_msg)
{
	std::fstream infile;
	infile.open(infile_name, std::fstream::in);
	std::string line;
	std::stringstream ss;
	while(std::getline(infile, line))
	{
    	line.replace(0,1,"");
    	line.replace(27, 1, " ");
    	line.replace(10, 1, " ");
    	line.replace(17, 1, "");
    	std::string data = line.substr(27, 16);
    	for (size_t pos=1; pos < 8; pos++)
			data.insert(2 * pos + pos - 1, " ");
    	line.replace(27, data.length(), data);
		message parsed;
		ss << line;
		ss >> parsed;
		v_msg.push_back(parsed);
		ss.clear();
	}
	infile.close();
}


int main()
{
    std::string test_str = "(1387326902.033281) can0 084#9032FFFE79608699\n";
    std::stringstream ss;
    ss << test_str;
    std::string line;
    std::getline(ss, line);
    std::cout << line.length() << std::endl;
    std::cout << "line: " << line << std::endl;
    message one;
    ss << line;
    ss >> one;
    std::cout << "one data length: " << sizeof(one.data) << std::endl;
    std::cout << "one message:\n" << one << std::endl;
   	ss.clear();
	std::vector<message> v_msg;
	std::string fname="/home/arch/Pobrane/Test_Car_Full_HS.log";
	parseFile(fname, v_msg);
}
