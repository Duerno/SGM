#ifndef STUDENT_H_
#define STUDENT_H_

#include <map>
#include <vector>
#include <string>

class Student {
public:
    Student(void);
    std::string name, registration_id, uri_name;
    std::map<std::string, double> substitutives;
    std::map<std::string, std::vector<double>> evaluation_grades;
};

extern bool sort_by_id;
bool operator<(Student const &a, Student const &b);

#endif // STUDENT_H_
