#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

class WrapperBase {
public:
    virtual int execute(const std::vector<int> &) = 0;
};

template<class Class, class... Args>
class WrapperTemp : public WrapperBase {
    friend class Wrapper;

private:
    Class *obj;

    int (Class::* method)(Args...);

    WrapperTemp(Class *obj, int (Class::* method)(Args...)) : obj(obj), method(method) {}

    template<size_t... I>
    int callHelper(const std::vector<int> &args, std::index_sequence<I...>) {
        return (obj->*method)(args[I]...);
    }

    int execute(const std::vector<int> &args) override {
        return callHelper(args, std::make_index_sequence<sizeof...(Args)>{});
    }
};


class Wrapper {
public:
    template<class Class, class... Args>
    Wrapper(Class *obj, int (Class::* method)(Args...), const std::vector<std::pair<std::string, int>> &args)
            : pImpl(std::make_unique<WrapperTemp<Class, Args... >>(WrapperTemp<Class, Args...>(obj, method))) {

        argsOrder.reserve(args.size());
        defaultVals.reserve(args.size());
        for (auto &arg: args) {
            argsOrder.push_back(arg.first);
            defaultVals.insert(arg);
        }
    }

    int execute(const std::unordered_map<std::string, int> &args) {
        std::vector<int> final_args;
        for (auto &arg_name: argsOrder) {
            auto i = args.find(arg_name);
            final_args.push_back(i != args.end() ? i->second : defaultVals.find(arg_name)->second);
        }
        return pImpl->execute(final_args);
    }

private:
    std::unique_ptr<WrapperBase> pImpl;
    std::vector<std::string> argsOrder;
    std::unordered_map<std::string, int> defaultVals;
};