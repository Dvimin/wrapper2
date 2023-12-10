#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <unordered_map>

class Wrapper {
private:
    using Function = std::function<int(int, int)>;

    Function m_function;
    std::unordered_map<std::string, int*> m_args;

public:
    Wrapper(int* arg1, int* arg2): m_args{{"arg1", arg1}, {"arg2", arg2}} {}

    void setFunction(Function function) {
        m_function = function;
    }

    int execute() {
        int arg1 = *(m_args["arg1"]);
        int arg2 = *(m_args["arg2"]);
        return m_function(arg1, arg2);
    }

    int* getArgument(const std::string& argName) {
        return m_args[argName];
    }
};

class Engine {
private:
    std::unordered_map<std::string, Wrapper*> m_commands;

public:
    void registerCommand(Wrapper* wrapper, const std::string& commandName) {
        m_commands[commandName] = wrapper;
    }

    int execute(const std::string& commandName, const std::unordered_map<std::string, int>& args) {
        Wrapper* wrapper = m_commands[commandName];
        for (const auto& arg : args) {
            *(wrapper->getArgument(arg.first)) = arg.second;
        }
        return wrapper->execute();
    }
};

class Subject {
public:
    int f3(int arg1, int arg2) {
        return arg1 + arg2;
    }
};

int main() {
    Subject subj;
    int arg1 = 0;
    int arg2 = 0;

    Wrapper wrappwer(&arg1, &arg2);
    wrappwer.setFunction(std::bind(&Subject::f3, &subj, std::placeholders::_1, std::placeholders::_2));

    Engine engine;
    engine.registerCommand(&wrappwer, "command1");

    std::unordered_map<std::string, int> args{{"arg1", 4}, {"arg2", 5}};
    std::cout << engine.execute("command1", args) << std::endl;

    return 0;
}


/*/
int main() {
    std::cout << "Hello, World!" << std::endl;

//    Wrapper wrappwer(&subj, &subject::f3, {{"arg1", 0}, {"arg2", 0}});
//    Engine engine;
//    engine.register_command(&wrapper, "command1");
//    std::cout << engine.execte("command1", {{"arg1", 4}, {"arg2", 5}}) << std::endl;
//

    return 0;
}
/*/


/*/

 Wrapper wrappwer(&subj, &subject::f3, {{"arg1", 0}, {"arg2", 0}});
 Engine engine;
 engine.register_command(&wrapper, "command1");
 std::cout << engine.execte("command1", {{"arg1", 4}, {"arg2", 5}}) << std::endl;

 /*/