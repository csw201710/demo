#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>


int main(int argc, char* argv[])
{
		
    CppUnit::TestResult r; 
    CppUnit::TestResultCollector rc;
    r.addListener(&rc); // 准备好结果收集器 

    CppUnit::TestRunner runner; // 定义执行实体
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(r); // 运行测试

    CppUnit::TextOutputter o(&rc, std::cout);
    o.write(); // 将结果输出

    return rc.wasSuccessful() ? 0 : -1;
} 

