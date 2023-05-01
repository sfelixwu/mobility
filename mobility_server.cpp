
// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

// for JsonRPCCPP
#include <iostream>
#include "mobility_server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include "mobility_client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <stdio.h>

// ecs36b
#include "SP_Person.h"
#include "Team.h"
#include <time.h>
#include "ecs36b_Common.h"

using namespace jsonrpc;
using namespace std;

class Mymobility_Server : public mobility_Server
{
public:
  Mymobility_Server(AbstractServerConnector &connector, serverVersion_t type);
  virtual Json::Value send(const std::string& action,
			   const std::string& class_id,
			   const Json::Value& json_object,
			   const std::string& object_id);
  virtual Json::Value dump2JSON(const std::string& action,
				const std::string& class_id,
				const std::string& object_id);
};

Mymobility_Server::Mymobility_Server(AbstractServerConnector &connector, serverVersion_t type)
  : mobility_Server(connector, type)
{
  std::cout << "Mymobility_Server Object created" << std::endl;
}

// member functions

Json::Value
Mymobility_Server::send
(const std::string& action, const std::string& class_id,
 const Json::Value& json_object, const std::string& object_id)
{
  int error_code = 0;
  Json::Value result;
  std::string strJson;

  if (class_id != "Message")
    {
      result["status"] = "failed";
      strJson = ( "class " + class_id + " unknown");
      result["reason"] = strJson;
      error_code = -1;
    }

  if (object_id.empty())
    {
      result["status"] = "failed";
      strJson = ( "object_id null or invalid");
      result["reason"] = strJson;
      error_code = -2;
    }

  if ((json_object.isNull() == true) ||
      (json_object.isObject() == false))
    {
      result["status"] = "failed";
      strJson = ( "message parsing error " );
      result["reason"] = strJson;
      error_code = -4;
    }

  if (error_code == 0)
    {
      char s[256];
      char *s_ptr = s;
      snprintf(s, strlen(object_id.c_str()) + 17,
	       "./objects/%s.json", object_id.c_str());
  
      Json::Value jv = json_object;
      myJSON2File(s_ptr, &jv);
      result["status"] = "successful";
    }

  myPrintLog(result.toStyledString(), "./log/ecs36b_server.log");
  // std::cout << result.toStyledString() << std::endl;
  return result;
}

Json::Value
Mymobility_Server::dump2JSON
(const std::string& action, const std::string& class_id,
 const std::string& object_id)
{
  int error_code = 0;
  Json::Value result;
  std::string strJson;

  if (class_id != "Message")
    {
      result["status"] = "failed";
      strJson = ( "class " + class_id + " unknown");
      result["reason"] = strJson;
      error_code = -1;
    }

  if (object_id.empty())
    {
      result["status"] = "failed";
      strJson = ( "object_id null or invalid");
      result["reason"] = strJson;
      error_code = -2;
    }

  if (error_code == 0)
    {
      char s[256];
      char *s_ptr = s;
      snprintf(s, strlen(object_id.c_str()) + 17,
	       "./objects/%s.json", object_id.c_str());
  
      Json::Value jv;
      error_code = myFile2JSON(s, &jv);

      if (error_code != 0)
	{
	  result["status"] = "failed";
	  strJson = ( "Message or team name mismatched");
	  result["reason"] = strJson;
	  error_code = -5;
	}
      else
	{
	  result["status"] = "successful";
	  result["message"] = jv;
	}
    }

  myPrintLog(result.toStyledString(), "./log/ecs36b_server.log");
  // std::cout << result.toStyledString() << std::endl;
  return result;
}

int main()
{
  HttpServer httpserver(7374);
  Mymobility_Server s(httpserver,
		JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  std::cout << "Hit enter to stop the server" << endl;
  getchar();

  s.StopListening();
  return 0;
}
