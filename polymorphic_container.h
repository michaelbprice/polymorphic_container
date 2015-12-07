#ifndef INCL_POLYMORPHIC_CONTAINER_H
#define INCL_POLYMORPHIC_CONTAINER_H

#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <map>
#include <functional>


template <typename T, typename Head, typename ... Tail>
struct is_one_of
{
	static constexpr bool value = std::is_same<T, Head>::value || is_one_of<T, Tail...>::value;
};

template <typename T, typename Head>
struct is_one_of<T, Head>
{
	static constexpr bool value = std::is_same<T, Head>::value;
};




template <typename BaseType, typename ... DerivedTypes >
class polymorphic_container
{
private:
	std::map<std::type_index, std::vector<BaseType>> data;


public:

	template <typename U>
	void push_back (U && value)
	{
		static_assert(is_one_of<U, DerivedTypes...>::value, "You cannot push_back one of those types here.");
		// check actual derivations????

		data[typeid(U)].push_back(std::forward<U>(value));

		for (const auto & t : data)
		{
			//std::cout << "Type: " << t.first.name() << "\t" << t.second.size() << "\n";

		}
		//std::cout << std::endl << std::endl;
	}



	struct iterator : public std::iterator<std::input_iterator_tag, BaseType>
	{
		polymorphic_container<BaseType, DerivedTypes...> & stuff;

		std::vector<std::type_index> keys;
		std::size_t keyIndex = 0;
		std::size_t valueIndex = 0;

		iterator(polymorphic_container<BaseType, DerivedTypes...> & _stuff)
		: stuff{_stuff}
		{
			auto keyIterator = stuff.data.cbegin();

			for (const auto & t : stuff.data)
			{
				keys.push_back(t.first);
			}
		}

		iterator operator++ ()
		{
			if (valueIndex < stuff.data[keys[keyIndex]].size() - 1)
			{
				valueIndex += 1;
				return *this;
			}
			else if (keyIndex < stuff.data.size() - 1)
			{
				keyIndex += 1;
				valueIndex = 0;
				return *this;
			}
			else
			{
				return *this;
			}
		}

		iterator operator++ (int)
		{
			auto me = *this;

			if (valueIndex < stuff.data[keys[keyIndex]].size() - 1)
			{
				valueIndex += 1;

			}
			else if (keyIndex < stuff.data.size() - 1)
			{
				keyIndex += 1;
				valueIndex = 0;
			}

			return me;	
		}

		BaseType & operator* ()
		{
			return stuff.data[keys[keyIndex]][valueIndex];
		}
	};



};



#endif // INCL_POLYMORPHIC_CONTAINER_H
