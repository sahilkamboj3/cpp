/*
Allows to move objects around. Used to be the case that the way to access an
object was only via copying (to take it from one place to another)

To solve this, we can just move the object from one place to another without
having to copy it over (useful especially if dealing with heap allocations)
*/

#include <iostream>
#include <utility>

class String
{
public:
	String() = default;

	String(const char* string)
	{
		m_Size = strlen(string);
		m_Data = new char[m_Size];
		memcpy(m_Data, string, m_Size);
		std::cout << this << " String created w/ m_Data " << m_Data << "!" << std::endl;
	}

	/* need to provide copy constructor for two reasons
	1. With move constructor, compiler reverts to move constructor since defined
	but invalid since ie. line 122 requires copy constructor. Compiler doesn't
	implicitly generate a copy constructor because move constructor is provided.
	2. Even without move constructor, implicit copy constructor allocates m_Data
	on the stack, so in destruction, m_Data/m_Size are torn down from the stack and
	then trying to call "delete m_Data" in the destructor doesn't work since it was
	on the stack. The copy constructor puts the m_Data on the heap to be able
	to use "delete m_Data".
	*/
	String(const String& string)
	{
		m_Size = string.m_Size;
		m_Data = new char[string.m_Size];
		memcpy(m_Data, string.m_Data, m_Size);
		std::cout << this << " String created w/ m_Data " << m_Data << "!" << std::endl;
	}

	// Performs a shallow copy of the object
	String(String&& other)
	{
		m_Size = other.m_Size;
		m_Data = other.m_Data;

		other.m_Data = nullptr;
		other.m_Size = 0;
		std::cout << this << " String moved w/ m_Data " << m_Data << "!" << std::endl;
	}

	~String()
	{
		delete m_Data;
		if (m_Data == nullptr)
		{
			std::cout << this << " was nullptr while destroying" << std::endl;
		} else
		{
			std::cout << this << " String destroyed" << std::endl;
		}
	}

	void Print()
	{
		if (m_Data == nullptr)
		{
			std::cout << this << " is null - nothing to Print" << std::endl;
			return;
		}

		for (uint32_t i = 0; i < m_Size; i++) {
			std::cout << m_Data[i];
		}
		std::cout << std::endl;
	}

private:
	char* m_Data;
	uint32_t m_Size;
};

class Entity
{
public:
	Entity(const String& string)
	: string_(string)
	{
		string_.Print();

		std::cout << &string << std::endl;
		std::cout << &string_ << std::endl;
	}

	Entity(String&& string)
	: string_(std::move(string))
	// : string_(string) // Uses copy constructor here because we aren't explicitly moving the data
	{
		string_.Print();

		std::cout << "string: " << &string << std::endl;
		std::cout << "string_: " << &string_ << std::endl;
	}

	~Entity()
	{
		std::cout << this << " Entity destroyed" << std::endl;
	}

private:
	String string_;
};

int main() {
	std::cout << "Construction started..." << std::endl;
	std::cout << std::endl;

	String string("hello world");
	Entity entity(std::move(string));
	// Entity entity(string);
	string.Print();

	std::cout << std::endl;
	std::cout << "Destruction started..." << std::endl;
}
