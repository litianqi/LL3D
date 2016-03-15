#include "Exceptions.h"
#include <sstream>
#include <comdef.h>
#include "../Utils/Encoding.h"

namespace LL3D {

NotFound::NotFound(const std::string & what) :
  std::runtime_error(what) {
}

ComException::ComException(HRESULT hr) {
  std::stringstream ss;
  _com_error err(hr);
  ss << "Failure with HRESULT of 0x" << std::hex << hr << "\n" <<
    "Description: " << Utils::Convert(err.ErrorMessage());
  what_ = ss.str();
}

const char * ComException::what() const {
  return what_.c_str();
}

InvalidArgument::InvalidArgument(const std::string & what) :
runtime_error(what) {
}

}  // namespace LL3D