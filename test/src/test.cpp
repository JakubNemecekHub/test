#include <windows.h>

#include "../lib/ts/tester.hpp"
#include "_tests.hpp"


int main()
{

	SetConsoleOutputCP(CP_UTF8);
	ts::Tester tester;
	tester.set_verbosity(ts::VERBOSITY::verbose);
	tester.add(tests, "tests");
	tester.add(some, "some");
	tester.run();                 // Runs all test suites
	// tester.run("some");          // Runs only one test suites
	// tester.run("tests", "some");  // Runs two test suites
	return 0;

}
