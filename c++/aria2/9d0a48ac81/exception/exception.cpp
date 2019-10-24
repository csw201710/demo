
#include <string>
#include <memory>
#include <sstream>
namespace aria2 {
	namespace error_code {

		enum Value {
			UNKNOWN_ERROR = -99,
			OK = 0
		};

	} // namespace error_code


	class Exception : public std::exception {
	private:
		const char* file_;

		int line_;
		// This is low-level system error code, typically errno in Linux.
		int errNum_;

		std::string msg_;
		// This is application-level error code.
		error_code::Value errorCode_;
		// Exception that this object wraps. Normally this cause_ is the
		// root cause of this exception.
		std::shared_ptr<Exception> cause_;

	protected:
		virtual std::shared_ptr<Exception> copy() const { return std::make_shared<Exception>(*this); };// = 0


	public:
		//错误信息
		Exception(const char* file, int line, const std::string& msg) : file_(file),
			line_(line),
			errNum_(0),
			msg_(msg),
			errorCode_(error_code::UNKNOWN_ERROR)
		{
		}
		//错误信息+错误码+异常
		Exception(const char* file, int line, const std::string& msg,
			error_code::Value errorCode, const Exception& cause) : file_(file),
			line_(line),
			errNum_(0),
			msg_(msg),
			errorCode_(errorCode),
			cause_(cause.copy())
		{
		}
		// 错误信息+异常
		// errorCode_ is initializedwith cause.errorCode_.
		Exception(const char* file, int line, const std::string& msg,
			const Exception& cause) : file_(file),
			line_(line),
			errNum_(0),
			msg_(msg),
			errorCode_(cause.errorCode_),
			cause_(cause.copy())
		{
		}

		//错误信息+错误码
		Exception(const char* file, int line, const std::string& msg,
			error_code::Value errorCode) : file_(file), line_(line), errNum_(0), msg_(msg), errorCode_(errorCode)
		{
		}
		//错误信息
		Exception(const char* file, int line, int errNum, const std::string& msg) : file_(file),
			line_(line),
			errNum_(errNum),
			msg_(msg),
			errorCode_(error_code::UNKNOWN_ERROR)
		{
		}
		//errno + 错误信息+错误码
		Exception(const char* file, int line, int errNum, const std::string& msg,
			error_code::Value errorCode) : file_(file),
			line_(line),
			errNum_(errNum),
			msg_(msg),
			errorCode_(errorCode)
		{
		}

		virtual ~Exception() throw() {};// = default;

		virtual const char* what() const throw() { return msg_.c_str(); } 
		

		std::string stackTrace() const {
			std::stringstream s;
			s << "Exception: "
				<< "[" << file_ << ":" << line_ << "] ";
			if (errNum_) {
				s << "errNum=" << errNum_ << " ";
			}
			s << "errorCode=" << errorCode_ << " ";
			s << what() << "\n";
			std::shared_ptr<Exception> e = cause_;
			while (e) {
				s << "  -> "
					<< "[" << e->file_ << ":" << e->line_ << "] ";
				if (e->getErrNum()) {
					s << "errNum=" << e->getErrNum() << " ";
				}
				s << "errorCode=" << e->getErrorCode() << " " << e->what() << "\n";
				e = e->cause_;
			}
			return s.str();
		}

		int getErrNum() const { return errNum_; }

		error_code::Value getErrorCode() const { return errorCode_; }
	};

} // namespace aria2
using namespace aria2;
class FatalException : public Exception {
protected:
	virtual std::shared_ptr<Exception> copy() const {
		return std::make_shared<FatalException>(*this);
	}

public:
	FatalException(const char* file, int line, const std::string& msg) : Exception(file, line, msg)
	{
	}

	FatalException(const char* file, int line, const std::string& msg,
		const Exception& cause) : Exception(file, line, msg, cause)
	{
	}
};
#define FATAL_EXCEPTION(arg) FatalException(__FILE__, __LINE__, arg)
#define FATAL_EXCEPTION2(arg1, arg2)   FatalException(__FILE__, __LINE__, arg1, arg2)




void testException(){

	try {
		throw FATAL_EXCEPTION("sssaaaaaaaaaaaaa");
	}
	catch (Exception e) {
		printf("%s",e.stackTrace().c_str());
		printf("%s", e.what());
	}
}
