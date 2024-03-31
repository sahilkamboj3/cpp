#include <exception>

template<typename T>
class Stack
{
public:
	Stack() = default;
	~Stack() = default;

	Stack(const Stack& other) = default;
	Stack& operator=(const Stack& other) = default;

	size_t Count() const { return vused_; } // can't throw

	void Push(const T& el) {
		if (vused_ == vsize_) {
			// create new array in resize, handling throwing in there
			T* resizedArray = resize(vsize_ * 2);

			// assign new state in class
			delete[] v_; // can't throw
			vsize_ *= 2;
			v_ = resizedArray;
		}

		// if errors, object won't actually be constructed and error is
		// propagated up
		v_[vused_++] = el;
	}

  // if empty, throws exception
	/*
	 * The problem with the current implementation has to do with how this pop
	 * function has two jobs, so it makes it harder to make it exception-safe.
	 * Namely, in the following code,
	 * 	std::string s = stack.Pop();
	 * Assuming no (N)RVO, the temporary returned by this Pop function needs
	 * to be copied into the object s. Although this is outside of the Pop function,
	 * this could lead to an exception and by this time, the Pop function has modified
	 * the stack state even though an exception was thrown.
	 * Hence, technically, the logic inside the Pop function is exception-safe,
	 * but the issue lies outside of the Pop function. The issue is because a
	 * modifying function like Pop is returning by value. We can't return by 
	 * reference (T&) because returning a ref to something saying it doesn't
	 * exist anymore is bad design and could lead to bugs later on.
	 * 
	 * We could take an input to a reference of object T and set that to the 
	 * popped value, but again, that is the same issue as returning a reference
	 * to something considered gone.
	 *
	 * The best way to handle this is to separate the two functions in the Pop
	 * function currently, popping the object and returning the object. This is why
	 * STL pop/pop_back functions are set up the way they are.
	 *
	 * Have a pop_back() and top() function where the top() function returns the
	 * top object without modifying stack state. Have a pop_back() function that
	 * decrements the vused_, which can't throw.
	 *
	 * void Pop() {
	 * 	if (vused_ == 0) throw std::runtime_error("Stack is empty");
	 * 	vused_--;
	 * }
	 *
	 * T& Top() {
	 * 	if (vused_ == 0) throw std::runtime_error("Stack is empty");
	 * 	return v_[vused_ - 1];
	 * }
	 *
	 * const T& Top() const {
	 * 	if (vused_ == 0) throw std::runtime_error("Stack is empty");
	 * 	return v_[vused_ - 1];
	 * }
	 *
	 * Hence, now there are two functions, one of get the top elements and one
	 * to pop it and both don't modify stack state if an exception is thrown.
	 * This makes the stack exception-safe. They are also exception-neutral since
	 * any errors are propagated up.
	 * This is why STL pop functionality is set up like this, you don't have to
	 * sacrifice on exception safety.
	 *
	 */
	T Pop() {
		if (vused_ == 0)
		{
			throw std::runtime_error("Stack is empty");
		}

		// if throws, state not actually changed yet
		T res = v_[vused_ - 1];
		vused_--;
		return res;
	}

private:
	T* resize(size_t newSize)
	{
		T* newV = new T[newSize];
		try {
			std::copy(
				v_,
				v_ + vsize_,
				newV
			);
		} catch(...) {
			delete[] newV;
		}

		return newV;
	}

	T* v_;
	size_t vsize_;
	size_t vused_;
};
