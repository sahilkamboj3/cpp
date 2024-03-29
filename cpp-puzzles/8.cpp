/* Notes:
 * The goal is to make the Stack class exception-safe and exception-neutral.
 * This means the Stack object should always be in a correct and consistent
 * state no matter the exceptions thrown in the operations.
 *
 * exception-safe: means the operation works properly in the presence of exceptions,
 * so if an exception is thrown, the state is consistent and correct
 * exception-neutral: means exceptions are propagated to the user without causing
 * integrity errors with the Stack
 *
 * When calling the new operator on either allocation an array of objects T or just
 * a single object T on the heap, if the memory allocation itself or any of the
 * constructors called throw errors, the previously successfully constructed
 * objects are destroyed and the memory is automatically released via the
 * delete[] operator.
 * 
 * std::expected - std::expected holds either an object of the successful type
 * or the object of the error'ed type in a std::unexpected if there's an error and
 * you can query to see if there is an error type or not
 */


template<typename T>
class Stack
{
public:
	Stack()
	: vsize_(10),
	: vused_(new T[vsize_]),
	: v_(nullptr)
	{
		// they could be exceptions thrown from 2 places here
		// 1. memory allocating on the heap
		// 2. default constructing on the elements T in the v_ array
		// Don't need to do the below
		/*
		try {
			v_ = new T[vsize_];
		} catch(...) {
			delete[] v_;
			throw;
		}
		*/

		// can simply do this
		v_ = new T[vsize_]; // the reason is because if the new[] operation throws,
		// then nothing is going to happen, bad_alloc will be thrown. If the constructor
		// of T throws, the operator new[]() will automatically destroy the objects
		// that were constructed and release the allocated memory.
		// 
		// This is exception-neutral because we are actually catching and handling
		// any errors, they are being propagated to the user. Also, it is exception-safe
		// because if new operator throws, don't actually have memory. If the T
		// construction fails, the objects created before that are destructed and
		// the delete[]() operation is called
	}

	~Stack() {
		delete[] v_; // this can't throw so is both exception-safe and exeception-neutral
	}

private:
	T* v_;
	size_t vsize_; // # full capacity of Stack
	size_t vused_; // # slots used
};

int main()
{
}
