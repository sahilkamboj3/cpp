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
 *
 * If you have a templated constructor with typename X inside a class with typename T,
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
 *
 */


