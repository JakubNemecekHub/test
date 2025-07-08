#pragma once

#include <string>
#include <unordered_map>
#include <concepts>

#include "suite.hpp"

namespace ts
{

template <typename T>
concept StringConvertible = std::convertible_to<T, std::string>;

class Tester
{
private:

    std::string name_;
    std::unordered_map<std::string, Suite> suites_;
    VERBOSITY verbosity_ { VERBOSITY::normal };

public:

    Tester(std::string name = "Suites of tests.")
        : name_{name} {}

    inline void set_verbosity(VERBOSITY verbosity)
    {
        verbosity_ = verbosity;
    }

    void add(Suite suite, std::string name)
    {
        suites_.insert(std::make_pair(name, suite));
    }

    void run()
    {
        std::cout << name_ << "\n\n";
        for ( auto [name, suite] : suites_ ) suite.run(verbosity_);
    }

    template<StringConvertible... Args>
    void run(Args... args)
    {
        std::cout << name_ << "\n\n";
        for ( const auto & arg : {args...} )
            if ( auto it { suites_.find(arg) }; it != suites_.end() )
                it->second.run(verbosity_);
    }

};

}  // namespace ts
