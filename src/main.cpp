#include "discipline.h"
#include "latexwriter.h"
#include <string>
#include <set>

const std::string ROOT_FOLDER("~/.sgm");
const std::string default_prefix("mentions");

void report_usage() {
    std::cerr << "USAGE:\n";
    std::cerr << "\tsgm init [PREFIX]\tInitialize SGM folder\n";
    std::cerr << "\tsgm build [PREFIX]\tGenerate missing files and folders\n";
    std::cerr << "\tsgm genpdf [PREFIX]\tGenerate PDF\n";
    std::cerr << "\tsgm report EV_SET EV_NUMBER [PREFIX]\tPresents the ";
    std::cerr << "students sorted by their evaluation grade\n";
    exit(1);
}
void init(std::string prefix = default_prefix) {
    system(("cp " + ROOT_FOLDER + "/static/mentions.ini " + prefix +
            ".ini").c_str());
    system(("cp " + ROOT_FOLDER + "/static/students.csv .").c_str());
    system("mkdir -p .latex");
    system(("cp " + ROOT_FOLDER + "/static/logo_fga.pdf .latex/.").c_str());
    system(("cp " + ROOT_FOLDER + "/static/main.tex .latex/main_" + prefix +
            ".tex").c_str());
    system(("cp " + ROOT_FOLDER + "/static/pgf-pie.sty " +
            ".latex/pgf-pie.sty").c_str());
    LatexWriter::generate_main(prefix);
}
void build(std::string prefix = default_prefix) {
    Discipline discipline(prefix);
    LatexWriter::generate_latex(discipline);
    // discipline.info();
    // discipline.view_students();
}
void genpdf(std::string prefix = default_prefix) {
    Discipline discipline(prefix);
    LatexWriter::generate_latex(discipline);
    std::string pdfcmd("pdflatex --jobname=" + prefix + " .latex/main_" +
                        prefix + ".tex");
    system(pdfcmd.c_str());
    system(pdfcmd.c_str());
    system("rm *.log *.aux");
    // system(("evince " + prefix + ".pdf &").c_str());
}
void report(std::string ev_name, std::string ev_num,
            std::string prefix = default_prefix) {
    Discipline discipline(prefix);

    int ev_number = stoi(ev_num);
    auto evsets = discipline.students[0].evaluation_grades;
    if(evsets.find(ev_name) == evsets.end()) {
        std::cerr << "EV_NAME error: evaluation set not found.\n";
        exit(1);
    }
    if(ev_number <= 0) {
        std::cerr << "EV_NUMBER error: value must be a positive integer.\n";
        exit(1);
    }
    if(evsets[ev_name].size() < ev_number) {
        std::cerr << "EV_NUMBER error: given set has only ";
        std::cerr << evsets[ev_name].size() << " evaluations, but the given ";
        std::cerr << "EV_NUMBER is " << ev_number << ".\n";
        exit(1);
    }

    std::set<std::pair<double, std::string>> students;
    for(auto student : discipline.students) {
        double grade = student.evaluation_grades[ev_name][ev_number-1];
        std::string name = student.name;
        students.insert(std::make_pair(-grade, name));
    }

    for(auto student : students) {
        std::cout << -student.first << "\t" << student.second << "\n";
    }
}

int main(int argc, char *argv[]) {
    if(argc == 1 || argc > 5 || (argc > 3 && std::string(argv[1]) != "report"))
        report_usage();
    else {
        std::string option(argv[1]);
        if(option == "init")
            argc == 2 ? init() : init(argv[2]);
        else if(option == "build")
            argc == 2 ? build() : build(argv[2]);
        else if(option == "genpdf")
            argc == 2 ? genpdf() : genpdf(argv[2]);
        else if(option == "report" && argc > 3)
            argc == 4 ? report(argv[2], argv[3]) : report(argv[2], argv[3], argv[4]);
        else
            report_usage();
    }
    return 0;
}
