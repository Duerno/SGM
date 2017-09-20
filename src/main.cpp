#include "discipline.h"
#include "latexwriter.h"
#include <string>

const std::string ROOT_FOLDER("~/.sgm");
const std::string default_prefix("mentions");

void report_usage() {
    std::cerr << "USAGE:\n";
    std::cerr << "\tsgm init [PREFIX]\tInitialize SGM folder\n";
    std::cerr << "\tsgm build [PREFIX]\tGenerate missing files and folders\n";
    std::cerr << "\tsgm genpdf [PREFIX]\tGenerate PDF\n";
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
    system(("cp " + ROOT_FOLDER + "/static/pgf-pie.sty .latex/pgf-pie.sty").c_str());
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
    std::string pdfcmd("pdflatex --jobname=" + prefix + " .latex/main_" + prefix
                        + ".tex");
    system(pdfcmd.c_str());
    system(pdfcmd.c_str());
    system("rm *.log *.aux");
    // system(("evince " + prefix + ".pdf &").c_str());
}

int main(int argc, char *argv[]) {
    if(argc == 1 || argc > 3) {
        report_usage();
    } else {
        std::string option(argv[1]);
        if(option == "init")
            argc == 2 ? init() : init(argv[2]);
        else if(option == "build")
            argc == 2 ? build() : build(argv[2]);
        else if(option == "genpdf")
            argc == 2 ? genpdf() : genpdf(argv[2]);
        else
            report_usage();
    }
    return 0;
}
