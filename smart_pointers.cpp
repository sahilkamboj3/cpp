/*
Smart pointers handle object lifetime for you and handle object deletion
(and creation in some cases). They are wrappers around C++ pointers. The
memory is automatically freed for you.

Unique pointer: when pointer goes out of scope, it will call delete and get
destroyed. They have to be unique, you can't copy the unique pointer because
if you have two unique pointers and one dies, it would free the memory and
that second unique pointer would be pointing to memory that has already been
freed. It is unique in the sense only one pointer is pointing to the memory
so the lifetime of the memory is tied to the lifetime of that unique pointer only.

Shared pointer: uses reference counting to keep track of how many references
you have to something and when it reaches 0, you free the memory. Allows to
make copies of the pointers to have multiple pointers to the same piece
of memory. More overhead than unique pointer because of the reference count
state logic.

Weak pointer: very similar to shared_pointer, except it doesn't increase the
reference count. When you copy a shared pointer to a second shared pointer,
that would increase the reference count. When you assign a shared_pointer to a
weak_pointer, that doesn't increase the reference count. This is good for when
you want to look at a piece of memory, but you don't necessarily care if it's
valid or not. You want to store a reference to it and you can ask if it's
alive/valid. If it is, you can perform logic on it, else you don't have to.
Weak pointers can use memory if it's alive, but it doesn't keep the memory alive
like a shared pointer would.
*/

#include <iostream>
#include <string>
#include <memory>

struct Entity
{
	void Print() { std::cout << "hello from Entity" << std::endl; }
};

int main()
{
	/*
	Better than "std::unique_ptr<Entity> unique_entity(new Entity());" because
	if there is an error/exception thrown in the constructor, you risk having a
	dangling pointer that hasn't been initialized properly and your program
	either crashed or user didn't handle it properly. Using std::make_unique<>()
	gives better safety against that happening.
	*/
	std::cout << std::boolalpha;

	std::cout << "UNIQUE POINTERS:" << std::endl;
	std::unique_ptr<Entity> unique_entity = std::make_unique<Entity>();
	// cannot do "std::unique_ptr<Entity> unq_ent_2 = unique_entity;
	// copy constructor is explicitly deleted, can use move constructor
	unique_entity->Print();

	std::weak_ptr<Entity> weak_ent;
	{
		std::cout << std::endl;
		std::cout << "SHARED/WEAK POINTERS:" << std::endl;
		std::shared_ptr<Entity> outer_scope;
		{
			std::shared_ptr<Entity> inner_scope(new Entity());
			std::cout << "REF COUNT 1:" << inner_scope.use_count() << std::endl;
			outer_scope = inner_scope;
			weak_ent = outer_scope;
			std::cout << "REF COUNT 2 INNER_SCOPE:" << inner_scope.use_count() << std::endl;
			std::cout << "REF COUNT 2 OUTER_SCOPE:" << outer_scope.use_count() << std::endl;
			std::cout << "REF COUNT 2 WEAK_PTR:" << weak_ent.use_count() << std::endl;
		}
		std::cout << "REF COUNT 3 OUTER_SCOPE:" << outer_scope.use_count() << std::endl;
		std::cout << "REF COUNT 3 WEAK_PTR:" << weak_ent.use_count() << std::endl;
		std::cout << "REF COUNT 3 WEAK_PTR.EXPIRED():" << weak_ent.expired() << std::endl;
	}
	std::cout << "REF COUNT 4 WEAK_PTR:" << weak_ent.use_count() << std::endl;
	std::cout << "REF COUNT 4 WEAK_PTR.EXPIRED():" << weak_ent.expired() << std::endl;

	return EXIT_SUCCESS;
}
