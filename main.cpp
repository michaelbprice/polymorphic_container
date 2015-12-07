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

constexpr auto gHowMany = 10000;

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


template <typename DistributionType, typename... ConcreteTypes>
std::vector<std::unique_ptr<PolymorphicBase>> createRandomVector (DistributionType & distribution)
{
    auto v = std::vector<std::unique_ptr<PolymorphicBase>>{gHowMany};

    //std::uniform_int_distribution<> distribution{0, sizeof...(ConcreteTypes) - 1};
    std::mt19937 engine{};

    std::generate(v.begin(), v.end(), [&]()
    {
        const auto which = std::lround(distribution(engine));
        return createTypeFromIndex<ConcreteTypes...>(which);
    });

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

template <typename DistributionType>
void testOneType(DistributionType & distribution)
{
    auto v = createRandomVector<decltype(distribution), One>(distribution);

    auto counts = countTypes<One>(v);

    std::cout << "Ones: " << std::get<0>(counts) << "\n";
    constexpr auto loopCount = 500;
    auto total = 0UL;

    for (auto x = 0; x < loopCount; ++x)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (auto const &i : v) {
            i->operation();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }

    std::cout << "Average Duration: " << total/loopCount << "\n" << std::endl;
}

template <typename DistributionType>
void testTwoTypes(DistributionType & distribution)
{
    auto v = createRandomVector<decltype(distribution), One, Two>(distribution);

    auto counts = countTypes<One, Two>(v);

    std::cout << "Ones: " << std::get<0>(counts) << "\n";
    std::cout << "Twos: " << std::get<1>(counts) << "\n";
    constexpr auto loopCount = 500;
    auto total = 0UL;

    for (auto x = 0; x < loopCount; ++x)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (auto const &i : v) {
            i->operation();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }

    std::cout << "Average Duration: " << total/loopCount << "\n" << std::endl;
}

template <typename DistributionType>
void testThreeTypes(DistributionType & distribution)
{
    auto v = createRandomVector<decltype(distribution), One, Two, Three>(distribution);

    auto counts = countTypes<One, Two, Three>(v);

    std::cout << "Ones: " << std::get<0>(counts) << "\n";
    std::cout << "Twos: " << std::get<1>(counts) << "\n";
    std::cout << "Three: " << std::get<2>(counts) << "\n";
    constexpr auto loopCount = 500;
    auto total = 0UL;

    for (auto x = 0; x < loopCount; ++x)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (auto const &i : v) {
            i->operation();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }

    std::cout << "Average Duration: " << total/loopCount << "\n" << std::endl;
}

template <typename DistributionType>
void testFourTypes(DistributionType & distribution)
{
    auto v = createRandomVector<decltype(distribution), One, Two, Three, Four>(distribution);

    auto counts = countTypes<One, Two, Three, Four>(v);

    std::cout << "Ones: " << std::get<0>(counts) << "\n";
    std::cout << "Twos: " << std::get<1>(counts) << "\n";
    std::cout << "Three: " << std::get<2>(counts) << "\n";
    std::cout << "Four: " << std::get<3>(counts) << "\n";
    constexpr auto loopCount = 500;
    auto total = 0UL;

    for (auto x = 0; x < loopCount; ++x)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (auto const &i : v) {
            i->operation();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }

    std::cout << "Average Duration: " << total/loopCount << "\n" << std::endl;
}

template <typename DistributionType>
void testFiveTypes(DistributionType & distribution)
{
    auto v = createRandomVector<decltype(distribution), One, Two, Three, Four, Five>(distribution);

    auto counts = countTypes<One, Two, Three, Four, Five>(v);

    std::cout << "Ones: " << std::get<0>(counts) << "\n";
    std::cout << "Twos: " << std::get<1>(counts) << "\n";
    std::cout << "Three: " << std::get<2>(counts) << "\n";
    std::cout << "Four: " << std::get<3>(counts) << "\n";
    std::cout << "Five: " << std::get<4>(counts) << "\n";
    constexpr auto loopCount = 500;
    auto total = 0UL;

    for (auto x = 0; x < loopCount; ++x)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (auto const &i : v) {
            i->operation();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }

    std::cout << "Average Duration: " << total/loopCount << "\n" << std::endl;
}

template <typename DistributionType>
void testTenTypes(DistributionType & distribution)
{
    auto v = createRandomVector<decltype(distribution), One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten>(distribution);

    auto counts = countTypes<One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten>(v);

    std::cout << "Ones: " << std::get<0>(counts) << "\n";
    std::cout << "Twos: " << std::get<1>(counts) << "\n";
    std::cout << "Three: " << std::get<2>(counts) << "\n";
    std::cout << "Four: " << std::get<3>(counts) << "\n";
    std::cout << "Five: " << std::get<4>(counts) << "\n";
    std::cout << "Six: " << std::get<5>(counts) << "\n";
    std::cout << "Seven: " << std::get<6>(counts) << "\n";
    std::cout << "Eight: " << std::get<7>(counts) << "\n";
    std::cout << "Nine: " << std::get<8>(counts) << "\n";
    std::cout << "Ten: " << std::get<9>(counts) << "\n";
    constexpr auto loopCount = 500;
    auto total = 0UL;

    for (auto x = 0; x < loopCount; ++x)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (auto const &i : v) {
            i->operation();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        total += duration.count();
    }

    std::cout << "Average Duration: " << total/loopCount << "\n" << std::endl;
}

int main ()
{
/*
    std::uniform_int_distribution<> uniformDistribution1{0, 0};
    std::cout << "Testing one type (uniform)" << std::endl;
    testOneType(uniformDistribution1);

    std::uniform_int_distribution<> uniformDistribution2{0, 1};
    std::cout << "Testing two types (uniform)" << std::endl;
    testTwoTypes(uniformDistribution2);

    std::uniform_int_distribution<> uniformDistribution3{0, 2};
    std::cout << "Testing three types (uniform)" << std::endl;
    testThreeTypes(uniformDistribution3);

    std::uniform_int_distribution<> uniformDistribution4{0, 3};
    std::cout << "Testing four types (uniform)" << std::endl;
    testFourTypes(uniformDistribution4);

    std::uniform_int_distribution<> uniformDistribution5{0, 4};
    std::cout << "Testing five types (uniform)" << std::endl;
    testFiveTypes(uniformDistribution5);

    std::uniform_int_distribution<> uniformDistribution10{0, 9};
    std::cout << "Testing ten types (uniform)" << std::endl;
    testTenTypes(uniformDistribution10);
*/
/*
    std::normal_distribution<> normalDistribution1{0, 0};
    std::cout << "Testing one type (normal)" << std::endl;
    testOneType(normalDistribution1);

    std::normal_distribution<> normalDistribution2{0.5, 0.1};
    std::cout << "Testing two types (normal)" << std::endl;
    testTwoTypes(normalDistribution2);

    std::normal_distribution<> normalDistribution3{1.0, 0.2};
    std::cout << "Testing three types (normal)" << std::endl;
    testThreeTypes(normalDistribution3);

    std::normal_distribution<> normalDistribution4{1.5, 0.3};
    std::cout << "Testing four types (normal)" << std::endl;
    testFourTypes(normalDistribution4);

    std::normal_distribution<> normalDistribution5{2, 0.4};
    std::cout << "Testing five types (normal)" << std::endl;
    testFiveTypes(normalDistribution5);

    std::normal_distribution<> normalDistribution10{4.5, 0.8};
    std::cout << "Testing ten types (normal)" << std::endl;
    testTenTypes(normalDistribution10);
*/

    polymorphic_container<std::unique_ptr<PolymorphicBase>,
                          std::unique_ptr<One>, std::unique_ptr<Two>, std::unique_ptr<Three>> pc;
 
    pc.push_back(std::make_unique<One>());
    pc.push_back(std::make_unique<Three>());
    //pc.push_back(std::make_unique<One>());
    pc.push_back(std::make_unique<Two>());

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

    return 0;
}

