// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Request Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the LSF request command pattern.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__LSF_Request_hpp__HH

#define HH__MLB__LSF_Request_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include "LSFWrapper/LSFWrapper.hpp"

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Result base class...
//	////////////////////////////////////////////////////////////////////////////
class API_LSFWRAPPER LSF_Result_Base {
public:
	//	Used for debugging...
	virtual std::string ToString() const = 0;
protected:
	LSF_Result_Base() { }
	virtual ~LSF_Result_Base() { }

};
inline std::ostream & operator << (std::ostream &o_str,
	const LSF_Result_Base &datum)
{
	o_str << datum.ToString();

	return(o_str);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Request base class...
//	////////////////////////////////////////////////////////////////////////////
class API_LSFWRAPPER LSF_Request_Base {
public:
	virtual bool CallLSF() = 0;
	virtual LSF_Result_Base &GetResult() = 0;

	virtual bool IsDone() const = 0;
	virtual bool IsOK() const = 0;
	virtual std::string GetErrorText() const = 0;

	virtual bool IsComplete() const {
		return(IsDone() && IsOK());
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Intermediate base class really provides result type subsitutability...
//	////////////////////////////////////////////////////////////////////////////
template <typename ResultType>
	class LSF_Request_Inter : public LSF_Request_Base {
public:
	bool CallLSF() {
		try {
			CallLSF_Internal();
		}
		catch (const std::exception &except) {
			SetError(except.what());
		}
		done_flag_ = true;
		return(IsOK());
	}

	ResultType &GetResult() {
		if (!done_flag_)
			throw LSFException("Unable to return the LSF_Result as the request \
has not yet been performed.");
		if (!ok_flag_) {
			std::ostringstream error_text;
			error_text << "Unable to return the LSF_Result as the request " <<
				"failed to complete successfully: " << error_text;
			throw LSFException(error_text);
		}
		return(result_data_);
	}

	bool        IsDone() const {
		return(done_flag_);
	}
	bool        IsOK() const {
		return(ok_flag_);
	}
	std::string GetErrorText() const {
		return(error_text_);
	}

protected:
	LSF_Request_Inter() :
		 done_flag_(false)
		,ok_flag_(true)
		,error_text_()
		,result_data_() { }
	virtual ~LSF_Request_Inter() { }

	virtual void CallLSF_Internal() = 0;

	void SetError(const std::exception &except) {
		SetError(except.what());
	}
	void SetError(const std::string &error_text) {
		SetError(error_text.c_str());
	}
	void SetError(const char *error_text) {
		ok_flag_    = false;
		error_text_ = (error_text == NULL) ? "?Unspecifed error?" : error_text;
	}

	bool        done_flag_;
	bool        ok_flag_;
	std::string error_text_;
	ResultType  result_data_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Support for 'ls_getclustername'.
//	////////////////////////////////////////////////////////////////////////////
	//	Result
class API_LSFWRAPPER LSF_Result_GetClusterName : public LSF_Result_Base {
public:
	LSF_Result_GetClusterName() :
		 LSF_Result_Base()
		,cluster_name_() { }

	std::string ToString() const {
		return(cluster_name_);
	}

	std::string cluster_name_;
};
//	----------------------------------------------------------------------------
	//	Request
class LSF_Request_GetClusterName :
	public LSF_Request_Inter<LSF_Result_GetClusterName> {
public:
	LSF_Request_GetClusterName() :
		 LSF_Request_Inter<LSF_Result_GetClusterName>() { }

protected:
	virtual void CallLSF_Internal() {
		result_data_.cluster_name_ = GetClusterName();
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Support for 'ls_gethostfactor'.
//	////////////////////////////////////////////////////////////////////////////
	//	Result
class API_LSFWRAPPER LSF_Result_GetHostFactor : public LSF_Result_Base {
public:
	LSF_Result_GetHostFactor() :
		 LSF_Result_Base()
		,host_factor_(0.0) { }

	std::string ToString() const {
		std::ostringstream out_text;
		out_text << host_factor_;
		return(out_text.str());
	}

	float host_factor_;
};
//	----------------------------------------------------------------------------
	//	Request
class API_LSFWRAPPER LSF_Request_GetHostFactor :
	public LSF_Request_Inter<LSF_Result_GetHostFactor> {
public:
	LSF_Request_GetHostFactor(const std::string host_name) :
		 LSF_Request_Inter<LSF_Result_GetHostFactor>()
		,host_name_(host_name) { }

	std::string host_name_;

protected:
	virtual void CallLSF_Internal() {
		result_data_.host_factor_ = GetHostFactor(host_name_);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Support for 'ls_initrex'.
//	////////////////////////////////////////////////////////////////////////////
	//	Result
class API_LSFWRAPPER LSF_Result_InitRex : public LSF_Result_Base {
public:
	LSF_Result_InitRex() :
		 LSF_Result_Base()
		,init_rex_result_(0) { }

	std::string ToString() const {
		std::ostringstream out_text;
		out_text << init_rex_result_;
		return(out_text.str());
	}

	int init_rex_result_;
};
//	----------------------------------------------------------------------------
	//	Request
class API_LSFWRAPPER LSF_Request_InitRex :
	public LSF_Request_Inter<LSF_Result_InitRex> {
public:
	LSF_Request_InitRex(unsigned int number_of_ports, int init_rex_options = 0) :
		 LSF_Request_Inter<LSF_Result_InitRex>()
		,number_of_ports_(number_of_ports)
		,init_rex_options_(init_rex_options) { }

	unsigned int number_of_ports_;
	int          init_rex_options_;

protected:
	virtual void CallLSF_Internal() {
		result_data_.init_rex_result_ =
			InitRex(number_of_ports_, init_rex_options_);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Support for 'ls_placereq'.
//	////////////////////////////////////////////////////////////////////////////
	//	Result
class API_LSFWRAPPER LSF_Result_PlaceReq : public LSF_Result_Base {
public:
	LSF_Result_PlaceReq() :
		 LSF_Result_Base()
		,host_name_list_(0) { }

	std::string ToString() const {
		HostList::const_iterator iter_b(host_name_list_.begin());
		HostList::const_iterator iter_e(host_name_list_.end());
		std::ostringstream       out_text;
		while (iter_b != iter_e) {
			out_text << *iter_b << std::endl;
			++iter_b;
		}
		return(out_text.str());
	}

	HostList host_name_list_;
};
//	----------------------------------------------------------------------------
	//	Request
class API_LSFWRAPPER LSF_Request_PlaceReq :
	public LSF_Request_Inter<LSF_Result_PlaceReq> {
public:
	LSF_Request_PlaceReq(const std::string &resource_requirements = "",
		unsigned int number_of_hosts = 0, int place_req_options = 0,
		const std::string &from_host_name = "") :
		 LSF_Request_Inter<LSF_Result_PlaceReq>()
		,resource_requirements_(resource_requirements)
		,number_of_hosts_(number_of_hosts)
		,place_req_options_(place_req_options)
		,from_host_name_(from_host_name) { }

	std::string             resource_requirements_;
	unsigned int            number_of_hosts_;
	int                     place_req_options_;
	std::string             from_host_name_;

protected:
	virtual void CallLSF_Internal() {
		result_data_.host_name_list_ = PlaceReq(resource_requirements_,
			number_of_hosts_, place_req_options_, from_host_name_);
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Support for 'ls_rtask'.
//	////////////////////////////////////////////////////////////////////////////
	//	Result
class API_LSFWRAPPER LSF_Result_RTask : public LSF_Result_Base {
public:
	LSF_Result_RTask() :
		 LSF_Result_Base()
		,remote_process_id_(0) { }

	std::string ToString() const {
		std::ostringstream out_text;
		out_text << remote_process_id_;
		return(out_text.str());
	}

	int remote_process_id_;
};
//	----------------------------------------------------------------------------
	//	Request
class API_LSFWRAPPER LSF_Request_RTask :
	public LSF_Request_Inter<LSF_Result_RTask> {
public:
	LSF_Request_RTask(const std::string &host_name,
		const ArgvList &argument_list, int rtask_options = 0) :
		 LSF_Request_Inter<LSF_Result_RTask>()
		,host_name_(host_name)
		,argument_list_(argument_list)
		,rtask_options_(rtask_options) { }
	LSF_Request_RTask(const std::string &host_name,
		const std::string &argument_0, int rtask_options = 0) :
		 LSF_Request_Inter<LSF_Result_RTask>()
		,host_name_(host_name)
		,argument_list_(1, argument_0)
		,rtask_options_(rtask_options) { }

	std::string host_name_;
	ArgvList    argument_list_;
	int         rtask_options_;

protected:
	virtual void CallLSF_Internal() {
		result_data_.remote_process_id_ =
			RTask(host_name_, argument_list_, rtask_options_);
	}
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__LSF_Request_hpp__HH

