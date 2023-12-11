#pragma once
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

template <class Class, class... Args>
class WrapperTemp : public WrapperBase {
public:
    WrapperTemp(Class *object, int (Class::* method)(Args...)) : object(object), method(method) {}

    int execute(const std::vector<int> &args) const override {
        return callFunc(object, method, args, std::index_sequence_for<Args...>());
    }

private:
    template <size_t... Is>
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
    template <class Class, class... Args>
    Wrapper(Class *object, int (Class::* method)(Args...), const std::vector<std::pair<std::string, int>> &args)
            : pImpl(std::make_unique<WrapperTemp<Class, Args...>>(object, method)),
              argumentOrder(args.begin(), args.end()) {}

    int execute(const std::unordered_map<std::string, int> &args) {
        std::vector<int> finalArgs;
        for (const auto &arg : argumentOrder) {
            auto it = args.find(arg.first);
            finalArgs.push_back(it != args.end() ? it->second : arg.second);
        }
        return pImpl->execute(finalArgs);
    }

private:
    WrapperPtr pImpl;
    std::vector<std::pair<std::string, int>> argumentOrder;
};

