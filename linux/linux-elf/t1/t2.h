#ifndef __T2__h__
#define __T2__h__
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>




class SysTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SysTest);  
    //CPPUNIT_TEST(testService);
    //CPPUNIT_TEST(testPipe);
    CPPUNIT_TEST(parseElf);
    CPPUNIT_TEST_SUITE_END(); 
private:
		
public:
		SysTest();
    void setUp();
    void tearDown();
    void testService();
    void testPipe();
    void parseElf();
};





#endif // __T2__h__