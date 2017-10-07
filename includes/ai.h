#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/filestream.h>
#include <string>
#include <thread>
#include <vector>
#include <glib.h>

#include "json.hpp"
#include "utils.h"
#include "DataStructures.h"

using json = nlohmann::json;
using namespace std;
using namespace utility;
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::http::experimental::listener;

class AI {

public:

    AI(string playerName_);
    void run();
    int saveState();
    void requestHandler(http_request req);
    string createAction(string actionName, PurchasableItem item);
    string createAction(string actionName, Point target);
    string createAction(string actionName, UpgradeType upgrade);
    vector<vector<Tile>> deserializeMap(string serializedMap);

    string createMoveAction(Point target);
    string createAttackAction(Point target);
    string createCollectAction(Point target);
    string createStealAction(Point target);
    string createHealAction(Point target);
    string createPurchaseAction(PurchasableItem item);


private:

    string playerName;
    string stateFilePath = "/data/state.phx";
    http_listener listener;

};
