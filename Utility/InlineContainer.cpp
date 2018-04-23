//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
// C++ Support Library Test Harness Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Test harness module for template class InlineContainer.

								From an article published in _C/C++ Users Journal_,
								Vol 16, No 9, September 1998 by Andrei Alexandrescu,
								pp. 73-74.

	Revision History	:	1998-10-07 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files . . .
//	////////////////////////////////////////////////////////////////////////////

#include "Utility/InlineContainer.hpp"

#include <iterator>

//	////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

#include <cstdlib>

using namespace MLB::Utility;

//	////////////////////////////////////////////////////////////////////////////
static void TEST_Deque(void);
static void TEST_List();
static void TEST_Vector();
template <class ContainerType>
	static void TEST_OutputContainer(const char *container_name,
	const ContainerType &the_cont);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	TEST_Deque();
	TEST_List();
	TEST_Vector();

	return(EXIT_SUCCESS);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_Deque()
{
	InlineContainer<int, std::deque<int> >       test_cont_int;
	InlineContainer<double, std::deque<double> > test_cont_double;

	TEST_OutputContainer("deque<int>", MakeInlineDeque(1)(2)(3)(7)(8)(9));
	test_cont_int = MakeInlineDeque(1)(2)(3)(7)(8)(9);
	TEST_OutputContainer("deque<int>", test_cont_int);

	TEST_OutputContainer("deque<double>",
		MakeInlineDeque(1.1)(2.2)(3.3)(7.7)(8.7)(9.7));
	test_cont_double = MakeInlineDeque(1.1)(2.2)(3.3)(7.7)(8.7)(9.7);
	TEST_OutputContainer("deque<double>", test_cont_double);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_List()
{
	InlineContainer<int, std::list<int> >       test_cont_int;
	InlineContainer<double, std::list<double> > test_cont_double;

	TEST_OutputContainer("list<int>", MakeInlineList(1)(2)(3)(7)(8)(9));
	test_cont_int = MakeInlineList(1)(2)(3)(7)(8)(9);
	TEST_OutputContainer("list<int>", test_cont_int);

	TEST_OutputContainer("list<double>",
		MakeInlineList(1.1)(2.2)(3.3)(7.7)(8.7)(9.7));
	test_cont_double = MakeInlineList(1.1)(2.2)(3.3)(7.7)(8.7)(9.7);
	TEST_OutputContainer("list<double>", test_cont_double);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
static void TEST_Vector()
{
	InlineContainer<int, std::vector<int> >       test_cont_int;
	InlineContainer<double, std::vector<double> > test_cont_double;

	TEST_OutputContainer("vector<int>", MakeInlineVector(1)(2)(3)(7)(8)(9));
	test_cont_int = MakeInlineVector(1)(2)(3)(7)(8)(9);
	TEST_OutputContainer("vector<int>", test_cont_int);

	TEST_OutputContainer("vector<double>",
		MakeInlineVector(1.1)(2.2)(3.3)(7.7)(8.7)(9.7));
	test_cont_double = MakeInlineVector(1.1)(2.2)(3.3)(7.7)(8.7)(9.7);
	TEST_OutputContainer("vector<double>", test_cont_double);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <class ContainerType>
	static void TEST_OutputContainer(const char *container_name,
	const ContainerType &the_cont)
{
	std::cout << container_name << std::endl;
	std::cout << std::string(10, '-') << std::endl;
	std::copy(the_cont.begin(), the_cont.end(),
		std::ostream_iterator<typename ContainerType::value_type>(
		std::cout, "\n"));
	std::cout << std::string(10, '-') << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

