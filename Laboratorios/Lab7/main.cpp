#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <atomic>
#include <map>
#include <iostream>
#include <fstream>
#include <regex>
#include "ParserHTML.h"

#define REG_EXPRESSION "(<[^!][^ >]*)"

void ocurrencias();

std::map<std::string, std::atomic_int> *etiquetas_HTML;

int main(int argc, char ** argv) {
    ParserHTML *parser = new ParserHTML();

    std::ifstream input(argv[1]);
    std::string line;

    etiquetas_HTML = new std::map<std::string, std::atomic_int>;

    while(std::getline(input, line)) {
        parser->leerLinea(line);
    }

    delete parser;
    ocurrencias();

    return 0;
}

void ocurrencias() {
    std::map<std::string, std::atomic_int>::iterator it;

    for(it = etiquetas_HTML->begin(); it != etiquetas_HTML->end(); ++it) {
        std::cout << "Key: " << it->first << "\t\tCantidad: " << it->second << std::endl;
    }
}

