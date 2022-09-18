//
// Created by mecha on 09.09.2022.
//

#pragma once

#include <filesystem>
#include <fstream>

namespace blueprint {

    struct Variable {
        const char* type;
        const char* name;
        const char* defaultValue = nullptr;

        Variable(const char* type, const char* name)
        : type(type), name(name) {}
        Variable(const char* type, const char* name, const char* defaultValue)
        : type(type), name(name), defaultValue(defaultValue) {}
    };

    enum class PassBy {
        VALUE, CONST_VALUE,
        REFERENCE, CONST_REFERENCE
    };

    struct FunctionParam {
        Variable var;
        PassBy passBy;
    };

    enum FunctionType {
        STATIC, MEMBER
    };

    struct Function {
        FunctionType functionType;
        const char* returnType;
        const char* name;
        std::vector<FunctionParam> params;
    };

    class Struct final {

    public:

        Struct(const char* filepath, const std::string& name)
        : name(name) {
            init(filepath);
        }

        Struct(const char* filepath, const std::string& name, const std::vector<std::string>& parents)
        : name(name), parents(parents) {
            init(filepath);
        }

        ~Struct();

    public:
        void addPragmaOnce();
        void addMainFunction();
        void addInclude(const std::string& include);
        void addIncludes(const std::vector<std::string>& includes);
        void release();
        // call begin() end() functions after addIncludes!
        void begin();
        void addMember(const Variable& variable);
        void addFunction(const Function& function);
        void end();

    private:
        void init(const char* filepath);

    private:
        std::string name;
        std::vector<std::string> parents;
        std::ofstream file;
    };

}