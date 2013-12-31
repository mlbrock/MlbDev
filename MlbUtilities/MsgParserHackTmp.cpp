// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ExcContext class.

	Revision History	:	2008-11-28 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/ExcContext.hpp>
#include <VFast/VFastException.hpp>

// ////////////////////////////////////////////////////////////////////////////

#include <Utility/HRTimer.hpp>

#include <boost/shared_ptr.hpp>

using namespace MLB::VFast;

namespace {
// ////////////////////////////////////////////////////////////////////////////
struct FastFixTestPacket {
   unsigned short       packet_length_;
   const unsigned char *packet_data_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	const FastFixTestPacket TestMe = {    50, reinterpret_cast<const unsigned char *>("\xC0\xB9\x08\x32\x69\xB0\x23\x55\x6F\x6B\x4B\x06\x3E\xF1\x09\x49\x52\x9D\x82\x6D\xC2\x80\x80\x80\x18\x69\xD0\x13\x72\xBF\x05\x0C\x8E\x3F\x7A\x79\xD8\x80\x80\x80\xC3\x30\xC0\x80\xCE\x80\x80\x80\x80\x80") };
//	const FastFixTestPacket TestMe = {	  55, reinterpret_cast<const unsigned char *>("\xc0\xee\x88\x84\x00\x0d\x13\x47\x85\x0a\x39\xa7\xe5\xfa\x83\x00\x01\x45\xc0\xf8\xff\xd6\x01\x23\x4d\x1f\x4a\xab\xbf\x23\x1c\xce\x23\x1c\xcf\x82\x01\x60\xf0\x10\x48\x00\x86\x81\xb2\xb2\x83\xff\x01\x27\x98\x07\xcf\xb1\xe9") };
//	const FastFixTestPacket TestMe = {	  71, reinterpret_cast<const unsigned char *>("\xc0\xee\x88\x84\x00\x0c\x8b\x16\x85\x0a\x00\x44\x1c\xe5\x83\x00\x00\x00\xc0\xf8\xf2\xd9\x14\x3d\x5b\x98\xbf\x10\x48\x00\x86\x01\x20\x02\x10\x37\xa5\x01\x20\x02\x10\x36\xcd\xb4\xa0\xb4\xce\xd3\xff\x01\x27\x98\x86\x28\x32\x84\x81\x81\x01\x58\xc9\x01\x58\xc9\x81\x30\x30\x35\x53\x58\xb9") };
	const FastFixTestPacket TestMe = {	  54, reinterpret_cast<const unsigned char *>("\xc0\xee\x88\x84\x00\x04\x63\x91\x85\x08\x45\xb1\x98\x72\x83\x00\x01\x67\xc0\xf8\xff\xd6\x01\x04\x2d\x46\x2e\xf4\xbf\x0b\x03\x99\x0b\x03\x9a\x82\x48\xa8\x10\x48\x00\x86\x81\xb2\xb2\x84\xff\x01\x26\xe6\x07\x9e\xb1\xfa") };
// ////////////////////////////////////////////////////////////////////////////
} // namespace

// ////////////////////////////////////////////////////////////////////////////
class M_B1 {
//	int i_b1_;
};
class M_B2 {
//	int i_b2_;
};
class M_D : public M_B1, public M_B2 {
};
void TEST_Me_1()
{
	M_D d;

	std::cout
		<< "TEST 1: "
		<< "D : " << &d << " --- "
		<< "B1: " << static_cast<M_B1 *>(&d) << " --- "
		<< "B2: " << static_cast<M_B2 *>(&d) << std::endl;

	std::cout
		<< "TEST 2: "
		<< "D : " << &d << " --- "
		<< "B1: " << dynamic_cast<M_B1 *>(&d) << " --- "
		<< "B2: " << dynamic_cast<M_B2 *>(&d) << std::endl;

	boost::shared_ptr<M_D> d_sptr(new M_D());

	std::cout
		<< "TEST 3: "
		<< "D : " << d_sptr.get() << " --- "
		<< "B1: " << boost::static_pointer_cast<M_B1>(d_sptr).get() << " --- "
		<< "B2: " << boost::static_pointer_cast<M_B2>(d_sptr).get() << std::endl;

	std::cout << "==========================================================" <<
		std::endl << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef struct {
    union {
        struct {
            unsigned PriceFactor        :1;
            unsigned ChannelID          :1;
            unsigned SegmentID          :1;
            unsigned SectorID           :1;

            unsigned InstrumentName     :1;
            unsigned InstrumentIndex    :1;
            unsigned ActionType         :1;
            unsigned TradingStatus      :1;

            unsigned Currency           :1;
            unsigned TickSize           :1;
            unsigned TickSizeAboveLimit :1;
            unsigned TickSizeLimit      :1;
            unsigned ReferencePrice     :1;
            unsigned PriceType          :1;
            unsigned ReferenceDate      :1;
            unsigned RoundLotSize       :1;

            unsigned LowLimit           :1;
            unsigned HighLimit          :1;
            unsigned MinTradeVolume     :1;
            unsigned MaxTradeVolume     :1;

            unsigned MaxPriceVariation  :1;
            unsigned MinLotSize         :1;
            unsigned MinCabPrice        :1;
            unsigned ImpliedPrices      :1;

            unsigned DealPriceFactor    :1;
            unsigned TradePriceFactor   :1;

            unsigned UnitQuantity       :1;
            unsigned Unit               :1;

            unsigned InstrumentContext  :1;

            unsigned OrderBookDepth     :1;
        } PresenceMap;
        unsigned int PresenceMapAll;
    };
} Has;
typedef struct {
} HasNot;
template <typename T> class Answer {
	typedef char one;
	typedef struct { char x[2]; } two;

	template <typename C>
		static one test( char[sizeof(&C::PresenceMap)] );
	template <typename C>
		static two test(...);

public:
	enum { value = sizeof(test<T>(0)) == sizeof(char) };
}; 
void TEST_Me_2()
{
	std::cout << "Has   : " << Answer<Has>::value    << std::endl;
	std::cout << "HasNot: " << Answer<HasNot>::value << std::endl;

	std::cout << "==========================================================" <<
		std::endl << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
TEST_Me_1();
TEST_Me_2();
		if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
			std::cout << "USAGE: " << std::endl <<
				"   " << argv[0] << " <XML-template-file>" <<
				std::endl << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (argc != 2)
			MLB::Utility::ThrowInvalidArgument("Expected a single argument of "
				"the name of an Fix/FAST XML template file. Use '-h' for help.");
		InsContext ins_context(argv[1]);
		ExcContext exc_context(ins_context, ExcContextInfo(true));
		//	----------------------------------------------------------------
		const unsigned char *begin_ptr = TestMe.packet_data_;
		const unsigned char *end_ptr   = begin_ptr + TestMe.packet_length_;
		while (begin_ptr < end_ptr) {
			try {
				begin_ptr = exc_context.FieldDecode(begin_ptr, end_ptr);
			}
			catch (const std::exception &except) {
				std::cout << "DECODE ERROR: " << except.what() << std::endl;
				return_code = EXIT_FAILURE;
			}
			unsigned int    template_id = exc_context.GetLastTemplateId();
			std::size_t     entry_count = exc_context.GetEntryCount();
			const ExcEntry *entry_list  = exc_context.GetEntryList();
			ExcEntry::EmitListDebug(entry_count, entry_list,
				exc_context.GetInsItemList());
			if (template_id == 120)
				exc_context.ResetDictionaries();
			std::cout << std::endl;
		}
		exc_context.ResetLastTemplateDictionary();
		//	----------------
		unsigned int count_1;
		unsigned int iteration_count = 1000000;
		MLB::Utility::HRTimer hr_timer;
		hr_timer.Start();
		for (count_1 = 0; count_1 < iteration_count; ++count_1) {
			exc_context.FieldDecode(TestMe.packet_data_,
				TestMe.packet_data_ + TestMe.packet_length_);
			exc_context.ResetLastTemplateDictionary();
		}
		hr_timer.End();
		std::cout << "Elapsed time    = " << std::setprecision(16) <<
			hr_timer.GetTickDiffInSeconds() << std::endl;
		std::cout << "Time per packet = " <<
			(hr_timer.GetTickDiffInSeconds() /
			static_cast<double>(iteration_count)) << std::endl;
		//	----------------------------------------------------------------
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

//\xc0\xee\x88\x84\x00\x04\x63\x91\x85\x08\x45\xb1\x98\x72\x83\x00\x01\x67\xc0\xf8\xff\xd6\x01\x04\x2d\x46\x2e\xf4\xbf\x0b\x03\x99\x0b\x03\x9a\x82\x48\xa8\x10\x48\x00\x86\x81\xb2\xb2\x84\xff\x01\x26\xe6\x07\x9e\xb1\xfa

