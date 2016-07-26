//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GpbElementInfo.cpp

	File Description	:	Implementation of the GpbElementInfo class.

	Revision History	:	2015-06-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/GpbElementInfo.hpp>
#include <ProtoBuf/GetMessageDescriptor.hpp>
#include <ProtoBuf/IsGpbType.hpp>

#include <Utility/Utility_Exception.hpp>

#include <iomanip>
#include <sstream>

#include <boost/io/ios_state.hpp>
#include <boost/algorithm/string.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

namespace {

//	////////////////////////////////////////////////////////////////////////////
const ::google::protobuf::FieldDescriptor::CppType InitialCppTypeValue =
	static_cast< ::google::protobuf::FieldDescriptor::CppType >(
	::google::protobuf::FieldDescriptor::MAX_CPPTYPE + 1);
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfo::GpbElementInfo()
	//	CODE NOTE: Fix me!!!
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfo::GpbElementInfo(const GPB_Descriptor &descriptor)
	//	CODE NOTE: Fix me!!!
{
	GpbElementInfo tmp(&descriptor, NULL, 0, -1);

	*this = tmp;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfo::GpbElementInfo(const GPB_Descriptor *descriptor)
	//	CODE NOTE: Fix me!!!
{
	GpbElementInfo tmp(descriptor, NULL, 0, -1);

	*this = tmp;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	CODE NOTE: Perhaps should enhance to support non-message types by
	searching for first for messages, then for enum types
	(with FindEnumTypeByName()).

	Perhaps finally could search for any name where it's possible to do
	fit the result into a GpbElementInfo: FindFieldByName(),
	FindExtensionByName(), FindOneofByName(), FindEnumValueByName() .
*/
GpbElementInfo::GpbElementInfo(const std::string &message_name)
	:descriptor_(NULL)
	,field_descriptor_(NULL)
	,file_descriptor_(NULL)
	,enum_descriptor_(NULL)
	,cpp_type_(InitialCppTypeValue)
	,datum_type_(GpbDatumType_Count)
	,depth_(0)
	,member_index_(-1)
	,max_depth_(0)
	,member_list_()
{
	const ::google::protobuf::Descriptor *descriptor =
		GetMessageDescriptor(message_name, true);

	GpbElementInfo tmp(descriptor, NULL, 0, -1);

	*this = tmp;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfo::GpbElementInfo(const GPB_Descriptor *descriptor,
	std::size_t depth)
	:descriptor_(descriptor)
	,field_descriptor_(NULL)
	,file_descriptor_(descriptor_->file())
	,enum_descriptor_(NULL)
	,cpp_type_(InitialCppTypeValue)
	,datum_type_(GpbDatumType_Count)
	,depth_(depth)
	,member_index_(-1)
	,max_depth_(depth_)
	,member_list_()
{
	GpbElementInfo tmp(descriptor, NULL, depth, -1);

	*this = tmp;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfo::GpbElementInfo(const GPB_Descriptor *descriptor,
	const GPB_FieldDescriptor *field_descriptor,
	std::size_t depth, int member_index)
	:descriptor_(descriptor)
	,field_descriptor_(field_descriptor)
	,file_descriptor_(DetermineFileDescriptorPtr(descriptor_, field_descriptor))
	,enum_descriptor_((field_descriptor_ && (field_descriptor_->cpp_type() ==
		::google::protobuf::FieldDescriptor::CPPTYPE_ENUM)) ?
		field_descriptor->enum_type() : NULL)
	,cpp_type_(InitialCppTypeValue)
	,datum_type_(GpbDatumType_Count)
	,depth_(depth)
	,member_index_(member_index)
	,max_depth_(depth_)
	,member_list_()
{
	using namespace ::google::protobuf;

	if (field_descriptor_) {
		cpp_type_ = field_descriptor_->cpp_type();
		if (field_descriptor_->label() == FieldDescriptor::LABEL_REPEATED) {
			if ((cpp_type_ == FieldDescriptor::CPPTYPE_STRING) ||
				 (cpp_type_ == FieldDescriptor::CPPTYPE_MESSAGE))
				datum_type_ = GpbDatumType_RepeatedPtr;
			else
				datum_type_ = GpbDatumType_Repeated;
		}
		else
			datum_type_ = static_cast<GpbDatumType>(cpp_type_);
	}
	else if (descriptor_) {
		cpp_type_   = ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE;
		datum_type_ = GpbDatumType_Message;
	}

	if ((!descriptor_) || (!descriptor_->field_count()))
		return;

	member_list_.reserve(static_cast<std::size_t>(descriptor_->field_count()));

	for (int count_1 = 0; count_1 < descriptor_->field_count(); ++count_1) {
		const GPB_FieldDescriptor *this_field_descriptor =
			descriptor_->field(count_1);
		const GPB_Descriptor      *this_descriptor       =
			this_field_descriptor->message_type();
		GpbElementInfo new_field(this_descriptor, this_field_descriptor,
			depth + 1, count_1);
		max_depth_ = std::max(max_depth_, new_field.max_depth_);
		member_list_.push_back(new_field);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetTypeNameFull() const
{
	return((enum_descriptor_) ? enum_descriptor_->full_name().c_str() :
		((descriptor_) ? descriptor_->full_name().c_str() :
		field_descriptor_->cpp_type_name()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetTypeName() const
{
	return((enum_descriptor_) ? enum_descriptor_->name().c_str() :
		((descriptor_) ? descriptor_->name().c_str() :
		field_descriptor_->cpp_type_name()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetMemberName() const
{
	return((field_descriptor_) ? field_descriptor_->name().c_str() : "");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetName() const
{
	return((field_descriptor_) ? field_descriptor_->name().c_str() :
		descriptor_->name().c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetNameFull() const
{
	return((field_descriptor_) ? field_descriptor_->full_name().c_str() :
		descriptor_->full_name().c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetTypeFileName() const
{
	return((enum_descriptor_) ? enum_descriptor_->file()->name().c_str() :
		((descriptor_) ? descriptor_->file()->name().c_str() : ""));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetMemberFileName() const
{
	return((field_descriptor_) ? field_descriptor_->file()->name().c_str() :
		"");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetFileName() const
{
	return((field_descriptor_) ? field_descriptor_->file()->name().c_str() :
		descriptor_->file()->name().c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetLabelName() const
{
	return(GetLabelName(GetLabel()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GpbElementInfo::IsGpbTypeRepeatedPtr() const
{
	return(::MLB::ProtoBuf::IsGpbTypeRepeatedPtr(*this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GpbElementInfo::IsGpbTypeRepeatedSimple() const
{
	return(::MLB::ProtoBuf::IsGpbTypeRepeatedSimple(*this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GpbElementInfo::IsGpbTypeRepeatedScalar() const
{
	return(::MLB::ProtoBuf::IsGpbTypeRepeatedScalar(*this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GpbElementInfo::IsGpbTypeRepeated() const
{
	return(::MLB::ProtoBuf::IsGpbTypeRepeated(*this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GpbElementInfo::IsGpbTypeSimple() const
{
	return(::MLB::ProtoBuf::IsGpbTypeSimple(*this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GpbElementInfo::IsGpbTypeScalar() const
{
	return(::MLB::ProtoBuf::IsGpbTypeScalar(*this));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
::google::protobuf::FieldDescriptor::Label GpbElementInfo::GetLabel() const
{
	return((field_descriptor_) ? field_descriptor_->label() :
		::google::protobuf::FieldDescriptor::LABEL_OPTIONAL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GpbElementInfo::GetSourceLocationType(
	::google::protobuf::SourceLocation &dst) const
{
	return(GetSourceLocationHelper(enum_descriptor_, dst) ||
		GetSourceLocationHelper(descriptor_, dst));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
::google::protobuf::SourceLocation GpbElementInfo::GetSourceLocationType()
	const
{
	::google::protobuf::SourceLocation tmp_dst;

	GetSourceLocationType(tmp_dst);

	return(tmp_dst);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool GpbElementInfo::GetSourceLocationMember(
	::google::protobuf::SourceLocation &dst) const
{
	return(GetSourceLocationHelper(field_descriptor_, dst));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
::google::protobuf::SourceLocation GpbElementInfo::GetSourceLocationMember()
	const
{
	::google::protobuf::SourceLocation tmp_dst;

	GetSourceLocationMember(tmp_dst);

	return(tmp_dst);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfoDescriptors GpbElementInfo::GetDescriptors() const
{
	GpbElementInfoDescriptors out_descriptors;

	out_descriptors.descriptor_       = descriptor_;
	out_descriptors.field_descriptor_ = field_descriptor_;
	out_descriptors.file_descriptor_  = file_descriptor_;
	out_descriptors.enum_descriptor_  = enum_descriptor_;

	return(out_descriptors);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const GpbElementInfo *GpbElementInfo::FindMemberName(const std::string &name,
	bool throw_if_not_found) const
{
	GpbElementInfoVectorIterC iter_b(member_list_.begin());
	GpbElementInfoVectorIterC iter_e(member_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if ((name == iter_b->GetMemberName()) || (name == iter_b->GetName()) ||
			 (name == iter_b->GetNameFull()))
			return(&(*iter_b));
	}

	if (throw_if_not_found) {
		std::ostringstream o_str;
		o_str << "Member '" << name << "' not found.";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}

	return(NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//namespace {

//	////////////////////////////////////////////////////////////////////////////
typedef const char * (GpbElementInfo::*GpbElementInfoNameFunc)() const;
//	----------------------------------------------------------------------------
GpbElementInfoNameFunc NameFuncList[GpbElementInfoMaxLengths::Count] = {
	&GpbElementInfo::GetTypeNameFull,
	&GpbElementInfo::GetTypeName,
	&GpbElementInfo::GetMemberName,
	&GpbElementInfo::GetName,
	&GpbElementInfo::GetNameFull,
	&GpbElementInfo::GetTypeFileName,
	&GpbElementInfo::GetMemberFileName,
	&GpbElementInfo::GetFileName,
	&GpbElementInfo::GetLabelName
};
/*
const std::size_t            NameFuncCount  =
	sizeof(NameFuncList) / sizeof(NameFuncList[0]);
*/
//	////////////////////////////////////////////////////////////////////////////

//} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfoMaxLengths GpbElementInfo::GetMaxLengths() const
{
	GpbElementInfoMaxLengths max_lengths;

	return(GetMaxLengths(max_lengths));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfoMaxLengths &GpbElementInfo::GetMaxLengths(
	GpbElementInfoMaxLengths &max_lengths) const
{
	for (int count_1 = 0; count_1 < GpbElementInfoMaxLengths::Count; ++count_1){
		const char *tmp_ptr = (this->*NameFuncList[count_1])();
		max_lengths.max_length_[count_1] =
			std::max(max_lengths.max_length_[count_1],
			((tmp_ptr) ? ::strlen(tmp_ptr) : 0));
	}

	for (std::size_t count_1 = 0; count_1 < GetMemberList().size(); ++count_1)
		GetMemberList()[count_1].GetMaxLengths(max_lengths);

	return(max_lengths);
}
//	////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
struct CompareByNameAndType {
	bool operator () (const GpbElementInfo &lhs, const GpbElementInfo &rhs)
	{
		int cmp;

		if ((cmp = ::strcmp(lhs.GetMemberName(), rhs.GetMemberName())) == 0)
			cmp = ::strcmp(lhs.GetTypeNameFull(), rhs.GetTypeNameFull());

		return(cmp < 0);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfoVector SetOperationHelper(bool is_diff,
	const GpbElementInfoVector &in_lhs, const GpbElementInfoVector &in_rhs,
	GpbElementInfoVector &out_lhs, GpbElementInfoVector &out_rhs)
{
	out_lhs = in_lhs;
	out_rhs = in_rhs;

	GpbElementInfoVector result(in_lhs.size());

	std::sort(out_lhs.begin(), out_lhs.end(), CompareByNameAndType());
	std::sort(out_rhs.begin(), out_rhs.end(), CompareByNameAndType());

	GpbElementInfoVectorIter iter_result((!is_diff) ?
		std::set_intersection(out_lhs.begin(), out_lhs.end(),
		out_rhs.begin(), out_rhs.end(), result.begin(), CompareByNameAndType()) :
		std::set_difference(out_lhs.begin(), out_lhs.end(),
		out_rhs.begin(), out_rhs.end(), result.begin(), CompareByNameAndType()));

	result.resize(static_cast<std::size_t>(iter_result - result.begin()));

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfoVector SetOperationHelper(bool is_diff,
	const GpbElementInfoVector &in_lhs, const GpbElementInfoVector &in_rhs)
{
	GpbElementInfoVector out_lhs;
	GpbElementInfoVector out_rhs;

	return(SetOperationHelper(is_diff, in_lhs, in_rhs, out_lhs, out_rhs));
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfoVector GpbElementInfo::SetIntersection(
	const GpbElementInfo &other) const
{
	return(SetOperationHelper(false, member_list_, other.member_list_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfoVector GpbElementInfo::SetDifference(
	const GpbElementInfo &other) const
{
	return(SetOperationHelper(true, member_list_, other.member_list_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
GpbElementInfoPairVector GpbElementInfo::SetIntersectionPair(
	const GpbElementInfo &other) const
{
	GpbElementInfoVector     tmp_lhs;
	GpbElementInfoVector     tmp_rhs;
	GpbElementInfoVector     inte_list(SetOperationHelper(false, member_list_,
		other.member_list_, tmp_lhs, tmp_rhs));
	GpbElementInfoPairVector result_list(inte_list.size());

	//	Can't fail --- we just matched lhs and rhs elements.
	for (std::size_t count_1 = 0; count_1 < result_list.size(); ++count_1)
		result_list[count_1] = GpbElementInfoPair(inte_list[count_1],
			*std::lower_bound(tmp_rhs.begin(), tmp_rhs.end(),
			inte_list[count_1], CompareByNameAndType()));

	return(result_list);

}
//	////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
const char        *EmitColumnText_1[] = {
	"Member",
	"Member",
	"Label",
	"Member",
	"Member",
	"Member",
	"Type",
	"Member"
};
const char        *EmitColumnText_2[] = {
	"Depth",
	"Index",
	"Name",
	"Type",
	"Name",
	"Full Name",
	"File Name",
	"File Name"
};
const std::size_t  EmitColumnCount     =
	sizeof(EmitColumnText_1) / sizeof(EmitColumnText_1[0]);
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
std::ostream &GpbElementInfo::EmitTabular(GpbEmitFlags::EmitFlags emit_flags,
	std::ostream &o_str) const
{
//	CODE NOTE: Test code. To be removed.
emit_flags = GpbEmitFlags::SetFlag(emit_flags, GpbEmitFlags::FullName);

	GpbElementInfoMaxLengths max_lengths(GetMaxLengths());

	max_lengths.FixMaxLengths(GetMaxDepth(), emit_flags);

	return(EmitTabular(max_lengths, emit_flags, o_str));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream &GpbElementInfo::EmitCsv(GpbEmitFlags::EmitFlags emit_flags,
	std::ostream &o_str) const
{
//	CODE NOTE: Test code. To be removed.
emit_flags = GpbEmitFlags::SetFlag(emit_flags, GpbEmitFlags::FullName);

	emit_flags = GpbEmitFlags::SetFlag(emit_flags, GpbEmitFlags::Csv);

	std::vector<std::string> head_1(EmitColumnText_1, EmitColumnText_1 + 5);
	std::vector<std::string> head_2(EmitColumnText_2, EmitColumnText_2 + 5);

	if (emit_flags & GpbEmitFlags::FullName) {
		head_1.push_back(EmitColumnText_1[5]);
		head_2.push_back(EmitColumnText_2[5]);
	}

	if (emit_flags & GpbEmitFlags::TypeFileName) {
		head_1.push_back(EmitColumnText_1[6]);
		head_2.push_back(EmitColumnText_2[6]);
	}

	if (emit_flags & GpbEmitFlags::MemberFileName) {
		head_1.push_back(EmitColumnText_1[7]);
		head_2.push_back(EmitColumnText_2[7]);
	}

	for (std::size_t count_1 = 0; count_1 < head_1.size(); ++count_1)
		o_str << ((count_1) ? "," : "") << "\"" << head_1[count_1] << "\"";
	o_str << std::endl;

	for (std::size_t count_1 = 0; count_1 < head_2.size(); ++count_1)
		o_str << ((count_1) ? "," : "") << "\"" << head_2[count_1] << "\"";
	o_str << std::endl;

	return(EmitCsv(true, emit_flags, o_str));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string GpbElementInfo::SourceLocationToString(
	const ::google::protobuf::SourceLocation &datum)
{
	std::ostringstream o_str;

	o_str << "{"
		"StartLine="        << datum.start_line        << ", "
		"EndLine="          << datum.end_line          << ", "
		"StartColumn="      << datum.start_column      << ", "
		"EndColumn="        << datum.end_column        << ", "
		"LeadingComments="  << datum.leading_comments  << ", "
		"TrailingComments=" << datum.trailing_comments <<
		"}";

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GpbElementInfo::TestFileName() const
{
	std::string pad;

	{
		std::ostringstream o_str;
		o_str << std::setw(static_cast<std::streamsize>(depth_ * 3)) << "";
		pad = o_str.str();
	}

	std::cout << pad << GetTypeName() << " " << GetName() << ": " <<
		std::endl <<
		pad << "FILE : " << ((file_descriptor_)  ?
			file_descriptor_->name()          : "") << std::endl <<
		pad << "ENUM : " << ((enum_descriptor_)  ?
			enum_descriptor_->file()->name()  : "") << std::endl <<
		pad << "FIELD: " << ((field_descriptor_) ?
			field_descriptor_->file()->name() : "") << std::endl <<
		pad << "TYPE : " << ((descriptor_)       ?
			descriptor_->file()->name()       : "");

	for (std::size_t count_1 = 0; count_1 < member_list_.size(); ++count_1) {
		std::cout << std::endl;
		member_list_[count_1].TestFileName();
	}
}
//	////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
std::string EmitTabularHelper(bool pad_flag, std::size_t pad_width,
	const char *in_string)
{
	std::ostringstream o_str;

	if (pad_flag)
		o_str << std::setw(static_cast<std::streamsize>(pad_width)) << "";

	o_str << in_string;

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::pair<std::size_t, std::size_t> GetEnumValueMaxLengths_Helper(
	const ::google::protobuf::EnumDescriptor &enum_descriptor)
{
	std::pair<std::size_t, std::size_t> max_lengths(0, 0);

	for (int count_1 = 0; count_1 < enum_descriptor.value_count(); ++count_1) {
		char buffer[1 + (sizeof(int) * 3) + 1];
		::sprintf(buffer, "%d", enum_descriptor.value(count_1)->number());
		max_lengths.first  = std::max(max_lengths.first,  ::strlen(buffer));
		max_lengths.second = std::max(max_lengths.second,
			enum_descriptor.value(count_1)->name().size());
	}

	return(max_lengths);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::pair<std::size_t, std::size_t> GetEnumValueMaxLengths_Helper(
	const GpbElementInfoDescriptors &descriptors)
{
	return((descriptors.enum_descriptor_) ?
		GetEnumValueMaxLengths_Helper(*descriptors.enum_descriptor_) :
		std::pair<std::size_t, std::size_t>(0, 0));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DatumType> std::pair<DatumType, DatumType>
	GetEnumValueMaxLengths(
		const ::google::protobuf::EnumDescriptor &enum_descriptor)
{
	std::pair<std::size_t, std::size_t> results(
		GetEnumValueMaxLengths_Helper(enum_descriptor));

	return(std::make_pair<DatumType, DatumType>(
		static_cast<DatumType>(results.first),
		static_cast<DatumType>(results.second)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DatumType> std::pair<DatumType, DatumType>
	GetEnumValueMaxLengths(const GpbElementInfoDescriptors &descriptors)
{
	std::pair<std::size_t, std::size_t> results(
		GetEnumValueMaxLengths_Helper(descriptors));

	return(std::make_pair<DatumType, DatumType>(
		static_cast<DatumType>(results.first),
		static_cast<DatumType>(results.second)));
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
std::ostream &GpbElementInfo::EmitTabular(
	const GpbElementInfoMaxLengths &max_lengths,
	GpbEmitFlags::EmitFlags emit_flags, std::ostream &o_str) const
{
	boost::io::ios_all_saver io_state(o_str);

	if (GetMemberIndex() >= 0) {
		std::size_t depth     = GetDepth();
		std::size_t depth_pad = (depth) ? ((depth - 1) * 3) : 0;
		o_str <<
			std::right <<
			std::setw(5)  << depth                  << " " <<
			std::setw(5)  << GetMemberIndex() << " " <<
			std::left  <<
			std::setw(max_lengths.width(GpbElementInfoMaxLengths::LabelName))  <<
				GetLabelName()   << " " <<
			std::setw(max_lengths.width(GpbElementInfoMaxLengths::TypeName))   <<
				EmitTabularHelper((emit_flags & GpbEmitFlags::IndentType) > 0,
				depth_pad, GetTypeName())    << " " <<
			std::setw(max_lengths.width(GpbElementInfoMaxLengths::MemberName)) <<
				EmitTabularHelper((emit_flags & GpbEmitFlags::IndentName) > 0,
				depth_pad, GetMemberName());
			if (emit_flags & GpbEmitFlags::FullName)
				o_str << " " <<
					std::setw(max_lengths.width(GpbElementInfoMaxLengths::NameFull)) <<
					GetNameFull();
			if (emit_flags & GpbEmitFlags::TypeFileName)
				o_str << " " << std::setw(max_lengths.width(
					GpbElementInfoMaxLengths::TypeFileName)) <<
					GetTypeFileName();
			if (emit_flags & GpbEmitFlags::MemberFileName)
				o_str << " " << std::setw(max_lengths.width(
					GpbElementInfoMaxLengths::MemberFileName)) <<
					GetMemberFileName();
			o_str << std::endl;
		if ((emit_flags & GpbEmitFlags::EnumValues) &&
			(GetDatumType() == GpbDatumType_Enum)) {
			const ::google::protobuf::EnumDescriptor    &enum_descriptor(
				*GetDescriptors().enum_descriptor_);
			std::pair<std::streamsize, std::streamsize>  enum_widths(
				GetEnumValueMaxLengths<std::streamsize>(enum_descriptor));
			for (int count_1 = 0; count_1 < enum_descriptor.value_count();
				++count_1)
				o_str <<
					std::setw(5 + 1 + 5 + 1 +
						max_lengths.width(GpbElementInfoMaxLengths::LabelName) + 1 + 3) <<
						"" << std::right << std::setw(enum_widths.first) <<
						enum_descriptor.value(count_1)->number() << " = " <<
						std::left << enum_descriptor.value(count_1)->name() <<
						std::endl;
		}
	}

	for (std::size_t count_1 = 0; count_1 < GetMemberList().size();
		++count_1)
		member_list_[count_1].EmitTabular(max_lengths, emit_flags, o_str);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
std::string EmitCsvHelper(bool pad_flag, std::size_t pad_width,
	const char *in_string)
{
	std::ostringstream o_str;

	o_str << "\"";

	if (pad_flag)
		o_str << std::setw(static_cast<std::streamsize>(pad_width)) << "";

	o_str << in_string << "\"";

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string EmitCsvHelper(const char *in_string)
{
	return(EmitCsvHelper(false, 0, in_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string EmitCsvHelper(const std::string &in_string)
{
	return(EmitCsvHelper(in_string.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
std::ostream &GpbElementInfo::EmitCsv(bool /* disambiguation */, 
	GpbEmitFlags::EmitFlags emit_flags, std::ostream &o_str) const
{
	boost::io::ios_all_saver io_state(o_str);

	if (GetMemberIndex() >= 0) {
		std::size_t depth     = GetDepth();
		std::size_t depth_pad = (depth) ? ((depth - 1) * 3) : 0;
		o_str <<
			depth                  << "," <<
			GetMemberIndex() << "," <<
			EmitCsvHelper(GetLabelName())  << "," <<
			EmitCsvHelper((emit_flags & GpbEmitFlags::IndentType) > 0,
				depth_pad, GetTypeName())    << "," <<
			EmitCsvHelper((emit_flags & GpbEmitFlags::IndentName) > 0,
				depth_pad, GetMemberName());
			if (emit_flags & GpbEmitFlags::FullName)
				o_str << "," <<
					EmitCsvHelper(GetNameFull());
			if (emit_flags & GpbEmitFlags::TypeFileName)
				o_str << "," <<
					EmitCsvHelper(GetTypeFileName());
			if (emit_flags & GpbEmitFlags::MemberFileName)
				o_str << "," <<
					EmitCsvHelper(GetMemberFileName());
			o_str << std::endl;
		if ((emit_flags & GpbEmitFlags::EnumValues) &&
			(GetDatumType() == GpbDatumType_Enum)) {
			const ::google::protobuf::EnumDescriptor    &enum_descriptor(
				*GetDescriptors().enum_descriptor_);
			std::pair<std::streamsize, std::streamsize>  enum_widths(
				GetEnumValueMaxLengths<std::streamsize>(enum_descriptor));
			for (int count_1 = 0; count_1 < enum_descriptor.value_count();
				++count_1)
				o_str <<
					",,,\"   " <<
					std::right << std::setw(enum_widths.first) <<
					enum_descriptor.value(count_1)->number() << " = " <<
					std::left << enum_descriptor.value(count_1)->name() << "\"" <<
					std::endl;
		}
	}

	for (std::size_t count_1 = 0; count_1 < GetMemberList().size();
		++count_1)
		member_list_[count_1].EmitCsv(true, emit_flags, o_str);

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const GpbElementInfo::GPB_FileDescriptor
	*GpbElementInfo::DetermineFileDescriptorPtr(
	const GPB_Descriptor *descriptor,
	const GPB_FieldDescriptor *field_descriptor)
{
	if (descriptor)
		return(descriptor->file());
	else if (!field_descriptor) {
		throw std::invalid_argument("Unable to determine the file descriptor "
			"value.");
	}

	return(field_descriptor->file());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *GpbElementInfo::GetLabelName(
	::google::protobuf::FieldDescriptor::Label label)
{
	if (label == ::google::protobuf::FieldDescriptor::LABEL_OPTIONAL)
		return("optional");
	else if (label == ::google::protobuf::FieldDescriptor::LABEL_REQUIRED)
		return("required");
	else if (label == ::google::protobuf::FieldDescriptor::LABEL_REPEATED)
		return("repeated");

	return("INVALID");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GpbElementInfo::ClearSourceLocation(
	::google::protobuf::SourceLocation &datum)
{
	datum.start_line   = 0;
	datum.end_line     = 0;
	datum.start_column = 0;
	datum.end_column   = 0;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const GpbElementInfo &datum)
{
	o_str << std::setw(static_cast<std::streamsize>(datum.depth_ * 3)) << "" <<
		"{"
		"Depth="                << datum.depth_              << ", "
		"TypeNameFull="         << datum.GetTypeNameFull()   << ", "
		"TypeName="             << datum.GetTypeName()       << ", "
		"MemberName="           << datum.GetMemberName()     << ", "
		"Label="                << datum.GetLabel()          << ", "
		"LabelName="            << datum.GetLabelName()      << ", "
		"TypeFileName="         << datum.GetTypeFileName()   << ", "
		"MemberFileName="       << datum.GetMemberFileName() <<
		"}";

	for (std::size_t count_1 = 0; count_1 < datum.member_list_.size();
		++count_1)
		o_str << std::endl << datum.member_list_[count_1];

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#ifdef TEST_MAIN

#include <cstdlib>
#include <iostream>

namespace {

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitSep(char sep_char, std::streamsize sep_width = 79,
	std::ostream &o_str = std::cout)
{
	boost::io::ios_all_saver io_state(o_str);

	o_str << std::setfill(sep_char) << std::setw(sep_width) << "" << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitDatum(const MLB::ProtoBuf::GpbElementInfo &datum)
{
#if 1
//	std::cout << datum << std::endl;
	datum.EmitTabular();
	datum.EmitCsv();
#else
	datum.TestFileName();
	std::cout << std::endl;
#endif // #if 1

}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename MessageType>
	bool TEST_RunTest_1(int &return_code)
{
	bool test_passed = true;

	TEST_EmitSep('=');
	TEST_EmitSep('=');
	std::cout << "TEST for a templated messaged type:" << std::endl;
	TEST_EmitSep('=');

	try {
		MessageType                          test_msg;
		MLB::ProtoBuf::GpbElementInfo element(*test_msg.descriptor());
		TEST_EmitDatum(element);
	}
	catch (const std::exception &except) {
		std::cerr << "TEST FAILED: " << except.what() << std::endl;
		test_passed = false;
	}

	TEST_EmitSep('=');
	std::cout << std::endl;

	if (!test_passed)
		return_code = EXIT_FAILURE;

	return(test_passed);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TEST_RunTest_2(int &return_code, const char *message_name)
{
	TEST_EmitSep('=');
	TEST_EmitSep('=');
	std::cout << "TEST for a message name ('" << message_name << "'):" <<
		std::endl;
	TEST_EmitSep('=');

	bool test_passed = true;

	try {
		MLB::ProtoBuf::GpbElementInfo element(message_name);
		TEST_EmitDatum(element);
	}
	catch (const std::exception &except) {
		std::cerr << "TEST FAILED: " << except.what() << std::endl;
		test_passed = false;
	}

	TEST_EmitSep('=');
	std::cout << std::endl;

	if (!test_passed)
		return_code = EXIT_FAILURE;

	return(test_passed);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_RunTest_3_HelperInner(
	const MLB::ProtoBuf::GpbElementInfo &element_1,
	const MLB::ProtoBuf::GpbElementInfo &element_2,
	MLB::ProtoBuf::GpbElementInfoVector &inte_list,
	MLB::ProtoBuf::GpbElementInfoVector &diff_list)
{
	inte_list = element_1.SetIntersection(element_2);
	diff_list = element_1.SetDifference(element_2);

	std::cout << "Source     : " << element_1.GetTypeNameFull() << std::endl;
	std::cout << "Destination: " << element_2.GetTypeNameFull() << std::endl;
	TEST_EmitSep('-');
	std::cout << "Same in source and in destination: " << inte_list.size() <<
		std::endl;
	std::copy(inte_list.begin(), inte_list.end(), std::ostream_iterator<
		MLB::ProtoBuf::GpbElementInfoVector::value_type>(
		std::cout, "\n"));
	TEST_EmitSep('-');
	std::cout << "In source, not in destination    : " << diff_list.size() <<
		std::endl;
	std::copy(diff_list.begin(), diff_list.end(), std::ostream_iterator<
		MLB::ProtoBuf::GpbElementInfoVector::value_type>(
		std::cout, "\n"));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_RunTest_3_Helper(
	const MLB::ProtoBuf::GpbElementInfo &element_1,
	const MLB::ProtoBuf::GpbElementInfo &element_2,
	MLB::ProtoBuf::GpbElementInfoVector inte_lists[2],
	MLB::ProtoBuf::GpbElementInfoVector diff_lists[2])
{
	TEST_RunTest_3_HelperInner(element_1, element_2, inte_lists[0],
		diff_lists[0]);
	TEST_EmitSep('=');
	TEST_RunTest_3_HelperInner(element_2, element_1, inte_lists[1],
		diff_lists[1]);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TEST_Compare(int &return_code, const std::string &message_name_1,
	const std::string &message_name_2)
{
	TEST_EmitSep('=');
	TEST_EmitSep('=');
	std::cout << "TEST of conversion logic generation:" << std::endl;
	std::cout << "   From: " << message_name_1 << std::endl;
	std::cout << "   To  : " << message_name_2 << std::endl;
	TEST_EmitSep('=');

	bool test_passed = true;

	try {
		MLB::ProtoBuf::GpbElementInfo       element_1(message_name_1);
		MLB::ProtoBuf::GpbElementInfo       element_2(message_name_2);
		MLB::ProtoBuf::GpbElementInfoVector inte_lists[2];
		MLB::ProtoBuf::GpbElementInfoVector diff_lists[2];
		TEST_RunTest_3_Helper(element_1, element_2, inte_lists, diff_lists);
	}
	catch (const std::exception &except) {
		std::cerr << "TEST FAILED: " << except.what() << std::endl;
		test_passed = false;
	}

	TEST_EmitSep('=');
	std::cout << std::endl;

	if (!test_passed)
		return_code = EXIT_FAILURE;

	return(test_passed);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TEST_RunTest_3(int &return_code, const std::string &message_name_1,
	const std::string &message_name_2)
{
	return(TEST_Compare(return_code, message_name_1, message_name_2));
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_RunTest_2(return_code, "GpbElementInfoTestOne::AddressBook");
		TEST_RunTest_2(return_code, "GpbElementInfoTestTwo::AddressBook");
		TEST_RunTest_2(return_code, "GpbElementInfoTestTwo::Person");
		TEST_RunTest_2(return_code, "GpbElementInfoTestTwo::PhoneNumber");
		TEST_RunTest_2(return_code, "GpbElementInfoTestThree::AddressBookOne");
		TEST_RunTest_2(return_code, "GpbElementInfoTestThree::AddressBookTwo");
		TEST_RunTest_3(return_code,
			"GpbElementInfoTestThree::PersonOne",
			"GpbElementInfoTestThree::PersonTwo");
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

