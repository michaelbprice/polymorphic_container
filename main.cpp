#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
#include <chrono>
#include <tuple>
#include <cmath>



#include "polymorphic_container.h"

constexpr auto gHowManyElements = 5000;
constexpr auto gSampleSize = 50000;

struct PolymorphicBase
{
    virtual void operation() = 0;
    void print() const { std::cout << x << "\n"; }
    int x{0};
};

struct One : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 1;
    }
};

struct Two : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 2;
    }
};

struct Three : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 3;
    }
};

struct Four : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 4;
    }
};

struct Five : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 5;
    }
};

struct Six : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 6;
    }
};

struct Seven : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 7;
    }
};

struct Eight : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 8;
    }
};

struct Nine : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 9;
    }
};

struct Ten : public PolymorphicBase
{
    virtual void operation() override
    {
        x = 10;
    }
};


template <typename Head = void>
std::unique_ptr<PolymorphicBase> createTypeFromIndex(std::size_t)
{
    static_assert(!std::is_same<Head, void>::value, "Can't have an empty type list!");
    return std::unique_ptr<PolymorphicBase>{new Head{}};
}

template <typename Head, typename Next, typename... Tail>
std::unique_ptr<PolymorphicBase> createTypeFromIndex(std::size_t index)
{
    if (index == 0)
    {
        return std::unique_ptr<PolymorphicBase>{new Head{}};
    }

    return createTypeFromIndex<Next, Tail...>(index-1);
}

template <typename PolyContainerType, typename Head = void>
void addElementToPolyContainer(PolyContainerType & container, std::size_t)
{
    static_assert(!std::is_same<Head, void>::value, "Can't have an empty type list!");
    std::cout << "only one" << std::endl;
    //std::unique_ptr<Head>{new Head{}};
    container.push_back(std::make_unique<Head>());

    std::cout << "added" << std::endl;
}

template <typename PolyContainerType, typename Head, typename Next, typename... Tail>
void addElementToPolyContainer(PolyContainerType & container, std::size_t index)
{
std::cout << "Looking for right type: " << index << std::endl;

    if (index == 0)
    {
        //return std::unique_ptr<Head>{new Head{}};
        //return std::make_unique<Head>();
        container.push_back(std::make_unique<Head>());
        std::cout << "added" << std::endl;
        return;
    }

    addElementToPolyContainer<PolyContainerType, Next, Tail...>(container, index-1);

}



template <typename DistributionType, typename... ConcreteTypes>
std::vector<std::unique_ptr<PolymorphicBase>> createRandomVector (DistributionType & distribution)
{
    auto v = std::vector<std::unique_ptr<PolymorphicBase>>{gHowManyElements};

    //std::uniform_int_distribution<> distribution{0, sizeof...(ConcreteTypes) - 1};
    std::mt19937 engine{};

    std::generate(v.begin(), v.end(), [&]()
    {
        const auto which = std::lround(distribution(engine));
        return createTypeFromIndex<ConcreteTypes...>(which);
    });

    return v;
}

template <typename DistributionType, typename... ConcreteTypes>
polymorphic_container<std::unique_ptr<PolymorphicBase>, std::unique_ptr<ConcreteTypes>...> createRandomPolyVector (DistributionType & distribution)
{
    auto v = polymorphic_container<std::unique_ptr<PolymorphicBase>, std::unique_ptr<ConcreteTypes>...>{}; // gHowManyElements ???

    //std::cout << "Say What" << std::endl;

    std::mt19937 engine{};

    for (auto i = 0; i < gSampleSize; ++i)
    {
        const auto which = std::lround(distribution(engine));
        //auto p = addElementToPolyContainer<ConcreteTypes...>(which);
        //std::cout << "Doing " << i << std::endl;
        addElementToPolyContainer<decltype(v), ConcreteTypes...>(v, which);
        //std::cout << "Done with " << i << std::endl;
        //v.push_back(p);
    }
/*
    std::generate(v.begin(), v.end(), [&]()
    {
        const auto which = std::lround(distribution(engine));
        return createTypeFromIndex<ConcreteTypes...>(which);
    });
    */

//std::cout << "Made the vector yo" << std::endl;

    return v;    
}

template <typename Head>
std::tuple<long> countTypes (std::vector<std::unique_ptr<PolymorphicBase>> const & v)
{
    auto count = std::count_if(v.cbegin(), v.cend(), [](std::unique_ptr<PolymorphicBase> const & p)
    {
        return dynamic_cast<Head*>(p.get()) != nullptr;
    });
    return std::make_tuple(count);
}

template <typename Head, typename Next, typename... Tail>
auto countTypes (std::vector<std::unique_ptr<PolymorphicBase>> const & v)
    -> decltype(std::tuple_cat(std::make_tuple(1L), countTypes<Next, Tail...>(v)))
{
    auto count = std::count_if(v.cbegin(), v.cend(), [](std::unique_ptr<PolymorphicBase> const & p)
    {
        return dynamic_cast<Head*>(p.get()) != nullptr;
    });

    return std::tuple_cat(std::make_tuple(count), countTypes<Next, Tail...>(v));
}

template <typename TupleType>
void printTypeDistributionImpl (const TupleType & t)
{
}

template <typename TupleType, typename Head, typename... Tail>
void printTypeDistributionImpl (const TupleType & t)
{
    constexpr auto index = std::tuple_size<TupleType>::value - (sizeof...(Tail) + 1);

    std::cout << typeid(Head).name() << ": " << std::get<index>(t) << "\n";

    printTypeDistributionImpl<TupleType, Tail...>(t);
}

template <typename TupleType, typename Head, typename... Tail>
void printTypeDistribution (const TupleType & t)
{
    static_assert(sizeof...(Tail) + 1 == std::tuple_size<TupleType>::value, "Tuple is not same length as type list");

    printTypeDistributionImpl<TupleType, Head, Tail...>(t);
}


template <typename Head>
std::tuple<long> countPolyTypes (polymorphic_container<std::unique_ptr<PolymorphicBase>,
                                                       std::unique_ptr<Head>> const & v)
{
   long count = 0;

   for (auto i = v.begin(); i != v.end(); i++) // TODO preincrement is broken;
   {
       i.printState();

       if (dynamic_cast<Head*>((*i).get()) != nullptr)
       {
           count++;
       }
   }

/* For some reason this doesn't work with my jankity iterator implementation

    auto count = std::count_if(v.begin(), v.end(), [](std::unique_ptr<PolymorphicBase> const & p)
    {
//        std::cout << "Counting" << std::endl;
        return dynamic_cast<Head*>(p.get()) != nullptr;
    });
*/

    return std::make_tuple(count);
}

template <typename Head, typename Next, typename... Tail>
auto countPolyTypes (polymorphic_container<std::unique_ptr<PolymorphicBase>,
                                           std::unique_ptr<Head>,
                                           std::unique_ptr<Next>,
                                           std::unique_ptr<Tail>...> const & v)
//    -> decltype(std::tuple_cat(std::make_tuple(1L), countPolyTypes<Next, Tail...>(v)))
{
   long count = 0;

   for (auto i = v.begin(); i != v.end(); i++) // TODO preincrement is broken;
   {
       i.printState();

       if (dynamic_cast<Head*>((*i).get()) != nullptr)
       {
           count++;
       }
   }

/*
    auto count = std::count_if(v.cbegin(), v.cend(), [](std::unique_ptr<PolymorphicBase> const & p)
    {
        return dynamic_cast<Head*>(p.get()) != nullptr;
    });
*/

    return std::tuple_cat(std::make_tuple(count), countPolyTypes<Next, Tail...>(v));
}


template <typename DistributionType, typename... ConcreteTypes>
void testStandardVector (DistributionType & distribution)
{
    auto v = createRandomVector<decltype(distribution), ConcreteTypes...>(distribution);

    auto counts = countTypes<ConcreteTypes...>(v);

    printTypeDistribution<decltype(counts), ConcreteTypes...>(counts);

    auto total = 0UL;

    for (auto x = 0; x < gSampleSize; ++x)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (auto const &i : v) {
            i->operation();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }

    std::cout << "Average Duration: " << total/gSampleSize << "\n" << std::endl;
}

template <typename DistributionType, typename... ConcreteTypes>
void testPolyContainer(DistributionType & distribution)
{
    //std::cout << "Oh Please" << std::endl;
    auto v = createRandomPolyVector<DistributionType, ConcreteTypes...>(distribution);

//std::cout << "Shall we count" << std::endl;
    auto counts = countPolyTypes<ConcreteTypes...>(v);

//std::cout << "Maybe we print" << std::endl;
    printTypeDistribution<decltype(counts), ConcreteTypes...>(counts);

    auto total = 0UL;

    for (auto x = 0; x < gSampleSize; ++x)
    {
        auto start = std::chrono::high_resolution_clock::now();

        auto end = v.end();

        for (auto i = v.begin(); i != end; i++) {
            //i.printState();
            (*i)->operation();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }

    std::cout << "Average Duration: " << total/gSampleSize << "\n" << std::endl;
}

int main ()
{

    std::uniform_int_distribution<> uniform1{0, 0};
    std::cout << "Testing std::vector with 1 type (uniform distribution)" << std::endl;
    testStandardVector<decltype(uniform1), One>(uniform1);

    std::uniform_int_distribution<> uniform2{0, 1};
    std::cout << "Testing std::vector with 2 types (uniform distribution)" << std::endl;
    testStandardVector<decltype(uniform2), One, Two>(uniform2);

/*
    std::uniform_int_distribution<> uniform3{0, 2};
    std::cout << "Testing three types (uniform)" << std::endl;
    testStandardVector<decltype(uniform3), One, Two, Three>(uniform3);

    std::uniform_int_distribution<> uniform4{0, 3};
    std::cout << "Testing four types (uniform)" << std::endl;
    testStandardVector<decltype(uniform4), One, Two, Three, Four>(uniform4);

    std::uniform_int_distribution<> uniform5{0, 4};
    std::cout << "Testing five types (uniform)" << std::endl;
    testStandardVector<decltype(uniform5), One, Two, Three, Four, Five>(uniform5);

    std::uniform_int_distribution<> uniform10{0, 9};
    std::cout << "Testing ten types (uniform)" << std::endl;
    testStandardVector<decltype(uniform10), One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten>(uniform10);


    std::normal_distribution<> normal1{0, 0};
    std::cout << "Testing one type (normal)" << std::endl;
    testStandardVector<decltype(normal1), One>(normal1);

    std::normal_distribution<> normal2{0.5, 0.1};
    std::cout << "Testing two types (normal)" << std::endl;
    testStandardVector<decltype(normal2), One, Two>(normal2);

    std::normal_distribution<> normal3{1.0, 0.2};
    std::cout << "Testing three types (normal)" << std::endl;
    testStandardVector<decltype(normal3), One, Two, Three>(normal3);

    std::normal_distribution<> normal4{1.5, 0.3};
    std::cout << "Testing four types (normal)" << std::endl;
    testStandardVector<decltype(normal4), One, Two, Three, Four>(normal4);

    std::normal_distribution<> normal5{2, 0.4};
    std::cout << "Testing five types (normal)" << std::endl;
    testStandardVector<decltype(normal5), One, Two, Three, Four, Five>(normal5);

    std::normal_distribution<> normal10{4.5, 0.8};
    std::cout << "Testing ten types (normal)" << std::endl;
    testStandardVector<decltype(normal10), One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten>(normal10);
*/


    std::cout << "Testing polymorphic_container with 1 type (uniform distribution)" << std::endl;
    testPolyContainer<decltype(uniform1), One>(uniform1);

//    std::cout << "Testing polymorphic_container with 2 types (uniform distribution)" << std::endl;
//    testPolyContainer<decltype(uniform2), One, Two>(uniform2);


//    polymorphic_container<std::unique_ptr<PolymorphicBase>,
//                          std::unique_ptr<One>, std::unique_ptr<Two>, std::unique_ptr<Three>> pc;
 
    //addElementToPolyContainer<decltype(pc), One, Two>(pc, 1);


/*
    pc.push_back(std::make_unique<One>());
    pc.push_back(std::make_unique<Three>());
    pc.push_back(std::make_unique<One>());
    pc.push_back(std::make_unique<Two>());
    pc.push_back(std::make_unique<Three>());
*/

/*
    auto b = pc.begin();
    b.printState();
    ++b;
    b.printState();
    b++;
    b.printState();
    //b++;
    //b.printState();

    auto b2 = pc.begin();

    auto e = pc.end();
    e.printState();
    e++;
    e.printState();

    std::cout << std::boolalpha << (b == e) << std::endl;
*/

/*
    for (auto i = pc.begin(); i != pc.end(); i++)
    {
        //i.printState();
        (*i)->operation();
        (*i)->print();
    }
*/

    //std::cout << std::boolalpha << (b == b2) << std::endl;
    //std::cout << std::boolalpha << (b == e) << std::endl;

/*
    decltype(pc)::iterator i(pc);
    (*i)->operation();
    (*i)->print();

i++;

    (*i)->operation();
    (*i)->print();

++i;

    (*i)->operation();
    (*i)->print();

++i;
    (*i)->operation();
    (*i)->print();

++i;
    (*i)->operation();
    (*i)->print();
*/
    return 0;
}

