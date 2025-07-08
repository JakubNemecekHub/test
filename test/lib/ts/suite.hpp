#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <concepts>

/*
    Concept of a streamable type
*/
template<typename T>
concept Streamable = requires(std::ostream& os, T value)
{
    { os << value } -> std::same_as<std::ostream&>;
};
template <typename T>
std::string to_string_if_streamable(const T& value) {
    if constexpr (Streamable<T>) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    } else {
        return "(non-streamable type)";
    }
}


namespace ts
{

// ANSI escape sequences for colours.
constexpr std::string RED   { "\033[31m" };
constexpr std::string GREEN { "\033[32m" };
constexpr std::string CYAN  { "\033[36m" };
constexpr std::string RESET { "\033[0m"  };

enum class VERBOSITY
{
    mute,
    normal,
    verbose
};

class Context
{
private:
    std::vector<std::string> failures_;
    std::vector<std::string> successes_;
public:
    inline void log_failure(const std::string& msg) { failures_.push_back(msg);  }
    inline void log_success(const std::string& msg) { successes_.push_back(msg); }
    inline bool no_failure() const { return failures_.empty(); }
    inline void print_failures() const { for ( const auto& msg : failures_ ) std::cout << msg << '\n'; }
    inline void print_successes() const { for ( const auto& msg : successes_ ) std::cout << msg << '\n'; }
};

class Suite
{
public:

    using test_function_t = std::function<void(Context&, VERBOSITY)>;

private:

    std::string name_;
    std::vector<std::pair<std::string, test_function_t>> tests_;

public:

    Suite(const std::string& name = "Test Suite")   // TO DO check why const &
        : name_{name} {}

    void register_test(const std::string& message, test_function_t test)
    {
        tests_.emplace_back(message, test);
    }

    bool run(VERBOSITY verbosity)
    {
        int passed { 0 };
        int failed { 0 };
        std::cout << "Running tests from " << CYAN << name_ << RESET <<'\n';
        for ( const auto& [message, test] : tests_ )
        {
            Context context;
            try
            {
                test(context, verbosity);
            }
            catch(const std::exception& e)
            {
                context.log_failure(std::string("Unexpected exception: ") + e.what());
            }
            catch(...)
            {
                context.log_failure("Unknown exception thrown");
            }

            if ( context.no_failure() )
            {
                ++passed;
                if ( verbosity != VERBOSITY::mute ) std::cout << GREEN << "[PASSED] " << RESET << message << '\n';
                if ( verbosity == VERBOSITY::verbose ) context.print_successes();
            }
            else
            {
                if ( verbosity != VERBOSITY::mute )
                {
                    std::cout << RED << "[FAILED] " << RESET << message << '\n';
                    context.print_failures();
                }
                ++failed;
            }

        }
        if ( verbosity != VERBOSITY::mute )
        {
            std::cout << "Suite Summary: " << passed << " passed, " << failed << " failed.\n";
            std::cout << "==================================\n\n";
        }
        return failed == 0;
    }

};

#define CONCAT_INNER(a, b) a ## b
#define CONCAT(a, b) CONCAT_INNER(a, b)
// Fo the indirection here see https://stackoverflow.com/questions/13301428/token-pasting-and-line/13301627#13301627
#define NAME(base) CONCAT(base, __LINE__)

#define TEST(suite, message)                                   \
    void NAME(TestFunction_)(ts::Context& context, ts::VERBOSITY verbosity);            \
    struct NAME(TestRegistrar_)                                \
    {                                                          \
        NAME(TestRegistrar_)()                                 \
        {                                                      \
            suite.register_test(message, NAME(TestFunction_)); \
        }                                                      \
    } NAME(test_registrar_);                                   \
    void NAME(TestFunction_)(ts::Context& context, ts::VERBOSITY verbosity)

#define ASSERT_TRUE(condition)                                            \
if ( !(condition) )                                                       \
{                                                                         \
    if ( verbosity != ts::VERBOSITY::mute )                                   \
    {                                                                     \
        std::ostringstream oss;                                           \
        oss << "\n\t" << ts::RED << "[ASSERT TRUE failed] " << ts::RESET  \
            << #condition;                                                \
        context.log_failure(oss.str());                                   \
    }                                                                     \
}                                                                         \
else if ( verbosity == ts::VERBOSITY::verbose )                               \
{                                                                         \
        std::ostringstream oss;                                           \
        oss << "\n\t" << ts::GREEN << "[ASSERT TRUE] " << ts::RESET       \
            << #condition;                                                \
        context.log_success(oss.str());                                   \
}

#define ASSERT_FALSE(condition)                                        \
if ( (condition) )                                                     \
{                                                                      \
    std::ostringstream oss;                                            \
    oss << "\n\t" << ts::RED << "[ASSERT FALSE failed] " << ts::RESET  \
    << #condition;                                                     \
    context.log_failure(oss.str());                                            \
}

#define ASSERT_EQ(lhs, rhs)                                            \
if ( !(lhs == rhs) )                                                   \
{                                                                      \
    std::ostringstream oss;                                            \
    oss << "\n\t" << ts::RED << "[ASSERT EQUAL failed] " << ts::RESET  \
        << #lhs << " == " << #rhs                                      \
        << "\n\t" << "[VALUES] " << to_string_if_streamable(lhs)       \
                     << ", and " << to_string_if_streamable(rhs);      \
    context.log_failure(oss.str());                                            \
}

#define ASSERT_NEQ(lhs, rhs)                                               \
if ( !(lhs != rhs) )                                                       \
{                                                                          \
    std::ostringstream oss;                                                \
    oss << "\n\t" << ts::RED << "[ASSERT NOT EQUAL failed] " << ts::RESET  \
        << #lhs << " != " << #rhs                                          \
        << "\n\t" << "[VALUES] " << to_string_if_streamable(lhs)           \
                     << ", and " << to_string_if_streamable(rhs);          \
    context.log_failure(oss.str());                                                \
}

#define ASSERT_THROWS(expression)                                      \
try                                                                    \
{                                                                      \
    expression;                                                        \
    std::ostringstream oss;                                            \
    oss << "\n\t" << ts::RED << "[ASSERT THROWS failed]" << ts::RESET  \
        << #expression << " didn't throw";                             \
    context.log_failure(oss.str());                                            \
}                                                                      \
catch (...)                                                            \
{                                                                      \
    /* Exception was thrown as expected. */                            \
}                                                                      \

}  // namespace ts
