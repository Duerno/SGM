#include "latexwriter.h"

static std::string bold(std::string text) {
    return "\\sffamily{\\textbf{\\textsc{" + text + "}}}";
}
static std::string print_grade(double grade, const unsigned int precision = 1) {
    if(grade == -1)
        return "-";
    std::stringstream ss;
    ss << std::setprecision(precision) << std::fixed;
    ss << grade;
    std::string result;
    ss >> result;
    return result;
}

LatexWriter::LatexWriter() {
}

void LatexWriter::generate_main(std::string prefix) {
    std::string filename(".latex/main_" + prefix + ".tex");
    std::ofstream main_tex(filename, std::fstream::app);
    main_tex << "\n\\input{.latex/header_" << prefix << "}\n\n";
    main_tex << "\\begin{huge}\\begin{center}\n";
    main_tex << "\t\\textbf{\\textsc{Menções Finais}}\n";
    main_tex << "\\end{center}\\end{huge}\n\n";
    main_tex << "\\input{.latex/grades_" << prefix << "}\n";
    main_tex << "\\input{.latex/charts_" << prefix << "}\n\n";
    main_tex << "\\end{document}\n";
    main_tex.close();
}
void LatexWriter::generate_latex(Discipline discipline) {
    LatexWriter::generate_header(discipline);
    LatexWriter::generate_grades(discipline);
    LatexWriter::generate_charts(discipline);
}
void LatexWriter::generate_header(Discipline discipline) {
    std::ofstream header(".latex/header_" + discipline.prefix + ".tex");
    header << "\\begin{center}\n\\begin{tabular}[h]{llll}\n\\hline\n";
    header << bold("Curso:") << " & " << discipline.course << " &\n";
    header << bold("Semestre/Ano:") << " & " << discipline.semester << " \\\\\n";
    header << bold("Disciplina:") << " & " << discipline.name << " &\n";
    header << bold("Código:") << " & " << discipline.code << " \\\\\n";
    header << bold("Carga Horária:") << " & " << discipline.workload << " &\n";
    header << bold("Créditos:") << " & " << discipline.credits << " \\\\\n";
    header << bold("Professor:") << " & " << discipline.professor << " &\n";
    header << bold("Turma:") << " & " << discipline.classname << " \\\\\n";
    header << "\\hline\n\\end{tabular}\n\\end{center}\n";
    header.close();
}
std::string LatexWriter::print_statistics(Discipline discipline,
                        std::map<std::pair<std::string,int>, double> statistic,
                        std::string type = "double") {
    std::stringstream result;
    for(auto evset : discipline.evaluationsets) {
        if(evset.show_in_pdf == "default") {
            for(uint i = 0; i < evset.max_score.size(); i++) {
                result << " & ";
                auto it = statistic.find(std::make_pair(evset.name, i));
                if(it != statistic.end()) {
                    if(type == "int")
                        result << print_grade(it->second, 0);
                    else
                        result << print_grade(it->second, 1);
                } else
                    result << "-";
            }
        } else if(evset.show_in_pdf == "percentage") {
            result << " & ";
            auto it = statistic.find(std::make_pair(evset.name, 0));
            if(it != statistic.end()) {
                result << print_grade(it->second * 10.0, 1);
                result << "\\%";
            } else
                result << "-";
        } else if(evset.show_in_pdf == "average") {
            result << " & ";
            auto it = statistic.find(std::make_pair(evset.name, 0));
            if(it != statistic.end())
                result << print_grade(it->second, 2);
            else
                result << "-";
        } else {
            if(discipline.show_others == "true" &&
                                            discipline.has_hidden_grades()) {
                result << " & ";
                auto it = statistic.find(others_id);
                if(it != statistic.end())
                    result << print_grade(it->second, 2);
                else
                    result << "-";
            }
        }
    }
    result << " & ";
    auto it = statistic.find(final_id);
    if(it != statistic.end())
        result << print_grade(it->second, 2);
    else
        result << "-";
    if(discipline.show_mention == "true")
        result << " & -";
    result << " \\\\\n";

    return result.str();
}
void LatexWriter::generate_grades(Discipline discipline) {
    std::ofstream grades(".latex/grades_" + discipline.prefix + ".tex");
    uint num_columns = 2; // Registration ID + Final Scores
    if(discipline.show_others == "true")
        num_columns++;
    if(discipline.show_mention == "true")
        num_columns++;

    for(auto evset : discipline.evaluationsets) {
        if(evset.show_in_pdf == "default")
            num_columns += evset.max_score.size();
        else if(evset.show_in_pdf == "percentage" ||
                evset.show_in_pdf == "average")
            num_columns++;
        else // evset.show_in_pdf == "none"
            continue;
    }

    grades << "\\begin{longtable}{" << std::string(num_columns, 'c') << "}\n";
    grades << "\\toprule\n";
    // first line of the table header
    grades << "\\multirow{2}{*}{\\textbf{Matrícula}}";
    for(auto evset : discipline.evaluationsets) {
        if(evset.show_in_pdf == "default") {
            grades <<" & \\multicolumn{" << evset.max_score.size();
            grades << "}{c}{\\textbf{" << evset.name << "}}";
        } else if(evset.show_in_pdf == "percentage")
            grades << " & \\textbf{" << evset.name << "}";
        else if(evset.show_in_pdf == "average")
            grades << " & \\textbf{" << evset.name << "}";
        else // evset.show_in_pdf == "none"
            continue;
    }
    if(discipline.show_others == "true" && discipline.has_hidden_grades())
        grades << " & \\textbf{Notas}";
    grades << " & \\textbf{Nota}";
    if(discipline.show_mention == "true")
        grades << " & \\textbf{Menção}";
    grades << " \\\\\n";

    // second line of the table header
    grades << ""; // skip registration ID
    for(auto evset : discipline.evaluationsets) {
        if(evset.show_in_pdf == "default") {
            for(uint i = 0; i < evset.max_score.size(); i++)
                grades << " & \\textbf{$" << evset.prefix << "_" << i+1 << "$}";
        } else if(evset.show_in_pdf == "percentage")
            grades << " & \\textbf{(\\%)}";
        else if(evset.show_in_pdf == "average")
            grades << " & \\textbf{Média}";
        else // evset.show_in_pdf == "none"
            continue;
    }
    if(discipline.show_others == "true" && discipline.has_hidden_grades())
        grades << " & \\textbf{Extras}";
    grades << " & \\textbf{Final}";
    if(discipline.show_mention == "true")
        grades << " & \\textbf{Final}";
    grades << " \\\\\n";
    grades << "\\toprule\n";

    // table content
    uint student_counter = 0;
    Discipline::sort_mode = "id";
    sort(discipline.students.begin(), discipline.students.end());
    for(auto student : discipline.students) {
        student_counter++;
        if(student_counter % 2)
            grades << "\\rowcolor[gray]{.9}\n";
        grades << student.registration_id;
        for(auto evset : discipline.evaluationsets) {
            if(evset.show_in_pdf == "default") {
                for(uint i = 0; i < evset.max_score.size(); i++) {
                    int subs_idx;
                    subs_idx = evset.subs_idx(student.evaluation_grades[evset.name],
                                            student.substitutives[evset.name]);
                    double grade;
                    if(subs_idx == i) {
                        grade = student.substitutives[evset.name];
                        grades << " & \\textbf{" << print_grade(grade) << "}";
                    } else {
                        grade = student.evaluation_grades[evset.name][i];
                        grades << " & " << print_grade(grade);
                    }
                }
            } else if(evset.show_in_pdf == "percentage") {
                double grade = evset.average(student.evaluation_grades[evset.name],
                                            student.substitutives[evset.name]);
                grades << " & " << print_grade(10.0 * grade) << "\\%";
            } else if(evset.show_in_pdf == "average") {
                double grade = evset.average(student.evaluation_grades[evset.name],
                                            student.substitutives[evset.name]);
                grades << " & " << print_grade(grade);
            } else // evset.show_in_pdf == "none"
                continue;

        }
        if(discipline.show_others == "true" && discipline.has_hidden_grades())
            grades << " & " << print_grade(discipline.hidden_grades(student), 2);
        grades << " & " << print_grade(discipline.final_score(student), 2);
        if(discipline.show_mention == "true")
            grades << " & " << discipline.final_mention(student);
        grades << " \\\\\n";
    }

    // statistics
    if(discipline.show_statistics == "true") {
        std::map<std::pair<std::string, int>, double> evpresents, evavg;
        std::map<std::pair<std::string, int>, double> evmax, evmin;

        double grade;
        std::pair<std::string,int> evaluation_id;
        for(auto student : discipline.students) {
            for(auto evset : discipline.evaluationsets) {
                if(evset.show_in_pdf == "default") {
                    int subs_idx = evset.subs_idx(
                                        student.evaluation_grades[evset.name],
                                        student.substitutives[evset.name]);

                    for(uint i = 0; i < evset.max_score.size(); i++) {
                        if(student.evaluation_grades[evset.name][i] == -1 &&
                            subs_idx != i)
                            continue;

                        grade = student.evaluation_grades[evset.name][i];
                        evaluation_id = std::make_pair(evset.name, i);
                        if(evpresents.find(evaluation_id) == evpresents.end()) {
                            evpresents[evaluation_id] = 1;
                            evavg[evaluation_id] = grade;
                            evmax[evaluation_id] = grade;
                            evmin[evaluation_id] = grade;
                        } else {
                            evpresents[evaluation_id]++;
                            evavg[evaluation_id] += grade;
                            evmax[evaluation_id] = std::max(evmax[evaluation_id], grade);
                            evmin[evaluation_id] = std::min(evmin[evaluation_id], grade);
                        }
                    }
                } else {
                    grade = evset.average(student.evaluation_grades[evset.name],
                                            student.substitutives[evset.name]);
                    if (evset.show_in_pdf == "percentage" ||
                            evset.show_in_pdf == "average") {
                        evaluation_id = std::make_pair(evset.name, 0);
                    } else {
                        evaluation_id = others_id;
                    }

                    if(evavg.find(evaluation_id) == evavg.end()) {
                        evavg[evaluation_id] = grade;
                        evmax[evaluation_id] = grade;
                        evmin[evaluation_id] = grade;
                    } else {
                        evavg[evaluation_id] += grade;
                        evmax[evaluation_id] = std::max(evmax[evaluation_id], grade);
                        evmin[evaluation_id] = std::min(evmin[evaluation_id], grade);
                    }
                }
            }
            grade = discipline.final_score(student);
            if(evavg.find(final_id) == evavg.end()) {
                evavg[final_id] = grade;
                evmax[final_id] = grade;
                evmin[final_id] = grade;
            } else {
                evavg[final_id] += grade;
                evmax[final_id] = std::max(evmax[final_id], grade);
                evmin[final_id] = std::min(evmin[final_id], grade);
            }
        }
        for(auto evset : discipline.evaluationsets) {
            if(evset.show_in_pdf == "default") {
                for(uint i = 0; i < evset.max_score.size(); i++) {
                    evaluation_id = std::make_pair(evset.name, i);
                    if(evpresents.find(evaluation_id) != evpresents.end())
                            evavg[evaluation_id] /= evpresents[evaluation_id];
                }
            } else {
                if (evset.show_in_pdf == "percentage" ||
                        evset.show_in_pdf == "average") {
                    evaluation_id = std::make_pair(evset.name, 0);
                } else {
                    evaluation_id = others_id;
                }
                evavg[evaluation_id] /= discipline.students.size();
            }
        }
        evavg[final_id] /= discipline.students.size();

        grades << "\\toprule\n";
        grades << "\\textbf{Estatísticas} & \\multicolumn{" << num_columns - 1;
        grades << "}{c}{} \\\\\n";
        grades << "\\toprule\n";
        grades << "\\rowcolor[gray]{.9}\n";
        grades << "\\multicolumn{1}{l}{Presentes}";
        grades << print_statistics(discipline, evpresents, "int");
        grades << "\\multicolumn{1}{l}{Média}";
        grades << print_statistics(discipline, evavg);
        grades << "\\rowcolor[gray]{.9}\n";
        grades << "\\multicolumn{1}{l}{Maior Nota}";
        grades << print_statistics(discipline, evmax);
        grades << "\\multicolumn{1}{l}{Menor Nota}";
        grades << print_statistics(discipline, evmin);
    }

    grades << "\\bottomrule\n";
    grades << "\\end{longtable}\n";
    grades.close();
}
void LatexWriter::generate_charts(Discipline discipline) {
    std::ofstream charts(".latex/charts_" + discipline.prefix + ".tex");
    if(discipline.show_charts == "false") {
        charts.close();
        return;
    }

    std::map<std::string, int> mentions;
    for(auto student : discipline.students) {
        if(mentions.find(discipline.final_mention(student)) == mentions.end()) {
            mentions[discipline.final_mention(student)] = 1;
        } else {
            mentions[discipline.final_mention(student)]++;
        }
    }
    charts << "\\centering\n";
    charts << "\\begin{tikzpicture}\n";
    charts << "\\pie[text=legend, radius=2, explode=0.1]{\n";
    for(auto mention : mentions) {
        charts << "\t" << mention.second * 100.0 / discipline.students.size();
        charts << "/" << mention.first;
        if(mention != *mentions.rbegin()) {
            charts << ",";
        }
        charts << "\n";
    }
    charts << "}\n";
    charts << "\\end{tikzpicture}\n";
    charts.close();
}
