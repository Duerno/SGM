#include "discipline.h"
#include "latexwriter.h"

int main (int argc, char *argv[]) {
    Discipline discipline(argc == 2 ? argv[1] : "configfile");
    // discipline.info();
    // discipline.view_students();
    LatexWriter::generate_latex(discipline);
    return 0;
}
