#include <iostream>
#include <SFML/Network.hpp>
#include <sstream>
#include <string>
#include <vector>

static bool port_is_open(const std::string& address, int port)
{
    return (sf::TcpSocket().connect(address, port) == sf::Socket::Done);
}


static std::vector<std::string> split(const std::string& string,
                                      char delimiter = ' ',
                                      bool allow_empty = false)
{
    std::vector<std::string> tokens;
    std::stringstream sstream(string);
    std::string token;
    while (std::getline(sstream, token, delimiter)) {
        if (allow_empty || token.size() > 0)
            tokens.push_back(token);
    }
    return tokens;
}

static int string_to_int(const std::string& string)
{
    std::stringstream sstream(string);
    int i;
    sstream >> i;
    return i;
}

template <typename T>
static void swap(T& a, T& b)
{
    T c = a;
    a = b;
    b = c;
}

template <typename T>
static std::vector<T> range(T min, T max)
{
    if (min > max)
        swap(min, max);
    if (min == max)
        return std::vector<T>(1, min);
    std::vector<T> values;
    for (; min <= max; ++min)
        values.push_back(min);
    return values;
}

static std::vector<int> parse_ports_list(const std::string& list)
{
    std::vector<int> ports;
    for (const std::string& token : split(list, ',')) {
        std::vector<std::string> strrange = split(token, '-');
        switch (strrange.size()) {
            case 0: ports.push_back(string_to_int(token));       break;
            case 1: ports.push_back(string_to_int(strrange[0])); break;
            case 2:
            {
                int min = string_to_int(strrange[0]),
                    max = string_to_int(strrange[1]);
                for (int port : range(min, max))
                    ports.push_back(port);
                break;
            }
            default:
                break;
        }
    }
    return ports;
}

int main()
{
    std::string address;
    std::string port_list;
    std::vector<int> ports;
    std::cout << "Address: " << std::flush;
    std::getline(std::cin, address);
    std::cout << "Port: " << std::flush;
    std::getline(std::cin, port_list);
    ports = parse_ports_list(port_list);
    std::cout << "Scanning " << address << "...\n";
    for (int port : ports) {
        std::cout << "Port " << port << " : ";
        if (port_is_open(address, port))
            std::cout << "OPEN\n";
        else
            std::cout << "CLOSED\n";
    }
    std::cout << std::flush;
    return 0;
}
