#include "configfile.h"

static std::string trim(std::string source, const char *delims = " \t\r\n") {
    std::string result(source);
    std::string::size_type index = result.find_last_not_of(delims);
    if(index != std::string::npos)
        result.erase(++index);

    index = result.find_first_not_of(delims);
    if(index != std::string::npos)
        result.erase(0, index);
    else
        result.erase();
    return result;
}

ConfigFile::ConfigFile(std::string configFile) {
    std::ifstream file(configFile.c_str());

    std::string line;
    std::string section;
    std::string name;
    std::string value;
    int posEqual;

    while (std::getline(file, line)) {
        if (!line.length())
            continue;

        if (line[0] == '#')
            continue;

        if (line[0] == '[') {
            section = trim(line.substr(1, line.find(']')-1));
            std::map<std::string, std::string> empty_map;
            content.push_back(section_t(section, empty_map));
            continue;
        }

        posEqual = line.find('=');
        name = trim(line.substr(0, posEqual));
        value = trim(line.substr(posEqual + 1));
        content.back().second[name] = value;
    }
}
std::vector<section_t>::iterator ConfigFile::get_section(std::string section) {
    auto it = content.begin();
    while(it != content.end()) {
        if(it->first == section)
            break;
        it++;
    }
    return it;
}

bool ConfigFile::has_section(std::string section) {
    return get_section(section) != content.end();
}

bool ConfigFile::has_value(std::string section, std::string entry) {
    return has_section(section)
            && (get_section(section)->second.find(entry) !=
                get_section(section)->second.end());
}

std::string ConfigFile::value(std::string section, std::string entry,
                                std::string default_value) {
    if (has_value(section, entry))
        return get_section(section)->second[entry];
    else
        return default_value;
}

double ConfigFile::value_to_double(std::string section, std::string entry,
                                    const double default_value = -1.0) {
    std::string sresult(value(section, entry, "nan"));
    if(sresult == "nan")
        return default_value;
    else
        return stod(sresult);
}

std::vector<double> ConfigFile::value_to_vector(std::string section,
                    std::string entry, const unsigned int default_size = 1) {
    std::stringstream sresult(value(section, entry, ""));
    std::vector<double> result;
    double aux;
    while(sresult >> aux)
        result.push_back(aux);
    if(result.size())
        return result;
    else
        return std::vector<double>(default_size, 10.0);
}
