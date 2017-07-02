#ifndef CONFIG_FILE_H_
#define CONFIG_FILE_H_

#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
using section_t = std::pair<std::string, std::map<std::string, std::string>>;

class ConfigFile {
public:
    std::vector<section_t> content;
    ConfigFile(std::string configFile);
    std::vector<section_t>::iterator get_section(std::string section);
    bool has_section(std::string section);
    bool has_value(std::string section, std::string entry);
    std::string value(std::string section, std::string entry,
                        std::string default_value);
    double value_to_double(std::string section, std::string entry,
                            const double default_value);
    std::vector<double> value_to_vector(std::string section, std::string entry,
                                        const unsigned int default_size);
};

#endif // CONFIG_FILE_H_
