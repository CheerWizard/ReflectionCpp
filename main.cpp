#include <iostream>
#include "reflection.h"
#include <Windows.h>
#include <thread>
#include "CodeGenerator.h"

struct B {
    int number;
};

R_STRUCT_EXT(A, B,
    int i;
    std::string key;
    int value;
    std::vector<A> children;
    float dt = 0;

    R_BEGIN(A)
        R_PROPERTY(i)
        R_PROPERTY(key)
        R_PROPERTY(value)
        R_PROPERTY(number)
        R_PROPERTY(children)
    R_END
)

struct RootNode {
    int mainValue;
};

R_STRUCT_EXT(Node, RootNode,
    std::string key;
    int value = 0;
    std::vector<Node> children;
    float dt = 0;

    Node() : RootNode() {}
    Node(const std::string& key, int value) : RootNode() {
        this->key = key;
        this->value = value;
    }

    R_BEGIN(Node) // begin reflection
        // reflected members
        R_PROPERTY(key)
        R_PROPERTY(value)
        R_PROPERTY(mainValue)
        R_PROPERTY(children)
    R_END // end reflection
)

struct Sample {
    static reflect::Struct typeDescriptor;
};

bool isKeyPressed(int keycode) {
    return GetKeyState(keycode) & 0x8000;
}

using namespace std::chrono;

int main() {
    // Create an object of type Node
    Node node { "apple", 3 };
    node.children.emplace_back("wine", 7);
    node.children.emplace_back("milk", 11);

    if(reflect::typeDescriptor<A>::value) {
        std::puts("structure has member variable");
    } else {
        std::puts("structure does not have member variable");
    }

    blueprint::Struct example("example.cpp", "Example");

    example.addIncludes({ "<vector>", "<string>", "<cstdint>" });
    // to make file as a root exe, call addMainFunction()
    example.addMainFunction();

    example.begin();

    example.addMember({ "int", "index", "0" });
    example.addMember({ "float", "x", "0" });
    example.addMember({ "float", "y", "0" });
    example.addMember({ "float", "z", "0" });

    example.addFunction({ blueprint::FunctionType::STATIC, "void", "print" });
    example.addFunction(
            {
                blueprint::FunctionType::MEMBER, "int", "calculate", {
                    { {"float", "x"}, blueprint::PassBy::VALUE },
                    { {"float", "y"}, blueprint::PassBy::VALUE },
                    { {"float", "z"}, blueprint::PassBy::VALUE },
                }
            }
    );

    example.end();

    example.release();

    system("g++ -o example example.cpp");
//    system("./example");

//    int i = 0;
//    while (!isKeyPressed(VK_ESCAPE)) {
//        reflect::TypeResolver<Node>::get()->dump(&node);
//        if (i++ == 10) {
//            i = 0;
//            std::this_thread::sleep_for(1s);
//        }
//    }

    return 0;
}
