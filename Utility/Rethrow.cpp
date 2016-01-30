//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for the re-throwing of exceptions.

								Well, actually just a test harness...

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/StringSupport.hpp>
#include <Utility/Utility_Exception.hpp>

//	////////////////////////////////////////////////////////////////////////////

//namespace MLB {
//namespace Utility {
//namespace PRIVATE_IMPLEMENTATION__MLB__Utility__Rethrow_cpp {
//	////////////////////////////////////////////////////////////////////////////
//	Dinky public symbol to prevent complaints by MS VC++...
extern const int
	PRIVATE_IMPLEMENTATION__MLB__Utility__Rethrow_cpp_SomeBogusSymbol = 0;
//	////////////////////////////////////////////////////////////////////////////
//} // namespace PRIVATE_IMPLEMENTATION__MLB__Utility__Rethrow_cpp
//} // namespace Utility
//} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::Utility;

//	////////////////////////////////////////////////////////////////////////////
class TestException_1 : public ExceptionGeneral {
public:
	TestException_1() throw() :
		 ExceptionGeneral("Unspecified TibCo/Rendezvous exception #1.") { }
	TestException_1(const char *except_string) throw() :
		ExceptionGeneral(except_string) { }
	TestException_1(const std::string &except_string) throw() :
		ExceptionGeneral(except_string) { }
	TestException_1(const std::ostringstream &except_string) throw() :
		ExceptionGeneral(except_string) { }
	~TestException_1() throw() { }

	const char *what() const throw() {
		std::ostringstream what_text;
		what_text << "***** TestException_1 *****: " << except_string_;
		tmp_text = what_text.str();
		return(tmp_text.c_str());
	}

	virtual void Rethrow(const char *except_string = NULL) const {
		TestException_1 tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}

	bool                TEST_EXCEPTION;
	mutable std::string tmp_text;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class TestException_2 : public TestException_1 {
public:
	TestException_2() throw() :
		 TestException_1("Unspecified TibCo/Rendezvous exception #2.") { }
	TestException_2(const char *except_string) throw() :
		TestException_1(except_string) { }
	TestException_2(const std::string &except_string) throw() :
		TestException_1(except_string) { }
	TestException_2(const std::ostringstream &except_string) throw() :
		TestException_1(except_string) { }
	~TestException_2() throw() { }

	const char *what() const throw() {
		std::ostringstream what_text;
		what_text << "***** TestException_2 *****: " << except_string_;
		tmp_text = what_text.str();
		return(tmp_text.c_str());
	}

	virtual void Rethrow(const char *except_string = NULL) const {
		TestException_2 tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}

	bool                TEST_EXCEPTION;
	mutable std::string tmp_text;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class UnaffiliatedException {
public:
	UnaffiliatedException(const char *except_string) throw() :
		 the_text_(except_string) { }
	~UnaffiliatedException() throw() { }

	std::string the_text_;
};
//	////////////////////////////////////////////////////////////////////////////

using namespace MLB::Utility;

#include <iomanip>

//	////////////////////////////////////////////////////////////////////////////
void TEST_1()
{
#if defined(_MSC_VER) && !defined(__MINGW32__)
	throw std::exception("Standard Exception");
#else
	throw std::exception();
#endif // #if defined(_MSC_VER) && !defined(__MINGW32__)

}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_2()
{
	throw std::domain_error("Standard Exception Domain Error");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_3()
{
#if defined(_MSC_VER) && !defined(__MINGW32__)
	throw std::bad_alloc("Standard Exception Bad Alloc");
#else
	throw std::bad_alloc();
#endif // #if defined(_MSC_VER) && !defined(__MINGW32__)
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_4()
{
	throw ExceptionGeneral("MLB Exception General");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_5()
{
	throw TestException_1("MLB Test Exception #1");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_6()
{
	throw TestException_2("MLB Test Exception #2");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_7()
{
	throw UnaffiliatedException("MLB Test UnaffiliatedException");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_8()
{
	throw 666;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct TEST_Datum {
	void (*func)();
	char  *name;
};
#define DEF_TEST_ELEMENT(the_func)	\
	{	the_func, #the_func	}
static const TEST_Datum   TEST_List[] = {
	DEF_TEST_ELEMENT(TEST_1),
	DEF_TEST_ELEMENT(TEST_2),
	DEF_TEST_ELEMENT(TEST_3),
	DEF_TEST_ELEMENT(TEST_4),
	DEF_TEST_ELEMENT(TEST_5),
	DEF_TEST_ELEMENT(TEST_6),
	DEF_TEST_ELEMENT(TEST_7),
	DEF_TEST_ELEMENT(TEST_8)
};
static const unsigned int TEST_Count  =
	sizeof(TEST_List) / sizeof(TEST_List[0]);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int TEST_Rethrow()
{
	int return_code = EXIT_SUCCESS;

	std::cout << PadLeft("", 79, '=') << std::endl;

	std::cout << "Test routine for MLB::Utility::Rethrow()" << std::endl;
	std::cout << "---- ------- --- -----------------------" << std::endl;

	unsigned int count_1;
	for (count_1 = 0; count_1 < TEST_Count; ++count_1) {
		try {
			try {
				std::cout << "Test " << std::setw(5) << count_1 << ": " <<
					TEST_List[count_1].name << ": " << std::endl;
				(*TEST_List[count_1].func)();
			}
			//	Catch the std::exception hierarchy...
			catch (const std::exception &except) {
				std::cout << "          : " << except.what() << std::endl;
				std::ostringstream error_text;
				error_text << except.what() << ": " << "Second stage";
				Rethrow(except, error_text);
			}
			//	Catch particular exceptions not in the std::exception hierarchy...
			catch (const UnaffiliatedException &except) {
				std::cout << "          : (UnaffiliatedException)" << std::endl;
				std::ostringstream error_text;
				error_text << "(UnaffiliatedException): " << "Second stage";
				Rethrow(except, error_text);
			}
			//	Catch all other exceptions not in the std::exception hierarchy...
			catch (...) {
				std::cout << "          : (*OTHER*)" << std::endl;
			}
		}
		catch (...) {
			try {
				throw;
			}
			catch (const TestException_2 &except) {
				std::cout << "          : " << except.what() << std::endl;
				std::cout << "          : TestException_2 CONFIRMED" << std::endl;
			}
			catch (const TestException_1 &except) {
				std::cout << "          : " << except.what() << std::endl;
				std::cout << "          : TestException_1 CONFIRMED" << std::endl;
			}
			catch (const ExceptionGeneral &except) {
				std::cout << "          : " << except.what() << std::endl;
				std::cout << "          : ExceptionGeneral CONFIRMED" << std::endl;
			}
			catch (const std::bad_alloc &except) {
				std::cout << "          : " << except.what() << std::endl;
				std::cout << "          : std::bad_alloc CONFIRMED" << std::endl;
			}
			catch (const std::domain_error &except) {
				std::cout << "          : " << except.what() << std::endl;
				std::cout << "          : std::domain_error CONFIRMED" << std::endl;
			}
			catch (const std::exception &except) {
				std::cout << "          : " << except.what() << std::endl;
				std::cout << "          : std::exception CONFIRMED" << std::endl;
			}
			catch (const UnaffiliatedException &) {
				std::cout << "          : Second stage" << std::endl;
				std::cout << "          : UnaffiliatedException CONFIRMED" <<
					std::endl;
			}
			catch (...) {
				std::cout << "          : (*OTHER*) CONFIRMED" << std::endl;
			}
		}
	}

	std::cout << PadLeft("", 79, '=') << std::endl << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class ExceptionGeneralX_1 : public virtual std::exception {
public:
	ExceptionGeneralX_1(const char *except_string = NULL) throw() :
#if defined(_MSC_VER) && !defined(__MINGW32__)
		 std::exception(GetFixedString(except_string))
#else
		 std::exception()
#endif // #if defined(_MSC_VER) && !defined(__MINGW32__)
		,except_string_(GetFixedString(except_string)) { }
	ExceptionGeneralX_1(const std::string &except_string) throw() :
#if defined(_MSC_VER) && !defined(__MINGW32__)
		 std::exception(GetFixedString(except_string.c_str()))
#else
		 std::exception()
#endif // #if defined(_MSC_VER) && !defined(__MINGW32__)
		,except_string_(GetFixedString(except_string.c_str())) { }
	ExceptionGeneralX_1(const std::ostringstream &except_string) throw() :
#if defined(_MSC_VER) && !defined(__MINGW32__)
		 std::exception(GetFixedString(except_string.str().c_str()))
#else
		 std::exception()
#endif // #if defined(_MSC_VER) && !defined(__MINGW32__)
		,except_string_(GetFixedString(except_string.str().c_str())) { }
	~ExceptionGeneralX_1() throw() { }

	const char *what() const throw() {
		return(except_string_.c_str());
	}

	void SetWhat(const char *except_string = NULL) throw() {
		except_string_ = GetFixedString(except_string);
	}
	void SetWhat(const std::string &except_string) throw() {
		except_string_ = GetFixedString(except_string.c_str());
	}
	void SetWhat(const std::ostringstream &except_string) throw() {
		except_string_ = GetFixedString(except_string.str().c_str());
	}

	virtual void Rethrow(const char *except_string = NULL) const {
		ExceptionGeneralX_1 tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
	//	Needn't be virtual.
	void Rethrow(const std::string &except_string) const {
		Rethrow(except_string.c_str());
	}
	//	Needn't be virtual.
	void Rethrow(const std::ostringstream &except_string) const {
		Rethrow(except_string.str().c_str());
	}


protected:
	std::string except_string_;

private:
	static const char *GetFixedString(const char *except_string = NULL) {
		return(((except_string == NULL) || (!(*except_string))) ?
			 "Unspecified exception." : except_string);
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class ExceptionGeneralX_2 : public virtual ExceptionGeneralX_1 {
public:
	ExceptionGeneralX_2(const char *except_string = NULL) throw() :
		 std::exception()
		,ExceptionGeneralX_1(except_string) { }
	ExceptionGeneralX_2(const std::string &except_string) throw() :
		 std::exception()
		,ExceptionGeneralX_1(except_string.c_str()) { }
	ExceptionGeneralX_2(const std::ostringstream &except_string) throw() :
		 std::exception()
		,ExceptionGeneralX_1(except_string.str().c_str()) { }
	~ExceptionGeneralX_2() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		ExceptionGeneralX_2 tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class ExceptionGeneralX_3 :
	public virtual ExceptionGeneralX_2,
	public virtual ExceptionGeneralX_1, public virtual std::exception {
public:
	ExceptionGeneralX_3(const char *except_string = NULL) throw() :
		 std::exception()
		,ExceptionGeneralX_1(except_string) { }
	ExceptionGeneralX_3(const std::string &except_string) throw() :
		 std::exception()
		,ExceptionGeneralX_1(except_string.c_str()) { }
	ExceptionGeneralX_3(const std::ostringstream &except_string) throw() :
		 std::exception()
		,ExceptionGeneralX_1(except_string.str().c_str()) { }
	~ExceptionGeneralX_3() throw() { }

	virtual void Rethrow(const char *except_string = NULL) const {
		ExceptionGeneralX_3 tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int TEST_VirtualBases()
{
	int return_code = EXIT_SUCCESS;

	std::cout << PadLeft("", 79, '=') << std::endl;

	std::cout << "Test routine for virtual base exceptions" << std::endl;
	std::cout << "---- ------- --- ------- ---- ----------" << std::endl;

	std::string test_name;

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	ExceptionGeneralX_1: Complete test.
	//	//////////////////////////////////////////////////////////////////////
	try {
		test_name = "A: Threw ExceptionGeneralX_1, caught ";
		throw ExceptionGeneralX_1("I'm an ExceptionGeneralX_1 object");
	}
	catch (const ExceptionGeneralX_3 &except) {
		std::cout << test_name << "ExceptionGeneralX_3" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (const ExceptionGeneralX_2 &except) {
		std::cout << test_name << "ExceptionGeneralX_2" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (const ExceptionGeneralX_1 &except) {
		std::cout << test_name << "ExceptionGeneralX_1" <<
			" (" << except.what() << ")" << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << test_name << "std::exception" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (...) {
		std::cout << test_name << "???" << std::endl;
		return_code = EXIT_FAILURE;
	}
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	ExceptionGeneralX_1: Fall-through to std::exception test.
	//	//////////////////////////////////////////////////////////////////////
	try {
		test_name = "B: Threw ExceptionGeneralX_1, caught ";
		throw ExceptionGeneralX_1("I'm an ExceptionGeneralX_1 object");
	}
	catch (const ExceptionGeneralX_3 &except) {
		std::cout << test_name << "ExceptionGeneralX_3" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (const ExceptionGeneralX_2 &except) {
		std::cout << test_name << "ExceptionGeneralX_2" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
/*
	catch (const ExceptionGeneralX_1 &except) {
		std::cout << test_name << "ExceptionGeneralX_1" <<
			" (" << except.what() << ")" << std::endl;
	}
*/
	catch (const std::exception &except) {
		std::cout << test_name << "std::exception" <<
			" (" << except.what() << ")" << std::endl;
	}
	catch (...) {
		std::cout << test_name << "???" << std::endl;
		return_code = EXIT_FAILURE;
	}
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	ExceptionGeneralX_3: Complete test.
	//	//////////////////////////////////////////////////////////////////////
	try {
		test_name = "A: Threw ExceptionGeneralX_3, caught ";
		throw ExceptionGeneralX_3("I'm an ExceptionGeneralX_3 object");
	}
	catch (const ExceptionGeneralX_3 &except) {
		std::cout << test_name << "ExceptionGeneralX_3" <<
			" (" << except.what() << ")" << std::endl;
	}
	catch (const ExceptionGeneralX_2 &except) {
		std::cout << test_name << "ExceptionGeneralX_2" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (const ExceptionGeneralX_1 &except) {
		std::cout << test_name << "ExceptionGeneralX_1" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (const std::exception &except) {
		std::cout << test_name << "std::exception" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (...) {
		std::cout << test_name << "???" << std::endl;
		return_code = EXIT_FAILURE;
	}
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	ExceptionGeneralX_3: Fall-through to ExceptionGeneralX_2 test.
	//	//////////////////////////////////////////////////////////////////////
	try {
		test_name = "B: Threw ExceptionGeneralX_3, caught ";
		throw ExceptionGeneralX_3("I'm an ExceptionGeneralX_3 object");
	}
	catch (const ExceptionGeneralX_2 &except) {
		std::cout << test_name << "ExceptionGeneralX_2" <<
			" (" << except.what() << ")" << std::endl;
	}
	catch (const ExceptionGeneralX_1 &except) {
		std::cout << test_name << "ExceptionGeneralX_1" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (const std::exception &except) {
		std::cout << test_name << "std::exception" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (...) {
		std::cout << test_name << "???" << std::endl;
		return_code = EXIT_FAILURE;
	}
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	ExceptionGeneralX_3: Fall-through to ExceptionGeneralX_1 test.
	//	//////////////////////////////////////////////////////////////////////
	try {
		test_name = "C: Threw ExceptionGeneralX_3, caught ";
		throw ExceptionGeneralX_3("I'm an ExceptionGeneralX_3 object");
	}
	catch (const ExceptionGeneralX_1 &except) {
		std::cout << test_name << "ExceptionGeneralX_1" <<
			" (" << except.what() << ")" << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << test_name << "std::exception" <<
			" (" << except.what() << ")" << std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (...) {
		std::cout << test_name << "???" << std::endl;
		return_code = EXIT_FAILURE;
	}
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	ExceptionGeneralX_3: Fall-through to std::exception test.
	//	//////////////////////////////////////////////////////////////////////
	try {
		test_name = "D: Threw ExceptionGeneralX_3, caught ";
		throw ExceptionGeneralX_3("I'm an ExceptionGeneralX_3 object");
	}
	catch (const std::exception &except) {
		std::cout << test_name << "std::exception" <<
			" (" << except.what() << ")" << std::endl;
	}
	catch (...) {
		std::cout << test_name << "???" << std::endl;
		return_code = EXIT_FAILURE;
	}
	//	//////////////////////////////////////////////////////////////////////

	std::cout << PadLeft("", 79, '=') << std::endl << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		if (TEST_Rethrow() != EXIT_SUCCESS)
			return_code = EXIT_FAILURE;
		if (TEST_VirtualBases() != EXIT_SUCCESS)
			return_code = EXIT_FAILURE;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}
	catch (...) {
		std::cerr << std::endl << std::endl << "ERROR: (*UNSPECIFIED*)" <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cerr << "Rethrow exception tests completed with" <<
		((return_code == EXIT_SUCCESS) ? "out error." : " errors.") << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

