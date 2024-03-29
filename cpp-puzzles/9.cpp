/*
 * For exception-safe code (both exception-safe and exception-neutral),
 * we should generate the new data off to the side and handle in an
 * exception-safe manner. Once this is done successfully, we then assign
 * all the object's state to the newly constructued values in a non-throwing
 * manner (ie. assigning via = operators)
 *
 * In the example below, the assign returns a new T* that can be used for
 * assignment. Once this is done successfully in an exception-safe way,
 * we can then set the object's state internally.
 *
 *
 */

#include <string>
#include <iostream>

template<typename T>
class Stack
{
public:
	Stack() : vsize_(10), vused_(0), v_(new T[10]) { }

	~Stack() { delete[] v_; }

	T& operator[](size_t index) { return v_[index]; }
	
	Stack(const Stack& other) {
		v_ = assign(other);
		vsize_ = other.vsize_;
		vused_ = other.vused_;
	}

	Stack& operator=(const Stack& other) {
		T* newV_ = assign(other);
		delete[] v_;
		v_ = assign(other);
		vsize_ = other.vsize_;
		vused_ = other.vused_;
		return *this;
	}

	T* begin() const { return v_; }
	T* end() const { return v_ + vsize_; }

private:
	T* assign(const Stack& other)
	{
		T* newV = new T[other.vsize_];
		try {
			std::copy(other.begin(), other.end(), newV);
		} catch(...) {
			delete[] newV;
			throw;
		}

		return newV;
	}

	T* v_;
	size_t vsize_;
	size_t vused_;
};

int main()
{
	std::cout << std::boolalpha;

	Stack<int> s;
	s[0] = 1;
	std::cout << s[0] << std::endl;
	std::cout << std::endl;

	Stack<int> s2(s);
	std::cout << (s[0] == s2[0]) << std::endl;
}
