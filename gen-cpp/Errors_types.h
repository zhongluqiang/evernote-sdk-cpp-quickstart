/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Errors_TYPES_H
#define Errors_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/stdcxx.h>
#include "Types_types.h"


namespace evernote { namespace edam {

struct EDAMErrorCode {
  enum type {
    UNKNOWN = 1,
    BAD_DATA_FORMAT = 2,
    PERMISSION_DENIED = 3,
    INTERNAL_ERROR = 4,
    DATA_REQUIRED = 5,
    LIMIT_REACHED = 6,
    QUOTA_REACHED = 7,
    INVALID_AUTH = 8,
    AUTH_EXPIRED = 9,
    DATA_CONFLICT = 10,
    ENML_VALIDATION = 11,
    SHARD_UNAVAILABLE = 12,
    LEN_TOO_SHORT = 13,
    LEN_TOO_LONG = 14,
    TOO_FEW = 15,
    TOO_MANY = 16,
    UNSUPPORTED_OPERATION = 17,
    TAKEN_DOWN = 18,
    RATE_LIMIT_REACHED = 19,
    BUSINESS_SECURITY_LOGIN_REQUIRED = 20,
    DEVICE_LIMIT_REACHED = 21,
    OPENID_ALREADY_TAKEN = 22,
    INVALID_OPENID_TOKEN = 23,
    USER_NOT_ASSOCIATED = 24,
    USER_NOT_REGISTERED = 25,
    USER_ALREADY_ASSOCIATED = 26,
    ACCOUNT_CLEAR = 27,
    SSO_AUTHENTICATION_REQUIRED = 28
  };
};

extern const std::map<int, const char*> _EDAMErrorCode_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const EDAMErrorCode::type& val);

struct EDAMInvalidContactReason {
  enum type {
    BAD_ADDRESS = 0,
    DUPLICATE_CONTACT = 1,
    NO_CONNECTION = 2
  };
};

extern const std::map<int, const char*> _EDAMInvalidContactReason_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const EDAMInvalidContactReason::type& val);

class EDAMUserException;

class EDAMSystemException;

class EDAMNotFoundException;

class EDAMInvalidContactsException;

typedef struct _EDAMUserException__isset {
  _EDAMUserException__isset() : parameter(false) {}
  bool parameter :1;
} _EDAMUserException__isset;

class EDAMUserException : public ::apache::thrift::TException {
 public:

  EDAMUserException(const EDAMUserException&);
  EDAMUserException& operator=(const EDAMUserException&);
  EDAMUserException() : errorCode((EDAMErrorCode::type)0), parameter() {
  }

  virtual ~EDAMUserException() throw();
  EDAMErrorCode::type errorCode;
  std::string parameter;

  _EDAMUserException__isset __isset;

  void __set_errorCode(const EDAMErrorCode::type val);

  void __set_parameter(const std::string& val);

  bool operator == (const EDAMUserException & rhs) const
  {
    if (!(errorCode == rhs.errorCode))
      return false;
    if (__isset.parameter != rhs.__isset.parameter)
      return false;
    else if (__isset.parameter && !(parameter == rhs.parameter))
      return false;
    return true;
  }
  bool operator != (const EDAMUserException &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EDAMUserException & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const throw();
};

void swap(EDAMUserException &a, EDAMUserException &b);

std::ostream& operator<<(std::ostream& out, const EDAMUserException& obj);

typedef struct _EDAMSystemException__isset {
  _EDAMSystemException__isset() : message(false), rateLimitDuration(false) {}
  bool message :1;
  bool rateLimitDuration :1;
} _EDAMSystemException__isset;

class EDAMSystemException : public ::apache::thrift::TException {
 public:

  EDAMSystemException(const EDAMSystemException&);
  EDAMSystemException& operator=(const EDAMSystemException&);
  EDAMSystemException() : errorCode((EDAMErrorCode::type)0), message(), rateLimitDuration(0) {
  }

  virtual ~EDAMSystemException() throw();
  EDAMErrorCode::type errorCode;
  std::string message;
  int32_t rateLimitDuration;

  _EDAMSystemException__isset __isset;

  void __set_errorCode(const EDAMErrorCode::type val);

  void __set_message(const std::string& val);

  void __set_rateLimitDuration(const int32_t val);

  bool operator == (const EDAMSystemException & rhs) const
  {
    if (!(errorCode == rhs.errorCode))
      return false;
    if (__isset.message != rhs.__isset.message)
      return false;
    else if (__isset.message && !(message == rhs.message))
      return false;
    if (__isset.rateLimitDuration != rhs.__isset.rateLimitDuration)
      return false;
    else if (__isset.rateLimitDuration && !(rateLimitDuration == rhs.rateLimitDuration))
      return false;
    return true;
  }
  bool operator != (const EDAMSystemException &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EDAMSystemException & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const throw();
};

void swap(EDAMSystemException &a, EDAMSystemException &b);

std::ostream& operator<<(std::ostream& out, const EDAMSystemException& obj);

typedef struct _EDAMNotFoundException__isset {
  _EDAMNotFoundException__isset() : identifier(false), key(false) {}
  bool identifier :1;
  bool key :1;
} _EDAMNotFoundException__isset;

class EDAMNotFoundException : public ::apache::thrift::TException {
 public:

  EDAMNotFoundException(const EDAMNotFoundException&);
  EDAMNotFoundException& operator=(const EDAMNotFoundException&);
  EDAMNotFoundException() : identifier(), key() {
  }

  virtual ~EDAMNotFoundException() throw();
  std::string identifier;
  std::string key;

  _EDAMNotFoundException__isset __isset;

  void __set_identifier(const std::string& val);

  void __set_key(const std::string& val);

  bool operator == (const EDAMNotFoundException & rhs) const
  {
    if (__isset.identifier != rhs.__isset.identifier)
      return false;
    else if (__isset.identifier && !(identifier == rhs.identifier))
      return false;
    if (__isset.key != rhs.__isset.key)
      return false;
    else if (__isset.key && !(key == rhs.key))
      return false;
    return true;
  }
  bool operator != (const EDAMNotFoundException &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EDAMNotFoundException & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const throw();
};

void swap(EDAMNotFoundException &a, EDAMNotFoundException &b);

std::ostream& operator<<(std::ostream& out, const EDAMNotFoundException& obj);

typedef struct _EDAMInvalidContactsException__isset {
  _EDAMInvalidContactsException__isset() : parameter(false), reasons(false) {}
  bool parameter :1;
  bool reasons :1;
} _EDAMInvalidContactsException__isset;

class EDAMInvalidContactsException : public ::apache::thrift::TException {
 public:

  EDAMInvalidContactsException(const EDAMInvalidContactsException&);
  EDAMInvalidContactsException& operator=(const EDAMInvalidContactsException&);
  EDAMInvalidContactsException() : parameter() {
  }

  virtual ~EDAMInvalidContactsException() throw();
  std::vector< ::evernote::edam::Contact>  contacts;
  std::string parameter;
  std::vector<EDAMInvalidContactReason::type>  reasons;

  _EDAMInvalidContactsException__isset __isset;

  void __set_contacts(const std::vector< ::evernote::edam::Contact> & val);

  void __set_parameter(const std::string& val);

  void __set_reasons(const std::vector<EDAMInvalidContactReason::type> & val);

  bool operator == (const EDAMInvalidContactsException & rhs) const
  {
    if (!(contacts == rhs.contacts))
      return false;
    if (__isset.parameter != rhs.__isset.parameter)
      return false;
    else if (__isset.parameter && !(parameter == rhs.parameter))
      return false;
    if (__isset.reasons != rhs.__isset.reasons)
      return false;
    else if (__isset.reasons && !(reasons == rhs.reasons))
      return false;
    return true;
  }
  bool operator != (const EDAMInvalidContactsException &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EDAMInvalidContactsException & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const throw();
};

void swap(EDAMInvalidContactsException &a, EDAMInvalidContactsException &b);

std::ostream& operator<<(std::ostream& out, const EDAMInvalidContactsException& obj);

}} // namespace

#endif
