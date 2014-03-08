// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the portable Ethernet address class.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/MACAddress.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef _Windows
# include <deque>
# include <wincon.h>
# include <nb30.h>
#elif __MSDOS__
# include <memory.h>
#elif defined(__linux__)
# include <memory.h>
# include <netinet/ether.h>
#else
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

#ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////
//	snmpmac.cpp
//	Get MAC Address, SNMP Method
/*
James Marsh (James.Marsh at sandtechnology dot com) writes:

In my search for an ironclad method of finding the MAC address of the ethernet
card of a system, I attempted the NetBIOS method as described in the FAQ, but
found this to be unacceptable as some of our customers reportedly didn't use
[NetBIOS]. I passed over the RPC method, and found a solution in a page
maintained by Stas Khirman and Raz Galili.... They describe a method of using
the internal SNMP calls in Windows (NT, 95, and 2000 have worked for me) that
are used by netstat and other windows utilities, but unfortunately provide no
code example for finding the MAC code.

[Below is] a small program I've written up, so that hopefully other people
won't have to go through the same learning curve I did with SNMP. I will
also attach a file, snmpapi.cpp, that was written by Stas Khirman and
distributed on his web page.

A great resource for those interested in retrieving system info through this
method is http://www.alvestrand.no/objectid/top.html.
*/

#if !defined(__MINGW32__)
# pragma warning(disable:4668)
#endif // #if !defined(__MINGW32__)
#include <snmp.h>
#if !defined(__MINGW32__)
# pragma warning(default:4668)
#endif // #if !defined(__MINGW32__)
#include <conio.h>
#include <stdio.h>

typedef BOOL(WINAPI * pSnmpExtensionInit) (
        IN DWORD dwTimeZeroReference,
        OUT HANDLE * hPollForTrapEvent,
        OUT AsnObjectIdentifier * supportedView);

typedef BOOL(WINAPI * pSnmpExtensionTrap) (
        OUT AsnObjectIdentifier * enterprise,
        OUT AsnInteger * genericTrap,
        OUT AsnInteger * specificTrap,
        OUT AsnTimeticks * timeStamp,
        OUT RFC1157VarBindList * variableBindings);

typedef BOOL(WINAPI * pSnmpExtensionQuery) (
        IN BYTE requestType,
        IN OUT RFC1157VarBindList * variableBindings,
        OUT AsnInteger * errorStatus,
        OUT AsnInteger * errorIndex);

typedef BOOL(WINAPI * pSnmpExtensionInitEx) (
        OUT AsnObjectIdentifier * supportedView);

typedef BOOL(WINAPI * pSnmpExtensionInitEx) (
        OUT AsnObjectIdentifier * supportedView);
    
std::deque<MLB::Utility::MACAddress>
	Win32_GetMacAddress_Internal(bool first_only_flag = false)
{
	WSADATA WinsockData;
	if (WSAStartup(MAKEWORD(2, 0), &WinsockData) != 0)
		MLB::Utility::ThrowStdException("This program requires Winsock 2.x.");

    HINSTANCE m_hInst;
    HANDLE PollForTrapEvent;
    AsnObjectIdentifier SupportedView;
    UINT OID_ifEntryType[] = {
        1, 3, 6, 1, 2, 1, 2, 2, 1, 3
    };
    UINT OID_ifEntryNum[] = {
        1, 3, 6, 1, 2, 1, 2, 1
    };
    UINT OID_ipMACEntAddr[] = {
        1, 3, 6, 1, 2, 1, 2, 2, 1, 6
    };                          //, 1 ,6 };
    AsnObjectIdentifier MIB_ifMACEntAddr =
        { sizeof(OID_ipMACEntAddr) / sizeof(UINT), OID_ipMACEntAddr };
    AsnObjectIdentifier MIB_ifEntryType = {
        sizeof(OID_ifEntryType) / sizeof(UINT), OID_ifEntryType
    };
    AsnObjectIdentifier MIB_ifEntryNum = {
        sizeof(OID_ifEntryNum) / sizeof(UINT), OID_ifEntryNum
    };
    RFC1157VarBindList varBindList;
    RFC1157VarBind varBind[2];
    AsnInteger errorStatus;
    AsnInteger errorIndex;
    AsnObjectIdentifier MIB_NULL = {
        0, 0
    };
    int ret;
    int dtmp;
    int j = 0;

   /* Load the SNMP dll and get the addresses of the functions
       necessary */
	m_hInst = LoadLibrary("inetmib1.dll");
	if (m_hInst == NULL)
		MLB::Utility::ThrowSystemError("Unable to load DLL 'inetmib1.dll' with \
'LoadLibrary()'");

#if !defined(__MINGW32__)
# pragma warning(disable:4191)
#endif // #if !defined(__MINGW32__)

	pSnmpExtensionInit m_Init     =
		reinterpret_cast<pSnmpExtensionInit>(GetProcAddress(m_hInst,
		"SnmpExtensionInit"));
	if (m_Init == NULL)
		MLB::Utility::ThrowSystemError("Attempt to invoke 'GetProcAddress()' \
for DLL 'inetmib1.dll', procedure 'SnmpExtensionInit()' failed");
	pSnmpExtensionInitEx m_InitEx =
		reinterpret_cast<pSnmpExtensionInitEx>(GetProcAddress(m_hInst,
		"SnmpExtensionInitEx"));
	if (m_InitEx == NULL)
		MLB::Utility::ThrowSystemError("Attempt to invoke 'GetProcAddress()' \
for DLL 'inetmib1.dll', procedure 'SnmpExtensionInitEx()' failed");
	pSnmpExtensionQuery m_Query   =
		reinterpret_cast<pSnmpExtensionQuery>(GetProcAddress(m_hInst,
		"SnmpExtensionQuery"));
	if (m_Query == NULL)
		MLB::Utility::ThrowSystemError("Attempt to invoke 'GetProcAddress()' \
for DLL 'inetmib1.dll', procedure 'SnmpExtensionQuery()' failed");
	pSnmpExtensionTrap  m_Trap    =
		reinterpret_cast<pSnmpExtensionTrap>(GetProcAddress(m_hInst,
		"SnmpExtensionTrap"));
	if (m_Trap == NULL)
		MLB::Utility::ThrowSystemError("Attempt to invoke 'GetProcAddress()' \
for DLL 'inetmib1.dll', procedure 'SnmpExtensionTrap()' failed");

#if !defined(__MINGW32__)
# pragma warning(default:4191)
#endif // #if !defined(__MINGW32__)

	m_Init(GetTickCount(), &PollForTrapEvent, &SupportedView);

	/* Initialize the variable list to be retrieved by m_Query */
	varBindList.list = varBind;
	varBind[0].name = MIB_NULL;
	varBind[1].name = MIB_NULL;

	/* Copy in the OID to find the number of entries in the
       Inteface table */
	varBindList.len = 1;        /* Only retrieving one item */
	SNMP_oidcpy(&varBind[0].name, &MIB_ifEntryNum);
	ret = m_Query(ASN_RFC1157_GETNEXTREQUEST, &varBindList, &errorStatus,
		&errorIndex);

/*
	DEBUGGING OUTPUT
std::cout << "# of adapters in this system : " <<
	varBind[0].value.asnValue.number << std::endl;
*/

	varBindList.len = 2;

	/* Copy in the OID of ifType, the type of interface */
	SNMP_oidcpy(&varBind[0].name, &MIB_ifEntryType);

	/* Copy in the OID of ifPhysAddress, the address */
	SNMP_oidcpy(&varBind[1].name, &MIB_ifMACEntAddr);

	std::deque<MLB::Utility::MACAddress> out_list;

	do {
        /* Submit the query.  Responses will be loaded into varBindList.
           We can expect this call to succeed a # of times corresponding
           to the # of adapters reported to be in the system */
		ret = m_Query(ASN_RFC1157_GETNEXTREQUEST, &varBindList, &errorStatus,
			&errorIndex);
		if (!ret)
			break;
		/* Confirm that the proper type has been returned */
		ret = SNMP_oidncmp(&varBind[0].name, &MIB_ifEntryType,
			MIB_ifEntryType.idLength);
		if (ret)
			break;

		j++;
		dtmp = varBind[0].value.asnValue.number;
/*
	DEBUGGING OUTPUT
std::cout << "Interface #" << j << " type : " << dtmp << std::endl;
*/
		/* Type 6 describes ethernet interfaces */
		if (dtmp == 6) {
			/* Confirm that we have an address here */
			ret = SNMP_oidncmp(&varBind[1].name, &MIB_ifMACEntAddr,
				MIB_ifMACEntAddr.idLength);
			if ((!ret) && (varBind[1].value.asnValue.address.stream != NULL)) {
				if ((varBind[1].value.asnValue.address.stream[0] == 0x44) &&
					 (varBind[1].value.asnValue.address.stream[1] == 0x45) &&
					 (varBind[1].value.asnValue.address.stream[2] == 0x53) &&
					 (varBind[1].value.asnValue.address.stream[3] == 0x54) &&
					 (varBind[1].value.asnValue.address.stream[4] == 0x00)) {
					/* Ignore all dial-up networking adapters */
/*
	DEBUGGING OUTPUT
std::cout << "Interface #" << j << " is a DUN adapter" << std::endl;
*/
					continue;
				}
				if ((varBind[1].value.asnValue.address.stream[0] == 0x00) &&
					 (varBind[1].value.asnValue.address.stream[1] == 0x00) &&
					 (varBind[1].value.asnValue.address.stream[2] == 0x00) &&
					 (varBind[1].value.asnValue.address.stream[3] == 0x00) &&
					 (varBind[1].value.asnValue.address.stream[4] == 0x00) &&
					 (varBind[1].value.asnValue.address.stream[5] == 0x00)) {
					/* Ignore NULL addresses returned by other network interfaces */
/*
	DEBUGGING OUTPUT
std::cout << "Interface #" << j << " is a NULL address" << std::endl;
*/
					continue;
				}
				MLB::Utility::MACAddress
					tmp_ethernet(varBind[1].value.asnValue.address.stream);
/*
	DEBUGGING OUTPUT
std::cout << "MAC Address of interface #" << j << ": " << tmp_ethernet <<
	std::endl;
*/
				out_list.push_back(tmp_ethernet);
				if (first_only_flag)
					break;
			}
		}
    } while (!ret);         /* Stop only on an error.  An error will occur
                               when we go exhaust the list of interfaces to
                               be examined */
	/* Free the bindings */
	SNMP_FreeVarBind(&varBind[0]);
	SNMP_FreeVarBind(&varBind[1]);

	return(out_list);
}
// ////////////////////////////////////////////////////////////////////////////
#endif // #ifdef _Windows

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
MACAddress::MACAddress()
{
	Clear();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MACAddress::MACAddress(const unsigned char *ethernet_address)
{
	::memcpy(octet, ethernet_address, sizeof(octet));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool MACAddress::operator <  (const MACAddress &other) const
{
	return(Compare(this, &other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool MACAddress::operator >  (const MACAddress &other) const
{
	return(Compare(this, &other) > 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool MACAddress::operator == (const MACAddress &other) const
{
	return(Compare(this, &other) == 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool MACAddress::operator != (const MACAddress &other) const
{
	return(Compare(this, &other) != 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool MACAddress::operator <= (const MACAddress &other) const
{
	return(Compare(this, &other) <= 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool MACAddress::operator >= (const MACAddress &other) const
{
	return(Compare(this, &other) >= 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int MACAddress::Compare(const MACAddress &other) const
{
	return(Compare(this, &other));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MACAddress &MACAddress::SetToMinimumValue()
{
	Clear();

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MACAddress &MACAddress::SetToMaximumValue()
{
	::memset(octet, std::numeric_limits<unsigned char>::max(), sizeof(octet));

	return(*this);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool MACAddress::IsZero() const
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < MACAddress_Length; ++count_1) {
		if (octet[count_1])
			return(false);
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MACAddress::Get()
{
	*this = GetMyMACAddress();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MACAddress::Clear()
{
	::memset(octet, '\0', sizeof(octet));
}
// ////////////////////////////////////////////////////////////////////////////

namespace {
// ////////////////////////////////////////////////////////////////////////////
const char INTERNAL_MACAdress_HexList[] = "0123456789ABCDEF";
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
char *MACAddress::ToString(char *buffer, int separator) const
{
	char         *tmp_ptr = buffer;
	unsigned int  count_1;

	for (count_1 = 0; count_1 < MACAddress_Length; ++count_1) {
		if (count_1)
			*tmp_ptr++ = static_cast<char>(separator);
		*tmp_ptr++ = INTERNAL_MACAdress_HexList[(octet[count_1] >> 4) & 0x0F];
		*tmp_ptr++ = INTERNAL_MACAdress_HexList[octet[count_1]        & 0x0F];
	}

	*tmp_ptr = '\0';

	return(buffer);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string MACAddress::ToString(int separator) const
{
	char buffer[MACAddress_StringLength + 1];

	return(ToString(buffer, separator));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MACAddress::swap(MACAddress &other)
{
	unsigned char tmp_octet[MACAddress_Length];

	::memcpy(tmp_octet, octet, sizeof(tmp_octet));
	::memcpy(octet, other.octet, sizeof(octet));
	::memcpy(other.octet, tmp_octet, sizeof(other.octet));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MACAddress MACAddress::GetMyMACAddress()
{
#ifdef __MSDOS__
	return(MACAddress());
#elif _Windows
/*
	//	The old Win32 NetBIOS code has been obsoleted by the SNMP
	//	implementation...
	typedef struct {
		ADAPTER_STATUS adapter_status;
		NAME_BUFFER    name_buffer[30];
	} GetMAC_WIN32;

	UCHAR        netbios_code;
	NCB          ncb_data;
	GetMAC_WIN32 status;

	memset(&ncb_data, '\0', sizeof(ncb_data));
	ncb_data.ncb_command  = NCBRESET;
	ncb_data.ncb_lana_num = 0;

	if ((netbios_code = Netbios(&ncb_data)) != 0)
		ThrowErrorCode(static_cast<unsigned long>(netbios_code),
			"Function 'Netbios()' returned error status on 'NCBRESET' attempt");

	memset(&ncb_data, '\0', sizeof(ncb_data));
	memset(ncb_data.ncb_callname, ' ', sizeof(ncb_data.ncb_callname));
	ncb_data.ncb_callname[0] = '*';

	ncb_data.ncb_command  = NCBASTAT;
	ncb_data.ncb_lana_num = 0;
	ncb_data.ncb_buffer   = ((unsigned char *) &status);
	ncb_data.ncb_length   = sizeof(status);

	if ((netbios_code = Netbios(&ncb_data)) != 0)
		ThrowErrorCode(static_cast<unsigned long>(netbios_code),
			"Function 'Netbios()' returned error status on 'NCASTAT' attempt");

	return(MACAddress(status.adapter_status.adapter_address));
*/
	std::deque<MACAddress> out_list = Win32_GetMacAddress_Internal(true);

	return((out_list.empty()) ? MACAddress() : out_list.front());
#else
	struct ether_addr tmp_ether_addr;

	if (::ether_hostton(GetHostName().c_str(), &tmp_ether_addr) != 0)
		ThrowSystemError("Invocation of 'ether_hostton()' failed");

	return(MACAddress(tmp_ether_addr.ether_addr_octet));
#endif // #ifdef __MSDOS__
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int MACAddress::Compare(const MACAddress *lhs, const MACAddress *rhs)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < MACAddress_Length; ++count_1) {
		if (lhs->octet[count_1] < rhs->octet[count_1])
			return(-1);
		else if (lhs->octet[count_1] > rhs->octet[count_1])
			return(1);
	}

	return(0);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MACAddress MACAddress::GetMinimumValue()
{
	MACAddress tmp_datum;

	return(tmp_datum.SetToMinimumValue());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
MACAddress MACAddress::GetMaximumValue()
{
	MACAddress tmp_datum;

	return(tmp_datum.SetToMaximumValue());
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const MACAddress &datum)
{
	char tmp_buffer[MACAddress_StringLength + 1];

	o_str <<
		datum.ToString(tmp_buffer);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <iostream>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << "MACAddress::GetMyMACAddress(): " <<
			MACAddress::GetMyMACAddress() << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << std::endl;
		std::cout << "Regression test error: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

