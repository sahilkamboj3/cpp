#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <tuple>

/* 
 * BitsOfQ to get introduced to template metaprogramming.
*/

template<typename Key, typename Value>
struct Foo
{
    static constexpr int type = 0;

    int getI() { return 5; }
};

template<typename Value>
struct Foo<int, Value>
{
    static constexpr int type = 1;
};

template<>
struct Foo<int, int>
{
    static constexpr int type = 2;
};

template<typename T>
struct is_pointer
{
    constexpr static bool value = false;
};

template<typename T>
struct is_pointer<T*>
{
    constexpr static bool value = true;
};

template<typename T>
void print(T val)
{
    if constexpr (is_pointer<T>::value)
    {
        std::cout << *val << std::endl;
    }
    else
    {
        std::cout << val << std::endl;
    }
}

// implement recursively
bool myContains(std::string search, std::vector<std::string> strings, size_t start_from = 0)
{
    if (strings[start_from] == search) return true;
    if (start_from == strings.size() - 1) return false;
    return myContains(search, strings, start_from + 1);
}

template<bool, typename Then, typename Else>
struct if_{};

template<typename Then, typename Else>
struct if_<true, Then, Else>
{
    using value = Then;
};

template<typename Then, typename Else>
struct if_<false, Then, Else>
{
    using value = Else;
};

template<typename Search, typename TupleType, size_t StartFrom = 0>
struct my_contains : 
    if_<
        std::is_same<Search, std::tuple_element_t<StartFrom, TupleType>>::value,
        std::true_type,
        typename if_<
            (StartFrom == std::tuple_size<TupleType>::value - 1),
            std::false_type,
            my_contains<Search, TupleType, StartFrom + 1>  
        >::value
    >::value
{};

void printN() {}

template<typename T>
void printN(T t)
{
    std::cout << t << "\n";
}

template<typename T, typename... Extras>
void printN(T t, Extras... extras)
{
    std::cout << t << ", ";
    printN(extras...);
}

template<typename Tuple, size_t... Idxs>
void printTuple(Tuple tuple, std::index_sequence<Idxs...>) {
    printN(std::get<Idxs>(tuple)...);
}

template<typename Tuple>
void printTuple(Tuple tuple)
{
    printTuple(tuple, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}

int main()
{
    // Ep.1 - Intro
    Foo<char, char> foo;
    std::cout << std::boolalpha;
    std::cout << decltype(foo)::type << std::endl;
    std::cout << foo.getI() << std::endl;

    std::cout << "is_pointer<int>::value  :" << is_pointer<int>::value << std::endl;
    std::cout << "is_pointer<int*>::value :" << is_pointer<int*>::value << std::endl;

    int x = 5;
    int* x_p = &x;
    print(x);
    print(x_p);

 
    // Ep. 2 - writing MetaFunctions
    std::tuple<int, bool, float> tup(5, true, 1.0f);
    std::vector<std::string> strings{"int", "bool", "float"};
    std::cout << myContains("int", strings) << std::endl;
    std::cout << myContains("double", strings) << std::endl;
    std::cout << my_contains<int, decltype(tup)>::value << std::endl;
    std::cout << my_contains<double, decltype(tup)>::value << std::endl;

    // Ep. 3 - Variadic templates
    std::cout << "PRINTN:" << "\n";
    printN(1, "hi", 1.5f);
    auto tup2 = std::make_tuple<int, bool, double>(5, false, 1.4);
}