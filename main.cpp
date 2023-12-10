#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <unordered_map>
#include <memory>

class WrapperBase {
public:
    virtual ~WrapperBase() = default;

    virtual int execute(const std::vector<int> &args) const = 0;
};

template<class Class, class... Args>
class WrapperTemp : public WrapperBase {
public:
    WrapperTemp(Class *object, int (Class::* method)(Args...)) : object(object), method(method) {}

    int execute(const std::vector<int> &args) const override {
        return callFunc(object, method, args, std::index_sequence_for<Args...>());
    }

private:
    template<size_t... Is>
    int callFunc(Class *object, int (Class::* method)(Args...), const std::vector<int> &args,
                 std::index_sequence<Is...>) const {
        return (object->*method)(args[Is]...);
    }

    Class *object;

    int (Class::* method)(Args...);
};

using WrapperPtr = std::unique_ptr<WrapperBase>;

class Wrapper {
public:
    template<class Class, class... Args>
    Wrapper(Class *object, int (Class::* method)(Args...), const std::vector<std::pair<std::string, int>> &args)
            : pImpl(std::make_unique<WrapperTemp<Class, Args...>>(object, method)),
              argumentOrder(args.begin(), args.end()) {}

private:
    WrapperPtr pImpl;
    std::vector<std::pair<std::string, int>> argumentOrder;
};

class Engine {
private:
    std::unordered_map<std::string, Wrapper *> commands;

public:
    void register_command(std::string name, Wrapper *wrapper) {
        commands.insert({name, wrapper});
    }

    int execute(std::string name, const std::unordered_map<std::string, int> &args);
};


class Subject {
public:
    int sum(int x, int y) {
        return x + y;
    }

    int dif(int x, int y) {
        return x - y;
    }
};

int main() {
    Subject subj;
    int arg1 = 0;
    int arg2 = 0;

    Wrapper wrapper1(&subj, &Subject::sum, {{"arg1", 0},
                                            {"arg2", 0}});
    Wrapper wrapper2(&subj, &Subject::dif, {{"arg1", 0},
                                            {"arg2", 0}});

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