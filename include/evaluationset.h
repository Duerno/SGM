#ifndef EVALUATION_SET_H_
#define EVALUATION_SET_H_

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class EvaluationSet {
public:
    std::string name;
    std::string prefix;
    std::string reader_type; // default, uri, boca
    std::string show_in_pdf; // default, percentage, average, none
    double perc_of_score;
    double subs_max_score;
    std::vector<double> max_score, weight;
    EvaluationSet(std::string name);
    double average(std::vector<double> grades, double subs_grade);
    int subs_idx(std::vector<double> grades, double subs_grade);
};

#endif // EVALUATION_SET_H_
