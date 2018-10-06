#ifndef READER_H_
#define READER_H_

#include "student.h"
#include "discipline.h"
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

class Reader {
public:
    Reader(void);
    static std::vector<Student> read_students(std::string filename);
    static std::map<std::string, std::string> read_students_uri(std::string
                                                                    filename);

    // The readers below read a specific file format and return a map of 'id' to
    // 'grade'. The grades are always returned in a range of (0-10)
    static std::map<std::string, double> read_grades(std::string filename,
                        const double MAX_SCORE, std::vector<Student> students);
    static std::map<std::string, double> read_uri_grades(std::string filename,
                                                         std::string uri_file);
    static std::map<std::string, double> read_boca_grades(std::string filename,
                                                    const double NUM_PROBLEMS);
    static std::map<std::string, double> read_300_grades(std::string filename,
                                                const double MAX_SCORE,
                                                std::vector<Student> students);
    static std::map<std::string, double> read_boca_300_grades(
                                                std::string filename,
                                                const double NUM_PROBLEMS,
                                                std::vector<Student> students);
};

#endif // READER_H_
