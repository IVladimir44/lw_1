#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>
#include "../ConsoleApplication1/ConsoleApplication1.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::string expected = "clear display : black";

			std::stringstream buffer;
			std::streambuf* sbuf = std::cout.rdbuf(); // Save cout's buffer
			std::cout.rdbuf(buffer.rdbuf()); // Redirect cout to the stringstream buffer

			int result = main();

			std::cout.rdbuf(sbuf);
			std::cout << "std original buffer: \n";
			std::cout << buffer.get();

			// Test
			Assert::AreEqual(expected, buffer.str());
		}
	};
}
