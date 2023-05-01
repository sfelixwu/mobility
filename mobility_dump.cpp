

// ecs36b mobility_client program

#include <iostream>
#include "ecs36b_Exception.h"

// JSON RPC part
#include "mobility_client.h"
#include "Message.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

int
main()
{
  std::string my_team_name = "fire_ball"; // put your registered team name

  // okay, going for the server
  HttpClient httpclient("http://127.0.0.1:7374"); // replace here with the ngrok link
  mobility_Client myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value myv;

  try {
    myv = myClient.dump2JSON("dump2JSON", "Message", my_team_name);
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
  std::cout << myv.toStyledString() << std::endl;  
  return 0;
}
