/*
Macro uses preprocessor to help automate writing some repetitive code.
Evaluates all the '#' prefixed preprocessor directives and then passes to
compiler to then do its compilation stuff. This helps determine which code
gets fed to the compiler, or a find-and-replace of pure text in other cases.
*/

#include <iostream>
#include <string>

// Don't include semicolon because during preprocessing stage, this is a pure
// text and replace so since 'WAIT;' has a semicolon, we don't need it when we
// need use the directive
#define WAIT std::cout << "WAIT DIRECTIVE" << std::endl
#define DEBUG 0

#define PRINT std::cout << "FOO MACRO PRINT" << std::endl;\
std::cout << "FOO MACRO PRINT 2" << std::endl

#define FUNC void func()\
{\
	std::cout << "hello from func()" << std::endl;\
}


#if DEBUG == 1
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

FUNC

int main()
{
	WAIT;
	LOG("LOG print");
	LOG(5);

	PRINT;
	func();
}
