#include <iostream>
#include <exception>
#include <string>
#include <sstream>

class Foo
{
public:
	Foo() : x(0) { std::cout << "default..." << std::endl; }
	Foo(int) : x(1) { std::cout << "int..." << std::endl; }
	Foo(std::string) : x(2) { std::cout << "string..." << std::endl; }

	bool operator==(const Foo& other) const { return x == other.x; }
	bool operator!=(const Foo& other) const { return !operator==(other); }

	friend std::ostream& operator<<(std::ostream& os, const Foo& foo)
	{
		os << "Foo" << foo.x;
		return os;
	}

private:
	int x;
};

template<typename T, size_t Size>
class FixedVector
{
public:
	typedef T* iterator;
	typedef const T* const_iterator;

	FixedVector() {
		index_ = 0;
	}

	FixedVector(int sizeDefaulted)
	{
		// require number of elements to be defaulted in vector to be less than Size
		static_assert(sizeDefaulted <= Size);
		for (size_t i = 0; i < sizeDefaulted; i++)
		{
			v_[i] = T();
		}
		index_ = sizeDefaulted;
	}

	FixedVector(const FixedVector& other) {
		memcpy(&v_, &(other.v_), sizeof(v_));
		index_ = other.index_;
	}

	FixedVector(FixedVector&& other)
	{
		memcpy(&v_, &(other.v_), sizeof(v_));
		index_ = other.index_;

		memset(&(other.v_), 0, sizeof(other.v_));
		other.index_ = 0;
	}

	FixedVector(std::initializer_list<T> list)
	{
		static_assert(list.size() <= Size);

		for (size_t i = 0; i < list.size(); i++)
		{
			v_[i] = std::move(list[i]);
		}
	}

	template<typename... Args>
	bool emplace_back(Args&&... args)
	{
		// vector size if full so cannot push any elements
		if (index_ == Size) { return false; }
		
		v_[index_++] = T(std::forward<Args>(args)...);
		return true;
	}

	T pop_back()
	{
		if (index_ > 0)
		{
			return std::move(v_[--index_]);
		}
		return T();
	}

	T& operator[](size_t index) {
		if (index >= Size) {
			throw std::runtime_error("index >= Size in operator[] request");
		}
		return v_[index];
	}

	const T& operator[](size_t index) const {
		static_assert(index < Size);
		return v_[index];
	}

	bool operator==(const FixedVector& other) const
	{
		if (index_ != other.index_) return false;

		for (size_t i = 0; i < index_; i++)
		{
			if (v_[i] != other.v_[i])
			{
				return false;
			}
		}
		return true;
	}

	iterator begin() { return v_; }
	iterator end() { return v_ + Size; }
	const_iterator begin() const { return v_; }
	const_iterator end() const { return v_ + Size; }

	size_t size() { return index_; }
	size_t capacity() { return Size; }

private:
	T v_[Size];
	size_t index_;
};

FixedVector<int, 5> make()
{
	return FixedVector<int, 5>();
}

int main()
{
	constexpr int SZ = 3;
	FixedVector<Foo, SZ> fv;
	FixedVector<Foo, SZ> fv3;
	fv.emplace_back("hi");

	FixedVector<Foo, SZ> fv2(fv);
	std::cout << std::boolalpha;
	std::cout << (fv == fv2) << std::endl;
	std::cout << (fv3 == fv2) << std::endl;

	std::cout << std::endl;

	std::cout << fv.size() << std::endl;
	FixedVector<Foo, SZ> fv4(std::move(fv));
	std::cout << fv.size() << std::endl;

	std::cout << std::endl;
	std::cout << "Using strings..." << std::endl;

	FixedVector<std::string, SZ> fvs;
	for (int i = 0; i < SZ; i++)
	{
		fvs.emplace_back("hi");
		std::cout << fvs[i] << std::endl;
	}
	std::cout << std::endl;
	
	if (!fvs.emplace_back("5"))
	{
		std::cout << "failed successfully" << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < SZ + 3; i++)
	{
		std::cout << fvs.pop_back() << std::endl;
	}
}

