#include <vector>
#include <string>

/*
 *
 * std::istringstream - Used to read in data from strings as if we were reading
 * from std::cin
 * ie.
 * std::string s = "0 1 2";
 * std::istringstream iss(s);
 * int a, b, c;
 * iss >> a >> b >> c;
 *
 *
 * std::ostringstream - Used to write data to strings as if we were writing to
 * std::cout
 * ie.
 * std::ostringstream oss;
 * oss << 5 << "Hello world" << 'a';
 * std::string s = oss.str();
 *
 * std::istream_iterator - used to read continuous items in a basic_istream as the data type
 * T that the user defines. The default constructed std::istream_iterator is an
 * end-of-stream iterator, meaning when a std::istream_iterator hits the end,
 * it becomes a end-of-stream iterator.
 * 
 * These read from std::basic_istream objects. The reading is done via the 
 * operator>> operation.
 * 
 * ie. 
 * std::istringstream iss("0.1 0.2 0.3");
 * auto it = std::find_if(
 * 		std::istream_iterator<double>(iss),
 * 		std::istream_iterator<double>(),
 * 		[](const double& d) { return d == 0.2; } );
 *
 * std::cout << *it << std::endl; // *it = 0.2
 *
 * 
 * 
 * std::ostream_iterator - used to write continuous items in a basic_ostream object
 * as the data type T that the user defines. Incrementing the std::ostream_iterator
 * is a no-op, so incrementing the iterator doesn't actually move the iterator.
 * This is why when we call std::partial_sum on the ostream_iterator, the accumulator
 * compounds on itself (ie. only one std::basic_ostream is getting updated) as opposed
 * to something like a vector of ints where we are inserting into the vector, 
 * or we are really looping over the vector of pre-allocated size, dereferencing
 * the iterators, and assigning them values.
 * The writing is done every time the iterator is assigned to. One can
 * specify a delimit string to user in between each of the write operations.
 * You need to specify where you are going to write to. The writing is done via
 * the operator<< operation.
 *
 * ie.
 * std::istringstream iss("0 1 2");
 * std::partial_sum(
 * 	std::istream_iterator<int>(iss),
 * 	std::istream_iterator<int>(),
 *  std::ostream_iterator<int>(std::cout, ", ") );
 * 
 * std::cout << std::endl;
 *
 *
 *
 * std::partial_sum - takes the input iterator range and keeps track of an
 * accumulator. For each element, the accumulator is updated and inserted
 * into the destination iterator.
 *
 * ie.
 * std::vector<int> v(3); // vector of size 3 w/ default constructed values (ie. 0's)
 * std::istringstream iss("0 1 2");
 * std::partial_sum(
 *	std::istream_iterator<int>(iss),
 *	std::istream_iterator<int>(),
 *	v.begin(),
 *  std::multiplies<int>() // accumulator fn can be overridden, ie. multiply accumulator
 *	// std::ostream_iterator<int>(std::cout, ", ")
 * );
 *
 *
 * std::multiplies<T>(const T& lhs, const T& rhs) - used to call the operator*
 * on the objects of type T.
 *
 */

#include <sstream>
#include <iostream>
#include <numeric>

int main()
{
	std::istringstream iss("1 2 3");

	std::partial_sum(
		std::istream_iterator<int>(iss),
		std::istream_iterator<int>(),
		std::ostream_iterator<int>(std::cout, ", ")
	);
	std::cout << std::endl;

	std::ostringstream oss;
	oss << 5 << " hello world" << 'a';
	std::cout << oss.str() << std::endl;
}

/*
 * Puzzle solution:
 *
 * 1. 'last' might be the end iterator, so we are not able to dereference
 * an end of vector iterator
 *
 * 2. --e.end() - might be invalid because the vector could be empty
 *
 * 3. Likely, the second copy needs to replace 'last' with 'e.end()'
 * 
 * 4. 'first' might come after 'last', there is no guarantee in the ordering
 * of the objects in the vector
 *
 * 5. The 'first' and 'last' iterators could be invalid at the time of the
 * last copy operation because the 'insert' operation might cause the
 * vector to resize, reallocating new memory. This means that the old
 * iterators would become invalidated, leading to core dumps.
 *
 */

