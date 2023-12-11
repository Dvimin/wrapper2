#include <iostream>
#include <string>
#include <unordered_map>
#include "Engine.h"

class Subject {
public:
    int sum(int x, int y) {
        return x + y;
    }
};

int main() {
    Subject subj;
    int arg1 = 0;
    int arg2 = 0;

    Wrapper wrapper1(&subj, &Subject::sum, {{"arg1", 0}, {"arg2", 0}});

    Engine engine;

    engine.register_command("command1", &wrapper1);

    std::unordered_map<std::string, int> args{{"arg1", 4},
                                              {"arg2", 5}};
    std::cout << engine.execute("command1", args) << std::endl;

    return 0;
}

/*/

 Wrapper wrappwer(&subj, &subject::f3, {{"arg1", 0}, {"arg2", 0}});
 Engine engine;
 engine.register_command(&wrapper, "command1");
 std::cout << engine.execte("command1", {{"arg1", 4}, {"arg2", 5}}) << std::endl;


 /*/