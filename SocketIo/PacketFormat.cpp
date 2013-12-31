// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	SocketIo Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of support for the PacketFormat
								enumeration.

	Revision History	:	2008-12-20 --- Creation.
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

#include <SocketIo/PacketFormat.hpp>

#include <Utility/C_StringSupport.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

namespace {
// ////////////////////////////////////////////////////////////////////////////
const char *PacketFormatNameList[PacketFormat_Count] = {
	"Raw",
	"Length",
	"Timestamped",
	"IpSource",
	"TimestampedIpSource",
	"IpSourceTLen"
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct PacketFormatStringToEnum {
	PacketFormat  packet_format_;
	const char   *packet_format_string_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const PacketFormatStringToEnum PacketFormatStringToEnumList[] = {
	{ PacketFormat_Raw,          "RAW"           				},
	{ PacketFormat_Raw,          "NONE"          				},

	{ PacketFormat_Length,       "LENGTH"        				},
	{ PacketFormat_Length,       "LEN"           				},

	{ PacketFormat_Time,         "TIME"								},
	{ PacketFormat_Time,         "TIMESTAMP"						},
	{ PacketFormat_Time,         "TIMESTAMPED"					},

	{ PacketFormat_IpSource,     "IPSOURCE"      				},
	{ PacketFormat_IpSource,     "IP_SOURCE"     				},
	{ PacketFormat_IpSource,     "IPSRC"         				},
	{ PacketFormat_IpSource,     "IP_SRC"        				},
	{ PacketFormat_IpSource,     "IP"            				},
	{ PacketFormat_IpSource,     "IPSOURCELLEN"  				},
	{ PacketFormat_IpSource,     "IP_SOURCELLEN" 				},
	{ PacketFormat_IpSource,     "IPSRCLLEN"     				},
	{ PacketFormat_IpSource,     "IP_SRCLLEN"    				},
	{ PacketFormat_IpSource,     "IPLLEN"        				},

	{ PacketFormat_TimeIpSource, "TIME_IPSOURCE"      			},
	{ PacketFormat_TimeIpSource, "TIME_IP_SOURCE"     			},
	{ PacketFormat_TimeIpSource, "TIME_IPSRC"         			},
	{ PacketFormat_TimeIpSource, "TIME_IP_SRC"        			},
	{ PacketFormat_TimeIpSource, "TIME_IP"            			},
	{ PacketFormat_TimeIpSource, "TIME_IPSOURCELLEN"  			},
	{ PacketFormat_TimeIpSource, "TIME_IP_SOURCELLEN" 			},
	{ PacketFormat_TimeIpSource, "TIME_IPSRCLLEN"     			},
	{ PacketFormat_TimeIpSource, "TIME_IP_SRCLLEN"    			},
	{ PacketFormat_TimeIpSource, "TIME_IPLLEN"        			},
	{ PacketFormat_TimeIpSource, "TIMEIPSOURCE"      			},
	{ PacketFormat_TimeIpSource, "TIMEIP_SOURCE"     			},
	{ PacketFormat_TimeIpSource, "TIMEIPSRC"         			},
	{ PacketFormat_TimeIpSource, "TIMEIP_SRC"        			},
	{ PacketFormat_TimeIpSource, "TIMEIP"            			},
	{ PacketFormat_TimeIpSource, "TIMEIPSOURCELLEN"  			},
	{ PacketFormat_TimeIpSource, "TIMEIP_SOURCELLEN" 			},
	{ PacketFormat_TimeIpSource, "TIMEIPSRCLLEN"     			},
	{ PacketFormat_TimeIpSource, "TIMEIP_SRCLLEN"    			},
	{ PacketFormat_TimeIpSource, "TIMEIPLLEN"        			},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IPSOURCE"      	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IP_SOURCE"     	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IPSRC"         	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IP_SRC"        	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IP"            	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IPSOURCELLEN"  	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IP_SOURCELLEN" 	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IPSRCLLEN"     	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IP_SRCLLEN"    	},
	{ PacketFormat_TimeIpSource, "TIMESTAMP_IPLLEN"        	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIPSOURCE"      	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIP_SOURCE"     	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIPSRC"         	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIP_SRC"        	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIP"            	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIPSOURCELLEN"  	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIP_SOURCELLEN" 	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIPSRCLLEN"     	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIP_SRCLLEN"    	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPIPLLEN"        	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IPSOURCE"      },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IP_SOURCE"     },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IPSRC"         },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IP_SRC"        },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IP"            },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IPSOURCELLEN"  },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IP_SOURCELLEN" },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IPSRCLLEN"     },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IP_SRCLLEN"    },
	{ PacketFormat_TimeIpSource, "TIMESTAMPED_IPLLEN"        },
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIPSOURCE"      	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIP_SOURCE"     	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIPSRC"         	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIP_SRC"        	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIP"            	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIPSOURCELLEN"  	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIP_SOURCELLEN" 	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIPSRCLLEN"     	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIP_SRCLLEN"    	},
	{ PacketFormat_TimeIpSource, "TIMESTAMPEDIPLLEN"        	},

	{ PacketFormat_IpSourceTLen, "IPSOURCETLEN"  				},
	{ PacketFormat_IpSourceTLen, "IP_SOURCETLEN" 				},
	{ PacketFormat_IpSourceTLen, "IPSRCTLEN"     				},
	{ PacketFormat_IpSourceTLen, "IP_SRCTLEN"    				},
	{ PacketFormat_IpSourceTLen, "IPTLEN"        				}

};
const unsigned int             PacketFormatStringToEnumCount  =
	sizeof(PacketFormatStringToEnumList) /
	sizeof(PacketFormatStringToEnumList[0]);
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
bool PacketFormatIsValid(PacketFormat packet_format)
{
	return((packet_format >= PacketFormat_Min) &&
		(packet_format <= PacketFormat_Max));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketFormat CheckPacketFormat(PacketFormat packet_format)
{
	if (!PacketFormatIsValid(packet_format))
 		MLB::Utility::ThrowInvalidArgument("Invalid packet format type (" +
			MLB::Utility::ValueToStringHex(packet_format) + ").");

	return(packet_format);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool IsPacketFormatString(const char *in_packet_format,
	PacketFormat &out_packet_format)
{
	unsigned int count_1;

	for (count_1 = 0; count_1 < PacketFormatStringToEnumCount; ++count_1) {
		if (!MLB::Utility::Utility_stricmp(in_packet_format,
			PacketFormatStringToEnumList[count_1].packet_format_string_)) {
			out_packet_format =
				PacketFormatStringToEnumList[count_1].packet_format_;
			return(true);
		}
	}

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool IsPacketFormatString(const std::string &in_packet_format,
	PacketFormat &out_packet_format)
{
	return(IsPacketFormatString(in_packet_format.c_str(),
		out_packet_format));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool IsPacketFormatString(const char *in_packet_format)
{
	PacketFormat out_packet_format;

	return(IsPacketFormatString(in_packet_format, out_packet_format));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool IsPacketFormatString(const std::string &in_packet_format)
{
	return(IsPacketFormatString(in_packet_format.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string PacketFormatToString(PacketFormat packet_format)
{
	return((PacketFormatIsValid(packet_format)) ?
		PacketFormatNameList[packet_format] :
		MLB::Utility::ValueToStringHex(packet_format));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketFormat StringToPacketFormat(const char *packet_format)
{
	PacketFormat out_packet_format = PacketFormat_Invalid;

	if (!IsPacketFormatString(packet_format, out_packet_format))
		MLB::Utility::ThrowInvalidArgument("Invalid packet format type string "
			"('" + std::string(packet_format) + "') --- expected one of 'RAW', "
			"'LENGTH', 'TIMESTAMPED', 'IPSOURCE', 'TIMESTAMPEDIPSOURCE' or "
			"'IPSOURCETLEN'.");

	return(out_packet_format);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketFormat StringToPacketFormat(const std::string &packet_format)
{
	return(StringToPacketFormat(packet_format.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PacketFormat &datum)
{
	o_str << PacketFormatToString(datum);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

