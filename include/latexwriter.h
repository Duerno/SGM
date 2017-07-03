#ifndef LATEX_WRITER_H_
#define LATEX_WRITER_H_

#include "student.h"
#include "discipline.h"
#include "evaluationset.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>

extern const std::string ROOT_FOLDER;

class LatexWriter {
private:
    static std::string print_statistics(Discipline discipline,
                        std::map<std::pair<std::string,int>, double> statistic,
                        std::string type);
public:
    LatexWriter(void);
    static void generate_main(std::string prefix);
    static void generate_latex(Discipline discipline);
    static void generate_header(Discipline discipline);
    static void generate_grades(Discipline discipline);
};

#endif // LATEX_WRITER_H_
