#include "discipline.h"

Discipline::Discipline(std::string prefix) {
    this->prefix = prefix;
    ConfigFile cf(prefix + ".ini");
    parse_discipline_section(cf);
    parse_evaluation_sets(cf);
    parse_students();
    parse_grades();
}

void Discipline::info() {
    std::cout << "[DISCIPLINE]" << std::endl;
    std::cout << "Course: " << course << std::endl;
    std::cout << "Semester: " << semester << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Code: " << code << std::endl;
    std::cout << "Workload: " << workload << std::endl;
    std::cout << "Credits: " << credits << std::endl;

    for(auto evset : evaluationsets) {
        std::cout << std::endl << "[" << evset.name << "]" << std::endl;
        std::cout << "Prefix: " << evset.prefix << std::endl;
        std::cout << "Reader Type: " << evset.reader_type << std::endl;
        std::cout << "Show in PDF: " << evset.show_in_pdf << std::endl;
        std::cout << "Perc. of Score: " << evset.perc_of_score;
        std::cout << " %" << std::endl;
        std::cout << "Evaluations (max_score, weight):" << std::endl;
        for(uint i = 0; i < evset.max_score.size(); i++) {
            std::cout << evset.prefix << i+1 << ": " << evset.max_score[i];
            std::cout << ", " << evset.weight[i] << std::endl;
        }
    }
}
void Discipline::view_students() {
    std::cout << "[STUDENTS]" << std::endl;
    for(auto s : students) {
        std::cout << s.name << "," << s.registration_id << "," << s.uri_id;
        std::cout << "," << s.uri_name << std::endl;
    }
}

bool Discipline::has_hidden_grades(void) {
    for(auto evset : evaluationsets)
        if(evset.show_in_pdf == "none")
            return true;
    return false;
}
double Discipline::hidden_grades(Student student) {
    double total = 0;
    for(auto evset : evaluationsets) {
        if(evset.show_in_pdf == "none") {
            total += evset.perc_of_score / 100.0 *
                        evset.average(student.evaluation_grades[evset.name],
                                        student.substitutives[evset.name]);
        }
    }
    return total;
}
double Discipline::final_score(Student student) {
    double score = 0;
    for(auto evset : evaluationsets) {
        double evset_avg = evset.average(student.evaluation_grades[evset.name],
                                            student.substitutives[evset.name]);
        score += evset.perc_of_score / 100.0 * evset_avg;
    }
    return score;
}
std::string Discipline::final_mention(Student student) {
    double score = final_score(student);
    if(score < 0.1)
        return "SR";
    else if(score < 3.0)
        return "II";
    else if(score < 5.0)
        return "MI";
    else if(score < 7.0)
        return "MM";
    else if(score < 9.0)
        return "MS";
    else
        return "SS";
}

void Discipline::parse_discipline_section(ConfigFile cf) {
    if(not cf.has_section(discipline_section)) {
        std::cerr << "Invalid configuration file: section ";
        std::cerr << discipline_section << " not found\n";
        exit(1);
    }
    this->course = cf.value(discipline_section, "course", "Sample Course");
    this->semester = cf.value(discipline_section, "semester", "2017/1");
    this->name = cf.value(discipline_section, "name", "Sample Discipline");
    this->code = cf.value(discipline_section, "code", "123456");
    this->workload = cf.value(discipline_section, "workload", "100 hours");
    this->credits = cf.value(discipline_section, "credits", "10 credits");
    this->professor = cf.value(discipline_section, "professor", "Sample Prof.");
    this->classname = cf.value(discipline_section, "classname", "AA");
    this->students_file = cf.value(discipline_section, "students_file", "students.csv");
    this->uri_file = cf.value(discipline_section, "uri_file", "");
    this->show_mention = cf.value(discipline_section, "show_mention", "true");
    this->show_others = cf.value(discipline_section, "show_others", "true");
    this->show_statistics = cf.value(discipline_section, "show_statistics", "true");
}
void Discipline::parse_evaluation_sets(ConfigFile cf) {
    for(auto section : cf.content) {
        if(section.first == discipline_section)
            continue;
        EvaluationSet evset(section.first);
        evset.prefix = cf.value(section.first, "prefix", section.first.substr(0,2));
        evset.reader_type = cf.value(section.first, "reader_type", "default");
        evset.show_in_pdf = cf.value(section.first, "show_in_pdf", "default");
        evset.perc_of_score = cf.value_to_double(section.first, "perc_of_score",
                                                    100.0);
        evset.subs_max_score = cf.value_to_double(section.first,
                                                    "subs_max_score", -1.0);
        evset.max_score = cf.value_to_vector(section.first, "max_score", 1);
        evset.weight = cf.value_to_vector(section.first, "weight",
                                            evset.max_score.size());
        if(evset.max_score.size() != evset.weight.size()) {
            std::cerr << "Invalid configuration file: 'max_score' and ";
            std::cerr << "'weight' in section [" << evset.name << "] have ";
            std::cerr << "different sizes\n";
            exit(1);
        }
        evaluationsets.push_back(evset);
    }
    if(!evaluationsets.size()) {
        std::cerr << "Invalid configuration file: no evaluation sets found\n";
        exit(1);
    }
}
void Discipline::parse_students() {
    students = Reader::read_students(students_file);
    if(!students.size()) {
        std::cerr << "Invalid students' file: no students were found\n";
        exit(1);
    }
    for(auto &student : students) {
        for(auto evset : evaluationsets) {
            student.evaluation_grades[evset.name] =
                                std::vector<double>(evset.max_score.size(), -1);
            student.substitutives[evset.name] = -1.0;
        }
    }

    // map<str,str> = 'uri_id' to 'uri_name'
    if(uri_file != "") {
        auto uri_students = Reader::read_students_uri(uri_file);
        for(auto &student : students) {
            if(uri_students.find(student.uri_id) != uri_students.end()) {
                student.uri_name = uri_students[student.uri_id];
            }
        }
    }
}
void Discipline::parse_grades() {
    for(auto evset : evaluationsets) {
        for(uint i = 0; i < evset.max_score.size(); i++) {
            std::string filename(evset.name + "/" + evset.prefix + "_" +
                                    std::to_string(i+1) + ".csv");
            std::map<std::string, double> grades;
            if(evset.reader_type == "uri")
                grades = Reader::read_uri_grades(filename, uri_file);
            else if(evset.reader_type == "boca")
                grades = Reader::read_boca_grades(filename, evset.max_score[i]);
            else
                grades = Reader::read_grades(filename, evset.max_score[i],
                                                students);
            for(auto &student : students) {
                std::string id;
                if(evset.reader_type == "uri")
                    id = student.uri_name;
                else
                    id = student.registration_id;
                if(grades.find(id) != grades.end())
                    student.evaluation_grades[evset.name][i] = grades[id];
            }
        }
        if(evset.subs_max_score != -1.0) {
            std::string filename(evset.name + "/" + evset.prefix + "_subs.csv");
            std::map<std::string, double> grades;
            if(evset.reader_type == "uri")
                grades = Reader::read_uri_grades(filename, uri_file);
            else if(evset.reader_type == "boca")
                grades = Reader::read_boca_grades(filename, evset.subs_max_score);
            else
                grades = Reader::read_grades(filename, evset.subs_max_score,
                                                students);
            for(auto &student : students) {
                std::string id;
                if(evset.reader_type == "uri")
                    id = student.uri_name;
                else
                    id = student.registration_id;
                if(grades.find(id) != grades.end())
                    student.substitutives[evset.name] = grades[id];
            }
        }
    }
}

std::string Discipline::sort_mode = "id";
bool operator<(Student const &a, Student const &b) {
   if(Discipline::sort_mode == "id") {
       // sort by registration id
       return a.registration_id < b.registration_id;
   } else if(Discipline::sort_mode == "name") {
       // sort by name
       return a.name < b.name;
   }
}
