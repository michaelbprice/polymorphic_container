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
		static_assert(is_one_of<typename std::remove_reference<U>::type, DerivedTypes...>::value, "You cannot push_back one of those types here.");
		// check actual derivations????

		data[typeid(U)].push_back(static_cast<BaseType>(std::forward<U>(value)));

		for (const auto & t : data)
		{
			//std::cout << "Type: " << t.first.name() << "\t" << t.second.size() << "\n";

		}
		//std::cout << std::endl << std::endl;
	}



	struct iterator : public std::iterator<std::input_iterator_tag, BaseType>
	{
		const polymorphic_container<BaseType, DerivedTypes...> & stuff;

		std::vector<std::type_index> keys;
		std::size_t keyIndex = 0;
		std::size_t valueIndex = 0;

		bool isEnd;

		void printState()
		{
			std::cout << keyIndex << " " << valueIndex << std::boolalpha << isEnd << "\n";
		}

		iterator(const polymorphic_container<BaseType, DerivedTypes...> & _stuff, bool end = false)
		: stuff{_stuff}
		, isEnd{end}
		{
			auto keyIterator = stuff.data.cbegin();

			for (const auto & t : stuff.data)
			{
				keys.push_back(t.first);
			}
		}

		// define copy constructor... 

		iterator operator++ ()
		{
			if (!isEnd)
			{
				if (valueIndex < stuff.data.at(keys[keyIndex]).size() - 1)
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
			}

			return iterator{stuff, true};
		}

		iterator operator++ (int)
		{
			auto me = *this;

			if (!isEnd)
			{
				if (valueIndex < stuff.data.at(keys[keyIndex]).size() - 1)
				{
					valueIndex += 1;

				}
				else if (keyIndex < stuff.data.size() - 1)
				{
					keyIndex += 1;
					valueIndex = 0;
				}
				else
				{
					keyIndex = 0;
					valueIndex = 0;
					isEnd = true;
				}
			}

			return me;	
		}

		auto & operator* ()
		{
			return stuff.data.at(keys[keyIndex])[valueIndex];
		}

		bool operator== (iterator const & other)
		{
			if (&stuff != &other.stuff) return false;
			if (isEnd != other.isEnd) return false;
			if (keys != other.keys) return false;
			if (keyIndex != other.keyIndex) return false;
			if (valueIndex != other.valueIndex) return false;

			return true;
		}

		bool operator!= (iterator const & other)
		{
			return !(*this == other);
		}
	};


    iterator begin() const
    {
    	return iterator{*this, false};
    }

    iterator end() const
    {
    	return iterator{*this, true};
    }


};

/*
operator==(polymorphic_container<unique_ptr<PolymorphicBase>, unique_ptr<One>, unique_ptr<Two>, unique_ptr<Three> >::iterator const&,
	       polymorphic_container<unique_ptr<PolymorphicBase>, unique_ptr<One>, unique_ptr<Two>, unique_ptr<Three> >::iterator const&)
*/
/*
template <typename BaseType, typename ... DerivedTypes >
bool operator== (typename polymorphic_container<BaseType, DerivedTypes...>::iterator const & l,
	             typename polymorphic_container<BaseType, DerivedTypes...>::iterator const & r)
{
	return true;
}
*/



#endif // INCL_POLYMORPHIC_CONTAINER_H
