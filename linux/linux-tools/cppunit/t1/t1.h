#ifndef __T1__h__
#define __T1__h__
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

// 定义测试类
class T001 : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(T001);  
    CPPUNIT_TEST(t1);
    CPPUNIT_TEST_SUITE_END(); 
private:
		
public:
    void setUp();
    void tearDown();

    void t1();
protected:
};

#endif //__T1__h__