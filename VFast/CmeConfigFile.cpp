// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Logic for the management of CME configuration files.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/CmeConfigFile.hpp>

#include <XercesUtils/XercesContext.hpp>

#include <Utility/ParseNumericString.hpp>
#include <Utility/ReplaceChars.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

namespace {
// ////////////////////////////////////////////////////////////////////////////
void CheckExpectedElement(const MLB::XercesUtils::XmlDomElement &xml_ele,
	const char *expected_element_name)
{
	if (!xml_ele.IsElement(expected_element_name))
		MLB::Utility::ThrowLogicError("Expected an element named '" +
			std::string(expected_element_name) + "', but the actual element name "
			"is '" + xml_ele.element_name_ + "'.");
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
CmeCfgProduct::CmeCfgProduct(const std::string &in_code)
	:code_(in_code)
	,group_set_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeCfgProduct::CmeCfgProduct(const MLB::XercesUtils::XmlDomElement &xml_ele)
	:code_()
	,group_set_()
{
	using namespace MLB::XercesUtils;

	CheckExpectedElement(xml_ele, "product");

	code_ = xml_ele.GetAttributeValue("code", true);

	XmlDomElementListIterC iter_b(xml_ele.child_list_.begin());
	XmlDomElementListIterC iter_e(xml_ele.child_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->IsElement("group"))
			group_set_.insert(iter_b->GetAttributeValue("code", true));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CmeCfgProduct::operator < (const CmeCfgProduct &other) const
{
	return(code_ < other.code_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CmeCfgProduct::EmitDatum() const
{
	std::ostringstream o_str;

	o_str << "product=" << code_ << ", groups=(";

	std::set<std::string>::const_iterator iter_b(group_set_.begin());
	std::set<std::string>::const_iterator iter_e(group_set_.end());

	for ( ; iter_b != iter_e; ++iter_b)
		o_str << ((iter_b != group_set_.begin()) ? ", " : "") << *iter_b;

	o_str << ")";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeCfgProduct &datum)
{
	o_str
		<< datum.EmitDatum()
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeCfgConn::CmeCfgConn()
	:type_()
	,protocol_()
	,ip_()
	,port_()
	,feed_()
	,host_ip_list_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeCfgConn::CmeCfgConn(const MLB::XercesUtils::XmlDomElement &xml_ele)
	:type_()
	,protocol_()
	,ip_()
	,port_()
	,feed_()
	,host_ip_list_()
{
	using namespace MLB::XercesUtils;

	CheckExpectedElement(xml_ele, "connection");

	XmlDomElementListIterC iter_b(xml_ele.child_list_.begin());
	XmlDomElementListIterC iter_e(xml_ele.child_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->IsElement("type")) {
			type_ = iter_b->GetAttributeValue("feed-type", true);
		}
		else if (iter_b->IsElement("protocol")) {
			if ((!iter_b->child_list_.empty()) &&
				(iter_b->child_list_.front().IsElement("#text")))
				protocol_ = iter_b->child_list_.front().GetNodeTextNormal();
		}
		else if (iter_b->IsElement("ip")) {
			if ((!iter_b->child_list_.empty()) &&
				(iter_b->child_list_.front().IsElement("#text")))
				ip_ = iter_b->child_list_.front().GetNodeTextNormal();
		}
		else if (iter_b->IsElement("host-ip")) {
			if ((!iter_b->child_list_.empty()) &&
				(iter_b->child_list_.front().IsElement("#text")))
				host_ip_list_.
					push_back(iter_b->child_list_.front().GetNodeTextNormal());
		}
		else if (iter_b->IsElement("port")) {
			if ((!iter_b->child_list_.empty()) &&
				(iter_b->child_list_.front().IsElement("#text")))
				port_ = iter_b->child_list_.front().GetNodeTextNormal();
		}
		else if (iter_b->IsElement("feed")) {
			if ((!iter_b->child_list_.empty()) &&
				(iter_b->child_list_.front().IsElement("#text")))
				feed_ = iter_b->child_list_.front().GetNodeTextNormal();
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CmeCfgConn::operator < (const CmeCfgConn &other) const
{
	if (type_ < other.type_)
		return(true);
	else if ((type_ == other.type_) && (feed_ < other.feed_))
		return(true);

	return(false);		
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CmeCfgConn::EmitDatum() const
{
	std::ostringstream o_str;

	o_str
		<< *this << ":host-ips=(";

	std::vector<std::string>::const_iterator iter_b(host_ip_list_.begin());
	std::vector<std::string>::const_iterator iter_e(host_ip_list_.end());

	for ( ; iter_b != iter_e; ++iter_b)
		o_str << ((iter_b != host_ip_list_.begin()) ? ", " : "") << *iter_b;

	o_str << ")";
		
	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CmeCfgConn::ToString() const
{
	std::ostringstream o_str;

	o_str
		<< type_ << ":"
		<< protocol_ << ":"
		<< ip_ << ":"
		<< port_ << ":"
		<< feed_
			;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *CmeCfgConn::FeedTypeToName(const char *feed_type)
{
	if (!::strcmp(feed_type, "I"))
		return("Incremental");
	else if (!::strcmp(feed_type, "S"))
		return("Snapshot");
	else if (!::strcmp(feed_type, "N"))
		return("Instrument Replay");
	else if (!::strcmp(feed_type, "H"))
		return("Historical Replay");

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeCfgConn &datum)
{
	o_str
		<< datum.type_ << ":"
		<< datum.protocol_ << ":"
		<< std::left << std::setw(3 + 1 + 3 + 1 + 3 + 1 + 3) <<
			datum.ip_ << std::right << ":"
		<< std::setw(5) << datum.port_ << ":"
		<< datum.feed_
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeCfgChannel::CmeCfgChannel(unsigned int channel_id)
	:id_(channel_id)
	,label_()
	,product_set_()
	,connection_set_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeCfgChannel::CmeCfgChannel(const MLB::XercesUtils::XmlDomElement &xml_ele)
	:id_(0)
	,label_()
	,product_set_()
	,connection_set_()
{
	using namespace MLB::XercesUtils;

	CheckExpectedElement(xml_ele, "channel");

	MLB::Utility::ParseNumericString(xml_ele.GetAttributeValue("id", true),
		id_, true);

	label_ = xml_ele.GetAttributeValue("label", true);

	XmlDomElementListIterC iter_b(xml_ele.child_list_.begin());
	XmlDomElementListIterC iter_e(xml_ele.child_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->IsElement("products")) {
			XmlDomElementListIterC iter_b_2(iter_b->child_list_.begin());
			XmlDomElementListIterC iter_e_2(iter_b->child_list_.end());
			for ( ; iter_b_2 != iter_e_2; ++iter_b_2) {
				if (iter_b_2->IsElement("product"))
					product_set_.insert(CmeCfgProduct(*iter_b_2));
			}
		}
		else if (iter_b->IsElement("connections")) {
			XmlDomElementListIterC iter_b_2(iter_b->child_list_.begin());
			XmlDomElementListIterC iter_e_2(iter_b->child_list_.end());
			for ( ; iter_b_2 != iter_e_2; ++iter_b_2) {
				if (iter_b_2->IsElement("connection"))
					connection_set_.insert(CmeCfgConn(*iter_b_2));
			}
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CmeCfgChannel::operator < (const CmeCfgChannel &other) const
{
	return(id_ < other.id_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmeCfgProduct *CmeCfgChannel::GetProductPtr(
	const std::string &product_code) const
{
	CmeCfgProductSetIterC iter_f(product_set_.find(CmeCfgProduct(product_code)));

	return((iter_f != product_set_.end()) ? &(*iter_f) : NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmeCfgProduct &CmeCfgChannel::GetProductRef(
	const std::string &product_code) const
{
	const CmeCfgProduct *product_ptr = GetProductPtr(product_code);

	if (product_ptr == NULL)
		MLB::Utility::ThrowLogicError("Unable to locate product code '" +
			product_code + "' in channel id " + MLB::Utility::AnyToString(id_) +
			" ('" + label_ + "').");

	return(*product_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CmeCfgChannel::EmitDatum() const
{
	std::ostringstream o_str;

	o_str
		<< *this << std::endl;

	{
		CmeCfgProductSetIterC iter_b(product_set_.begin());
		CmeCfgProductSetIterC iter_e(product_set_.end());

		for ( ; iter_b != iter_e; ++iter_b)
			o_str
				<< "   " << iter_b->EmitDatum() << std::endl;
	}

	{
		CmeCfgConnSetIterC iter_b(connection_set_.begin());
		CmeCfgConnSetIterC iter_e(connection_set_.end());

		for ( ; iter_b != iter_e; ++iter_b)
			o_str
				<< "   " << iter_b->EmitDatum() << std::endl;
	}

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeCfgChannel &datum)
{
	o_str
		<< "Channel=" << datum.id_ << ", label=" << datum.label_
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeCfgProductKey::CmeCfgProductKey(unsigned int channel_id,
	const std::string &security_group, const std::string &symbol)
	:channel_id_(channel_id)
	,security_group_(security_group)
	,symbol_(symbol)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CmeCfgProductKey::operator < (const CmeCfgProductKey &other) const
{
	if (channel_id_ < other.channel_id_)
		return(true);
	else if (channel_id_ == other.channel_id_) {
		if (security_group_ < other.security_group_)
			return(true);
		else if (security_group_ == other.security_group_) {
			if (symbol_ < other.symbol_)
				return(true);
		}
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeCfgProductKey &datum)
{
	o_str
		<< std::setw(5) << datum.channel_id_     << " "
		<< std::left
		<< std::setw(5) << datum.security_group_ << " "
		<< std::setw(5) << datum.symbol_
		<< std::right
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeConfigFile::CmeConfigFile(const std::string &xml_file_name)
	:file_name_(xml_file_name)
	,environment_()
	,updated_()
	,channel_set_()
{
	try {
		MLB::XercesUtils::XercesContext xerces_context;
		ConstructInstance(
			MLB::XercesUtils::XmlDomElement::ParseXmlFile(xml_file_name));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Failed to parse CME XML configuration "
			"file '" + xml_file_name + "': " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmeCfgChannel &CmeConfigFile::GetChannelRefById(
	unsigned int channel) const
{
	CmeCfgChannelSetIterC iter_f(channel_set_.find(CmeCfgChannel(channel)));

	if (iter_f == channel_set_.end())
		MLB::Utility::ThrowLogicError("Unable to locate channel id number " +
			MLB::Utility::AnyToString(channel) + ".");

	return(*iter_f);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmeCfgChannel &CmeConfigFile::GetChannelRefByName(
	const char *channel) const
{
	CmeCfgChannelSetIterC iter_b(channel_set_.begin());
	CmeCfgChannelSetIterC iter_e(channel_set_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (!MLB::Utility::Utility_stricmp(channel, iter_b->label_.c_str()))
			break;
	}

	if (iter_b == iter_e)
		MLB::Utility::ThrowLogicError("Unable to locate channel name '" +
			std::string(channel) + "'.");

	return(*iter_b);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmeCfgChannel &CmeConfigFile::GetChannelRefByName(
	const std::string &channel) const
{
	return(GetChannelRefByName(channel.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmeCfgChannel &CmeConfigFile::GetChannelRef(const char *channel) const
{
	unsigned int tmp_uint;

	MLB::Utility::ParseNumericString(channel, tmp_uint, false);

	return((MLB::Utility::ParseNumericString(channel, tmp_uint, false)) ?
		GetChannelRefById(tmp_uint) : GetChannelRefByName(channel));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmeCfgChannel &CmeConfigFile::GetChannelRef(
	const std::string &channel) const
{
	return(GetChannelRef(channel.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const CmeCfgChannel &CmeConfigFile::GetChannelRefByIdAndProduct(
	unsigned int channel, const std::string &product_code) const
{
	const CmeCfgChannel &channel_ref(GetChannelRefById(channel));

	channel_ref.GetProductRef(product_code);

	return(channel_ref);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ProductMap &CmeConfigFile::GetProductMap(ProductMap &prod_map) const
{
	CmeCfgChannelSetIterC iter_chan_b(channel_set_.begin());
	CmeCfgChannelSetIterC iter_chan_e(channel_set_.end());
	ProductMap            tmp_prod_map;

	for ( ; iter_chan_b != iter_chan_e; ++iter_chan_b) {
		CmeCfgProductSetIterC  iter_prod_b(iter_chan_b->product_set_.begin());
		CmeCfgProductSetIterC  iter_prod_e(iter_chan_b->product_set_.end());
		for ( ; iter_prod_b != iter_prod_e; ++iter_prod_b) {
			std::set<std::string>::const_iterator iter_group_b(
				iter_prod_b->group_set_.begin());
			std::set<std::string>::const_iterator iter_group_e(
				iter_prod_b->group_set_.end());
			for ( ; iter_group_b != iter_group_e; ++iter_group_b) {
				CmeCfgProductKey prod_key(iter_chan_b->id_, iter_prod_b->code_,
					*iter_group_b);
				ProductMapIterC  iter_f(tmp_prod_map.find(prod_key));
				if (iter_f != tmp_prod_map.end()) {
					std::ostringstream o_str;
					o_str << "Product key '" << prod_key << "' was located "
						"in both '" << *iter_f->second << "' and '" <<
						*iter_chan_b << "'.";
					MLB::Utility::ThrowLogicError(o_str);
				}
				tmp_prod_map[prod_key] = &(*iter_chan_b);
			}
		}
	}

	prod_map.swap(tmp_prod_map);

	return(prod_map);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ProductMap CmeConfigFile::GetProductMap() const
{
	ProductMap tmp_prod_map;

	return(GetProductMap(tmp_prod_map));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::string &CmeConfigFile::CheckEnvName(
	const std::string &env_name) const
{
	if (MLB::Utility::Utility_stricmp(env_name.c_str(), environment_.c_str()))
		MLB::Utility::ThrowInvalidArgument("The specified CME environment "
			"name ('" + env_name + "') is not the same as that loaded from the "
			"CME configuration file '" + file_name_ + "' ('" + environment_ +
			"').");

	return(environment_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CmeConfigFile::CheckChannelIdSet(
	const std::set<unsigned int> channel_id_set) const
{
	std::set<unsigned int>::const_iterator iter_b(channel_id_set.begin());
	std::set<unsigned int>::const_iterator iter_e(channel_id_set.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		try {
			GetChannelRefById(*iter_b);
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "At least one CME channel identifier "
				"in the provided set of " +
				MLB::Utility::AnyToString(channel_id_set.size()) + " is invalid: "+
				std::string(except.what()));
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string CmeConfigFile::EmitDatum() const
{
	std::ostringstream o_str;

	o_str
		<< *this << std::endl;

	CmeCfgChannelSetIterC iter_b(channel_set_.begin());
	CmeCfgChannelSetIterC iter_e(channel_set_.end());

	for ( ; iter_b != iter_e; ++iter_b)
		o_str << iter_b->EmitDatum() << std::endl;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CmeConfigFile::ConstructInstance(
	const MLB::XercesUtils::XmlDomElement &xml_ele)
{
	using namespace MLB::XercesUtils;

	CheckExpectedElement(xml_ele, "configuration");

	environment_ = xml_ele.GetAttributeValue("environment", false);
	updated_     = xml_ele.GetAttributeValue("updated", false);

	XmlDomElementListIterC iter_b(xml_ele.child_list_.begin());
	XmlDomElementListIterC iter_e(xml_ele.child_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->IsElement("channel"))
			channel_set_.insert(CmeCfgChannel(*iter_b));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeConfigFile &datum)
{
	o_str
		<< "Environment=" << datum.environment_ << ", updated=" <<
			datum.updated_
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/StringSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"<CME-config-file> [ <CME-config-file> ... ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	int return_code = EXIT_SUCCESS;

	using namespace MLB::VFast;

	try {
		if (argc < 2)
			MLB::Utility::ThrowInvalidArgument("Expected to find the names of "
				"one or more CME configuration files on the command line "
				"(note: not FAST template files).");
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			CmeConfigFile cme_cfg(argv[count_1]);
			std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
			std::cout << "File: " << argv[count_1] << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			std::cout << cme_cfg.EmitDatum() << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			ProductMap      prod_map(cme_cfg.GetProductMap());
			ProductMapIterC iter_b(prod_map.begin());
			ProductMapIterC iter_e(prod_map.end());
			for ( ; iter_b != iter_e; ++iter_b)
				std::cout << std::setw(6) << iter_b->first << ": " <<
					*iter_b->second << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
			std::cout << std::endl;
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN


/*
	Testing command line for Windows:
	------- ------- ---- --- --------
	C:\DevEnv\Tests\FastCommon\Other\CME\Production.20090623_A.config.xml > C:\DevEnv\Tests\VFast\UnitTests\CmeConfigFile.txt
*/

