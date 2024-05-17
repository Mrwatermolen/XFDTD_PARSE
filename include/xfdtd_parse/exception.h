#ifndef __XFDTD_PARSE_EXCEPTION_H__
#define __XFDTD_PARSE_EXCEPTION_H__

#include <xfdtd/exception/exception.h>

namespace xfdtd::parse {

class XFDTDParseException : public XFDTDException {
 public:
  XFDTDParseException(const std::string &msg) : XFDTDException{msg} {}
};

}  // namespace xfdtd::parse

#endif  // __XFDTD_PARSE_EXCEPTION_H__
