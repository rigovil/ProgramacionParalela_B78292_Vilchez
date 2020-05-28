#include "ParserHTML.h"

ParserHTML :: ParserHTML() {
}

ParserHTML :: ~ParserHTML() {

}

// std::map<std::string, std::atomic_int>
void ParserHTML :: leerLinea(std::string linea) {
    std::smatch matches;
    std::regex reg_exp (REG_EXPRESSION);
    std::string etiq;

    while(std::regex_search(linea, matches, reg_exp)) {
        etiq = matches.str(1) + '>';

        if(etiquetas_HTML->find(etiq) == etiquetas_HTML->end()) {
            etiquetas_HTML->emplace(etiq, 1);
        }
        
        else {
            ++etiquetas_HTML->at(etiq);
        }

        linea = matches.suffix().str();  // resetea el match
    }
}