// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for managing CME configuration files.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__CmeConfigFile_hpp__HH

#define HH__MLB__VFast__CmeConfigFile_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file CmeConfigFile.hpp

	\brief VFast CME-specific support.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

#include <XercesUtils/XmlDomElement.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
struct CmeCfgProduct {
	CmeCfgProduct(const std::string &in_code = "");
	CmeCfgProduct(const MLB::XercesUtils::XmlDomElement &xml_ele);

	bool operator < (const CmeCfgProduct &other) const;

	std::string EmitDatum() const;

	/**
		Corresponds to the CME field \e 1151 - \e SecurityGroup .
	*/
	std::string           code_;

	/**
		Correspond to the CME field \e 55 - \e Symbol .
	*/
	std::set<std::string> group_set_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<CmeCfgProduct>          CmeCfgProductSet;
typedef CmeCfgProductSet::iterator       CmeCfgProductSetIter;
typedef CmeCfgProductSet::const_iterator CmeCfgProductSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeCfgProduct &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct CmeCfgConn {
	CmeCfgConn();
	CmeCfgConn(const MLB::XercesUtils::XmlDomElement &xml_ele);

	bool operator < (const CmeCfgConn &other) const;

	std::string EmitDatum() const;
	std::string ToString() const;

	std::string              type_;
	std::string              protocol_;
	std::string              ip_;
	std::string              port_;
	std::string              feed_;
	std::vector<std::string> host_ip_list_;

	static const char *FeedTypeToName(const char *feed_type);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<CmeCfgConn>          CmeCfgConnSet;
typedef CmeCfgConnSet::iterator       CmeCfgConnSetIter;
typedef CmeCfgConnSet::const_iterator CmeCfgConnSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeCfgConn &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct CmeCfgChannel {
	CmeCfgChannel(unsigned int channel_id = 0);
	CmeCfgChannel(const MLB::XercesUtils::XmlDomElement &xml_ele);

	bool operator < (const CmeCfgChannel &other) const;

	const CmeCfgProduct *GetProductPtr(const std::string &product_code) const;
	const CmeCfgProduct &GetProductRef(const std::string &product_code) const;

	std::string EmitDatum() const;

	unsigned int     id_;
	std::string      label_;
	CmeCfgProductSet product_set_;
	CmeCfgConnSet    connection_set_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<CmeCfgChannel>          CmeCfgChannelSet;
typedef CmeCfgChannelSet::iterator       CmeCfgChannelSetIter;
typedef CmeCfgChannelSet::const_iterator CmeCfgChannelSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeCfgChannel &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct CmeCfgProductKey {
	CmeCfgProductKey(unsigned int channel_id = 0,
		const std::string &security_group = "", const std::string &symbol = "");

	bool operator < (const CmeCfgProductKey &other) const;

	unsigned int channel_id_;
	std::string  security_group_;
	std::string  symbol_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeCfgProductKey &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::map<CmeCfgProductKey, const CmeCfgChannel *> ProductMap;
typedef ProductMap::iterator                              ProductMapIter;
typedef ProductMap::const_iterator                        ProductMapIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct CmeConfigFile {
	explicit CmeConfigFile(const std::string &xml_file_name);

	const CmeCfgChannel &GetChannelRefById(unsigned int channel) const;
	const CmeCfgChannel &GetChannelRefByName(const char *channel) const;
	const CmeCfgChannel &GetChannelRefByName(const std::string &channel) const;
	const CmeCfgChannel &GetChannelRef(const char *channel) const;
	const CmeCfgChannel &GetChannelRef(const std::string &channel) const;
	const CmeCfgChannel &GetChannelRefByIdAndProduct(unsigned int channel,
		const std::string &product_code) const;

	ProductMap &GetProductMap(ProductMap &prod_map) const;
	ProductMap  GetProductMap() const;

	const std::string &CheckEnvName(const std::string &env_name) const;
	      void         CheckChannelIdSet(
		const std::set<unsigned int> channel_id_set) const;

	std::string EmitDatum() const;

	std::string      file_name_;
	std::string      environment_;
	std::string      updated_;
	CmeCfgChannelSet channel_set_;

private:
	void ConstructInstance(const MLB::XercesUtils::XmlDomElement &xml_ele);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const CmeConfigFile &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__CmeConfigFile_hpp__HH

