#include <string>
#include <iostream>
#include "my_shared_ptr.hpp"
#include "gtest/gtest.h"

struct SomeUsefulClass
{
    SomeUsefulClass() : message_(""){};
    SomeUsefulClass(std::string message) : message_(message){};
    std::string message_;
};

std::string functionSharedPtrAsArg(my_shared_ptr<SomeUsefulClass>& ptr)
{
    return ptr->message_;
}

std::string functionSharedPtrAsArg(my_shared_ptr<SomeUsefulClass>&& ptr)
{
    return ptr->message_;
}

my_shared_ptr<SomeUsefulClass> provider(std::string someString)
{
    return my_shared_ptr<SomeUsefulClass>(new SomeUsefulClass(someString));
}

class SharedPointerTestSuite : public ::testing::Test {
public:
  SharedPointerTestSuite()
      : m_someString("provided by provider"),
        m_msg("some important message"),
        m_sut(provider(m_msg)) {}

  std::string m_someString;
  std::string m_msg;
  my_shared_ptr<SomeUsefulClass> m_sut;
  my_shared_ptr<SomeUsefulClass> m_sut_empty = my_shared_ptr<SomeUsefulClass>();


  int initial_value = 42;
  int new_value = 100;

};


TEST_F(SharedPointerTestSuite, emptySharedPtr)
{
    EXPECT_EQ(m_sut_empty.get(), nullptr);
    EXPECT_EQ(m_sut_empty.use_count(), 0);
}

TEST_F(SharedPointerTestSuite, notEmptySharedPtr)
{
    EXPECT_NE(m_sut.get(), nullptr);
    EXPECT_EQ(m_sut.use_count(), 1);
}

TEST_F(SharedPointerTestSuite, moreReferencesSharedPointerTestSuite)
{
    my_shared_ptr<SomeUsefulClass> ptr = my_shared_ptr<SomeUsefulClass>(new SomeUsefulClass());
    EXPECT_NE(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 1);
    my_shared_ptr<SomeUsefulClass>& another_ptr = ptr;
    my_shared_ptr<SomeUsefulClass> yet_another_ptr(another_ptr);

    EXPECT_EQ(another_ptr->message_, "");
    EXPECT_EQ(ptr.use_count(), 2);
}

TEST_F(SharedPointerTestSuite, passedAsTemporary)
{
    EXPECT_EQ(m_msg, functionSharedPtrAsArg(provider(m_msg)));
}

TEST_F(SharedPointerTestSuite, passedWithMove)
{
    EXPECT_EQ(m_msg, functionSharedPtrAsArg(std::move(m_sut)));
}

TEST_F(SharedPointerTestSuite, moveAssigmenOperator)
{
    my_shared_ptr<SomeUsefulClass>&& ptr = std::move(m_sut);
    EXPECT_EQ(ptr->message_, m_msg);
}

TEST_F(SharedPointerTestSuite, emptyDefaultCtor)
{
    my_shared_ptr<SomeUsefulClass> ptr = my_shared_ptr<SomeUsefulClass>(new SomeUsefulClass());
    EXPECT_EQ(ptr->message_, "");
}

TEST_F(SharedPointerTestSuite, dereferencingAfterGet)
{
    EXPECT_EQ( (*(m_sut.get())).message_, m_msg);
}

TEST_F(SharedPointerTestSuite, asgnOperator)
{
    my_shared_ptr<SomeUsefulClass> copy_ptr = m_sut;
    EXPECT_EQ(copy_ptr.use_count(), 2);
}

TEST_F(SharedPointerTestSuite, CopyConstructor)
{
    my_shared_ptr<int> ptr = my_shared_ptr<int>(new int(initial_value));
    ASSERT_TRUE(ptr);
    my_shared_ptr<int> copy_ptr = my_shared_ptr<int>(ptr);
    ASSERT_TRUE(copy_ptr);

}

TEST_F(SharedPointerTestSuite, ShouldReset)
{
    my_shared_ptr<int> pointer_ = my_shared_ptr<int>(new int(initial_value));
    pointer_.reset(new int(new_value));
    EXPECT_EQ(*(pointer_.get()), new_value);
}

TEST_F(SharedPointerTestSuite, ShouldResetWithSimpleClass)
{
    m_sut.reset(new SomeUsefulClass(m_someString));
    EXPECT_EQ((*m_sut.get()).message_, m_someString);
}

TEST_F(SharedPointerTestSuite, ShouldResetWithNullptr)
{
    m_sut.reset(nullptr);
    EXPECT_EQ(m_sut.get(), nullptr);
}


