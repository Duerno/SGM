#include "evaluationset.h"

EvaluationSet::EvaluationSet(std:: string name) {
    this->name = name;
}

double EvaluationSet::average(std::vector<double> grades, double subs_grade) {
    if(grades.size() != max_score.size()) {
        std::cerr << "Evaluation set error: student grades and evaluation set ";
        std::cerr << "sizes do not match.\n";
        exit(1);
    }

    double avg = 0, total_weight = 0;
    for(uint i = 0; i < max_score.size(); i++) {
        avg += weight[i] * std::max(0.0, grades[i]);
        total_weight += weight[i];
    }
    avg /= total_weight;

    int subs_to_idx = subs_idx(grades, subs_grade);
    if(subs_to_idx != -1) {
        avg += (weight[subs_to_idx] * subs_grade - weight[subs_to_idx] *
                std::max(0.0, grades[subs_to_idx])) / total_weight;
    }

    return avg;
}

int EvaluationSet::subs_idx(std::vector<double> grades, double subs_grade) {
    if(subs_max_score == -1 || subs_grade == -1)
        return -1;

    double avg = 0, total_weight = 0;
    for(uint i = 0; i < max_score.size(); i++) {
        avg += weight[i] * std::max(0.0, grades[i]);
        total_weight += weight[i];
    }
    avg /= total_weight;

    int idx = 0;
    double max = 0;
    for(uint i = 0; i < max_score.size(); i++) {
        double maxi = std::max(max, avg + (weight[i] * subs_grade -
                        weight[i] * std::max(0.0, grades[i])) / total_weight);
        if(maxi > max){
            max = maxi;
            idx = i;
        }
    }

    return idx;
}
