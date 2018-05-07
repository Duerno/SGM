#include "reader.h"

static std::string trim(std::string source, const char *delims = " \"\t\r\n") {
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
static std::string capitalize(std::string source) {
    std::string result(source);
    bool is_first_letter;
    for(uint i = 0; i < result.size(); i++) {
        is_first_letter = false;
        if(i == 0 || result[i-1] == ' ')
            is_first_letter = true;

        if(is_first_letter)
            result[i] = toupper(result[i]);
        else
            result[i] = tolower(result[i]);
    }
    return result;
}

static void create_file(std::string path, std::ofstream& ofs) {
    std::cerr << "File '" << path << "' could not be opened.\n";
    std::cerr << "Creating file '" << path << "'.\n";
    std::string dirname = path.substr(0, path.find('/'));
    system(std::string("mkdir -p \"" + dirname + "\"").c_str());
    ofs.open(path);
}

Reader::Reader() {
}

// csv file must contain name, id, and uri_id of each student
std::vector<Student> Reader::read_students(std::string filename) {
    std::ifstream fstudents(filename);
    if(not fstudents.good()) {
        std::cerr << "Broken database: file '" << filename  << "' could not be";
        std::cerr << " opened." << std::endl;
        exit(1);
    }
    std::vector<Student> students;
    std::string str;
    while(std::getline(fstudents, str)) {
        if(str[0] == '#')
            continue;
        std::stringstream line(str);
        Student student;
        std::getline(line, str, ',');
        student.registration_id = trim(str);
        std::getline(line, str, ',');
        student.name = capitalize(trim(str));
        std::getline(line, str);
        student.uri_id = trim(str);
        students.push_back(student);
    }
    return students;
}
std::map<std::string, std::string> Reader::read_students_uri(std::string filename) {
    std::ifstream fstudents(filename);
    if(not fstudents.good()) {
        std::cerr << "Broken database: file '" << filename  << "' could not be";
        std::cerr << " opened." << std::endl;
        exit(1);
    }
    std::map<std::string, std::string> uri_students;
    std::string str;
    while(getline(fstudents, str)) {
        std::stringstream line(str);
        std::string uri_id, uri_name;
        line >> str; // read student number
        line >> uri_id; // read student uri id
        line >> str; // read student id
        getline(line, str, '\t'); // read tab
        getline(line, uri_name, '\t'); // read student uri name
        uri_students[uri_id] = uri_name;
    }
    return uri_students;
}

std::map<std::string, double> Reader::read_grades(std::string filename,
                        const double MAX_SCORE, std::vector<Student> students) {
    std::ifstream fstudents(filename);
    if(not fstudents.good()) {
        std::ofstream ofs;
        create_file(filename, ofs);
        Discipline::sort_mode = "name";
        std::sort(students.begin(), students.end());
        for(auto student : students) {
            ofs << "-1," << student.name << "," << student.registration_id;
            ofs << std::endl;
        }
        ofs.close();
        return std::map<std::string, double>();
    }
    std::map<std::string, double> grades;
    std::string str, grade;
    while(getline(fstudents, str)) {
        std::stringstream line(str);
        getline(line, grade, ','); // read grade
        grade = trim(grade);
        if(grade == "-1")
            continue;
        getline(line, str, ','); // read name
        getline(line, str); // read registration_id
        grades[trim(str)] = stod(grade) * 10.0 / MAX_SCORE;
    }
    fstudents.close();
    return grades;
}
std::map<std::string, double> Reader::read_uri_grades(std::string filename,
                                                        std::string uri_file) {
    if(uri_file == "") {
        std::cerr << "Batadase error: when using the reader type 'URI', you ";
        std::cerr << "must specify an URI students file with parameter ";
        std::cerr << "'uri_file'.\n";
        exit(1);
    }
    std::ifstream fstudents(filename);
    if(not fstudents.good()) {
        std::ofstream ofs;
        create_file(filename, ofs);
        ofs << "Sample URI Student\t\t\t\t\t\t\t0%\n";
        ofs.close();
        return std::map<std::string, double>();
    }
    std::map<std::string, double> grades;
    std::string str;
    double grade;
    while(getline(fstudents, str)) {
        std::stringstream line(str);
        getline(line, str, '\t'); // read uri name
        line >> grade; // read uri absolute grade
        line >> grade; // read uri grade percentage
        grades[trim(str)] = grade / 10.0;
    }
    return grades;
}
std::map<std::string, double> Reader::read_boca_grades(std::string filename,
                                                const double NUM_PROBLEMS) {
    std::ifstream fstudents(filename);
    if(not fstudents.good()) {
        std::ofstream ofs;
        create_file(filename, ofs);
        ofs << "00\t00/0000000/1\tSample BOCA User\t\t\t\t\t\t\t0 (0)\n";
        ofs.close();
        return std::map<std::string, double>();
    }
    std::map<std::string, double> grades;
    std::string str, id;
    double grade;
    while(getline(fstudents, str)) {
        std::stringstream line(str);
        getline(line, str, '\t'); // read number
        getline(line, id, '\t'); // read registration id
        id = trim(id).substr(0,10); // ignore "/1" after id
        getline(line, str); // read line
        int it = str.size() - 1;
        while(str[it] != '(') it--;
        int size = -2;
        while(str[it] != '\t')
            size++, it--;
        auto begin = ++it;
        grade = stod(str.substr(begin,size));
        grades[trim(id)] = grade * 10.0 / NUM_PROBLEMS;
    }
    return grades;
}
