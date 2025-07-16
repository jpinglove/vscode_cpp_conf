#include "Cplusfeature17.h"
#include <iostream>
#include <string>
#include <variant>
#include <optional>
#include <any>
#include <vector>
#include <map>

Cplusfeature17::Cplusfeature17() {
    std::cout << "[Cplusfeature17] Constructor called." << std::endl;
}

Cplusfeature17::~Cplusfeature17() {
    std::cout << "[Cplusfeature17] Destructor called." << std::endl;
}

void Cplusfeature17::runSamples() {
    std::cout << "\n--- Running C++17 Feature Samples ---" << std::endl;

    variantExample();
    optionalExample();
    anyExample();
    foldExpressionsExample();
    structuredBindingsExample();

    std::cout << "\n--- if constexpr Example ---" << std::endl;
    ifConstexprExample(10);
    ifConstexprExample("hello");
    ifConstexprExample(3.14);

    std::cout << "--- Finished C++17 Feature Samples ---\n" << std::endl;
}

void Cplusfeature17::variantExample() {
    std::cout << "\n--- std::variant Example ---" << std::endl;
    std::variant<int, float, std::string> v;

    v = 10;
    std::cout << "Variant holds int: " << std::get<int>(v) << std::endl;

    v = 3.14f;
    std::cout << "Variant holds float: " << std::get<float>(v) << std::endl;

    v = "hello variant";
    std::cout << "Variant holds string: " << std::get<std::string>(v) << std::endl;

    // Using std::visit
    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) { std::cout << "Visited int: " << arg << std::endl; }
        else if constexpr (std::is_same_v<T, float>) { std::cout << "Visited float: " << arg << std::endl; }
        else if constexpr (std::is_same_v<T, std::string>) { std::cout << "Visited string: " << arg << std::endl; }
    }, v);
}

void Cplusfeature17::optionalExample() {
    std::cout << "\n--- std::optional Example ---" << std::endl;
    std::optional<std::string> opt1; // Empty optional
    std::optional<std::string> opt2 = "Hello Optional"; // Optional with value

    if (opt1.has_value()) {
        std::cout << "opt1 has value: " << *opt1 << std::endl;
    } else {
        std::cout << "opt1 is empty." << std::endl;
    }

    if (opt2) { // Implicit conversion to bool
        std::cout << "opt2 has value: " << *opt2 << std::endl; // Dereference to get value
        std::cout << "opt2 value (using .value()): " << opt2.value() << std::endl;
    }

    // Using value_or
    std::cout << "opt1 value_or 'default': " << opt1.value_or("default") << std::endl;
    std::cout << "opt2 value_or 'default': " << opt2.value_or("default") << std::endl;
}

void Cplusfeature17::anyExample() {
    std::cout << "\n--- std::any Example ---" << std::endl;
    std::any a;

    std::cout << "a.has_value(): " << std::boolalpha << a.has_value() << std::endl;

    a = 10;
    std::cout << "a holds int: " << std::any_cast<int>(a) << std::endl;

    a = std::string("Hello Any");
    std::cout << "a holds string: " << std::any_cast<std::string>(a) << std::endl;

    try {
        std::any_cast<float>(a); // Throws std::bad_any_cast
    } catch (const std::bad_any_cast& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
}

// Helper function for fold expressions
template <typename... Args>
auto sum_all(Args... args) {
    return (args + ...); // Unary right fold
}

template <typename... Args>
void print_all(Args... args) {
    // Binary left fold with comma operator
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

void Cplusfeature17::foldExpressionsExample() {
    std::cout << "\n--- Fold Expressions Example ---" << std::endl;
    std::cout << "Sum of 1, 2, 3, 4, 5: " << sum_all(1, 2, 3, 4, 5) << std::endl;
    std::cout << "Sum of 1.5, 2.5, 3.0: " << sum_all(1.5, 2.5, 3.0) << std::endl;

    std::cout << "Printing values: ";
    print_all(1, "hello", 3.14, 'C');
}

void Cplusfeature17::structuredBindingsExample() {
    std::cout << "\n--- Structured Bindings Example ---" << std::endl;

    // Binding a pair
    std::pair<int, std::string> p = {1, "apple"};
    auto [id, name] = p;
    std::cout << "Pair: id=" << id << ", name=" << name << std::endl;

    // Binding a struct/class
    struct Point { int x; int y; };
    Point pt = {10, 20};
    auto [px, py] = pt;
    std::cout << "Point: x=" << px << ", y=" << py << std::endl;

    // Binding a map element
    std::map<int, std::string> myMap = {{1, "one"}, {2, "two"}};
    for (const auto& [key, value] : myMap) {
        std::cout << "Map element: Key=" << key << ", Value=" << value << std::endl;
    }
}

template <typename T>
void Cplusfeature17::ifConstexprExample(T value) {
    std::cout << "  Processing value: " << value << " (Type: " << typeid(T).name() << ")" << std::endl;
    if constexpr (std::is_integral_v<T>) {
        std::cout << "    It's an integral type. Doubling it: " << value * 2 << std::endl;
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "    It's a floating-point type. Squaring it: " << value * value << std::endl;
    } else if constexpr (std::is_convertible_v<T, std::string>) {
        std::cout << "    It's convertible to string. Appending '!': " << static_cast<std::string>(value) << "!" << std::endl;
    } else {
        std::cout << "    It's another type. No specific action." << std::endl;
    }
}


