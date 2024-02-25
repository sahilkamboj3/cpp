/*
In a sense, compiler writes/generates code based on the rules/blueprint you write.
Templates are evaluated at compile time, it only gets created when we call it
with the specific types

typename T -> template parameter named as T (typename is synonymous with class)
								the type is deduced at compile time
int Size   -> template parameter specifying a parameter of type int that is
								set by the user and the associated class is created at compile
								time

If type can be deduced by either the argument or the return value, the compiler
can deduce what the template parameter type is. These are templates for functions/
classes that are materialized when we actually use. Functions are generated and
linked when we actually use a templated function/class.

Stack allocated variables need to have known size at compile time and templates
are deduced at compile time, hence can create stack-allocated objects with
templates since those are created at compile time.

This is sort of meta-programming, instead of coding what the code does at runtime,
we are coding what the compiler does at comile time.
*/

#include <iostream>
#include <string>

/*
// Need to redefine function multiple times for different input data types
// This, this has a lot of repetitve code
// These three can created from the templated function based on our usage
// 	of the Print function.
void Print(int value)
{
	std:cout << value << std::endl;
}

void Print(std::string value)
{
	std:cout << value << std::endl;
}

void Print(float value)
{
	std:cout << value << std::endl;
}
*/

template<typename T>
void Print(T value)
{
	std::cout << value << std::endl;
}

// int Size here defines
template<int Size, typename T>
class Array
{
public:
	size_t GetSize() const { return Size; }

private:
	T m_Array[Size];
};

int main()
{
	Print<int>(5);
	Print<float>(5.5f);
	Print<std::string>("hello world");

	Array<10, std::string> arr;
	std::cout << "Array size:" << arr.GetSize() << std::endl;
}
