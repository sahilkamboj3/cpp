/* Notes/Explanations:
 *
 * Default constructor: default constructs the elements of T inside v_
 *
 * Copy constructor: Takes in a vector of different type O and size and copies
 * the values into v_. This may be an issue because types T and O could be
 * different, which could cause copying errors.
 * Even if this was somehow made possible, the iterators could be invalid
 * because the type has changed. Not only that, the bytes being copied
 * into v_ from the other FixedVector could be greater than the v_
 * in the current FixedVector, which would mean setting bytes not owned by
 * the class, which would also lead to undefined behavior. Hence, this technically
 * isn't even a copy constructor/assignment because type of the incoming object
 * is different.
 *
 * Copy assignment: Same thing goes for here because the implementation 
 * is the same.
 * * If you have a templated constructor with typename X inside a class with typename T,
 * the typename X can never be the same as typename T. Even if the type of the
 * templated constructor matches the one of the copy constructor, the rules of 
 * C++ state the copy construtor is going to be used.
 * Because a templated constructor isn't actually a copy constructor, the
 * compiler still (within the rules) default generates a copy constructor.
 *
 *
 * A homogenous container is when in which all the elements stored are of
 * the same type.
 *
 *
 *
 * Standard library approach:
 * - when working with modifier (ie. insert, push_back), sometime susing
 * iterators can help provide more flexibility for the user. If the user wants
 * to do something like vec1.insert(vec2.begin(), vec2.end()), the iterators allow
 * this sort of programming whereas not having vectors (using elements of T) 
 * means you can only insert single elements or parameter packs of the element.
 *
 * - you can't templatize the operator=() function because the rule is that that
 * function can only take 1 value
 * 	- instead, you can provide your own function (ie. assign) that can take in
 * 		iterators and do the assignment operation for the user via iterators
 *
 *
 * std::copy - takes in 2 inputs iterators (begin and end) and copies those
 * values into the destination starting at the destination iterator passed into
 * the function. This works if given type Src and Dest where Src is the type of
 * the values pointed by the input iterators and Dest is the type of the object
 * pointed to by the output iterator, Dest has a constructor taking in the
 * type Src.
 * ie.
 * class Foo { Foo(std::string) { std::cout << "default..." << std::endl; }
 * 
 * std::vector<Foo> foos;
 * std::vector<std::string> vs{"hi", "bye"};
 * std::copy(vs.begin(), vs.end(), foos.begin()); // called Foo(std::string) on all
 * strings in the 'vs' vector
 *
 *
 * TODO: ask what happens when copying std::vector<Derived*> to std::vector<Base*>
 * Does it dynamic_cast, reinterpret_cast, etc? The other way around (ie. 
 * std::vector<Base*> to std::vector<Derived*> gives errors if no Base(Derived*)
 * is given
 * 
 * Reference Items 8-17 for writing exception-safe code (ie. swapping safely, etc)
 *
 */

#include <string>
#include <iostream>

// standard library style approach
template<typename T, size_t Size>
class FixedVector
{
public:
	typedef T* iterator;
	typedef const T* const_iterator;

	FixedVector() : v_(new T[Size]) { std::cout << "Default..." << std::endl; }

	// this gives strong exception safety because internally, we are copying
	// and if the copying operation throws, we revert the v_ (by deleting it
	// because we also heap allocated it in the constructor), and then continue
	// to throw and propogate the error upwards
	FixedVector(const FixedVector<T, Size>& other)
	: v_(new T[Size])
	{
		std::cout << "Copy..." << std::endl;
		try {
			std::copy(other.begin(), other.end(), begin());
		} catch (...) {
			delete[] v_;
			throw;
		}
	}

	template<typename OtherT, size_t OtherSize>
	FixedVector(const FixedVector<OtherT, OtherSize>& other)
	{
		std::cout << "Templated..." << std::endl;
		constexpr size_t MIN = std::min(Size, OtherSize);
		try {
			v_ = new T[Size];
			std::copy(
				other.begin(),
				other.begin() + MIN,
				begin()
			);
		} catch (...) {
			delete[] v_;
			throw;
		}
	}

	~FixedVector() { delete[] v_; }

	iterator begin() { return v_; }
	iterator end() { return v_ + Size; }
	const_iterator begin() const { return v_; }
	const_iterator end() const { return v_ + Size; }

private:
	T* v_;
};

int main()
{
	FixedVector<std::string, 5> fv;
	FixedVector<std::string, 6> fv6;
	FixedVector<std::string, 5> fv2(fv);
	FixedVector<std::string, 5> fv3(fv6);
	FixedVector<std::string, 6> fv4(fv);
}

