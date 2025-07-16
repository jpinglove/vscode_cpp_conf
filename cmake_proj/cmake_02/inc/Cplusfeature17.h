#ifndef CPLUSFEATURE17_H
#define CPLUSFEATURE17_H

#include <iostream>
#include <string>
#include <variant>
#include <optional>
#include <any>

class Cplusfeature17 {
public:
    Cplusfeature17();
    ~Cplusfeature17();

    void runSamples();

private:
    // C++17 std::variant example
    void variantExample();

    // C++17 std::optional example
    void optionalExample();

    // C++17 std::any example
    void anyExample();
    // C++17 fold expressions example
    void foldExpressionsExample();
    // C++17 structured bindings example
    void structuredBindingsExample();

    // C++17 if constexpr example
    template <typename T>
    void ifConstexprExample(T value);
};

#endif // CPLUSFEATURE17_H
