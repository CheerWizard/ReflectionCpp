//
// Created by mecha on 14.09.2022.
//

#include "CodeGenerator.h"

namespace blueprint {

    void Struct::init(const char *filepath) {
        file.open(filepath);
    }

    void Struct::addPragmaOnce() {
        file << "#pragma once\n\n";
    }

    void Struct::addMainFunction() {
        file << "\nint main() {\n"
                "    return 0;\n"
                "}\n";
    }

    Struct::~Struct() {
        release();
    }

    void Struct::addInclude(const std::string &include) {
        file << "#include " << include <<  "\n";
    }

    void Struct::addIncludes(const std::vector<std::string> &includes) {
        for (const auto& include : includes) addInclude(include);
    }

    void Struct::begin() {
        file << "\n" << "struct " << name << " ";
        if (!parents.empty()) {
            file << ": ";
            for (int i = 0 ; i < parents.size() ; i++) {
                file << parents[i];
                if (i < parents.size() - 1) {
                    file << ", ";
                }
            }
        }
        file << "{" << "\n";
    }

    void Struct::end() {
        file << "};" << "\n";
    }

    void Struct::addMember(const Variable &variable) {
        file << variable.type << " " << variable.name;
        if (variable.defaultValue) {
            file << " = " << variable.defaultValue;
        }
        file << ";\n";
    }

    void Struct::addFunction(const Function &function) {
        file << "\n";
        if (function.functionType == FunctionType::STATIC) {
            file << "static ";
        }
        file << function.returnType << " " << function.name;
        file << "(";
        for (int i = 0; i < function.params.size(); i++) {
            const auto& param = function.params[i];
            const auto& var = param.var;

            switch (param.passBy) {
                case PassBy::VALUE:
                    file << var.type << " " << var.name;
                    break;

                case PassBy::CONST_VALUE:
                    file << "const " << var.type << " " << var.name;
                    break;

                case PassBy::REFERENCE:
                    file << var.type << "& " << var.name;
                    break;

                case PassBy::CONST_REFERENCE:
                    file << "const " << var.type << "& " << var.name;
                    break;
            }

            if (var.defaultValue) {
                file << " = " << var.defaultValue;
            }

            if (i < function.params.size() - 1) {
                file << ", ";
            }
        }
        file << ") {\n}\n";
    }

    void Struct::release() {
        if (file.is_open()) {
            file.close();
        }
    }
}