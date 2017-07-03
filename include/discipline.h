#ifndef DISCIPLINE_H_
#define DISCIPLINE_H_

#include "configfile.h"
#include "reader.h"
#include "student.h"
#include "evaluationset.h"

#include <vector>
#include <string>
#include <iostream>

class Discipline {
private:
    const std::string discipline_section = "DISCIPLINE";
    void parse_discipline_section(ConfigFile cf);
    void parse_evaluation_sets(ConfigFile cf);
    void parse_students();
    void parse_grades();

public:
    static std::string sort_mode; // "id", "name"
    std::string course, semester;
    std::string name, code;
    std::string workload, credits;
    std::string professor, classname;
    std::string students_file, uri_file;
    std::string show_mention, show_others, show_statistics;
    std::vector<Student> students;
    std::vector<EvaluationSet> evaluationsets;
    std::string prefix;
    Discipline(std::string prefix);
    void info(void);
    void view_students(void);
    bool has_hidden_grades(void);
    double hidden_grades(Student student);
    double final_score(Student student);
    std::string final_mention(Student student);
};

#endif // DISCIPLINE_H_
