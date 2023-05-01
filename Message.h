
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

// Message.h -- a simple message format between Teams
#include "ecs36b_Common.h"
#include "Commutable.h"
#include "Team.h"
#include "SP_Person.h"

class Message
{
 private:
 protected:
 public:
  Commutable& from;
  Commutable& to;
  Json::Value content;

  Message(Commutable& _f, Commutable& _t): from {_f}, to {_t} {};
  virtual Json::Value dump2JSON();
  virtual bool JSON2Object(Json::Value);
};

#endif /* _MESSAGE_H_ */
