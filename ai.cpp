#include "ai.h"

AI::AI(string playerName_) {
        playerName = playerName_;
        uri serverURI("http://0.0.0.0:3000/");
        uri_builder builder;
        listener = http_listener(serverURI);
        listener.open().wait();
}

string AI::createAction(string actionName, Point target) {

    json action;
    json point;
    point["X"] = target.getX();
    point["Y"] = target.getY();
    action["ActionName"] = actionName;
    action["Content"] = point.dump();

    return action.dump();

}

string AI::createAction(string actionName, PurchasableItem item) {

    char itemString[10];
    json action;
    action["ActionName"] = actionName;
    sprintf(itemString, "%d", item);
    action["Content"] = itemString;

    return action.dump();

}

string AI::createAction(string actionName, UpgradeType upgrade) {

    char upgradeString[10];
    json action;
    action["ActionName"] = actionName;
    sprintf(upgradeString, "%d", upgrade);
    action["Content"] = upgradeString;

    return action.dump();

}

string AI::createMoveAction(Point target) {

    return createAction("MoveAction", target);

}

string AI::createAttackAction(Point target) {
    return createAction("AttackAction", target);
}

string AI::createCollectAction(Point target) {
    return createAction("CollectAction", target);
}

string AI::createStealAction(Point target) {
    return createAction("StealAction", target);
}

string AI::createHealAction(Point target) {
    return createAction("HealAction", target);
}

string AI::createPurchaseAction(PurchasableItem item) {
    return createAction("PurchaseAction", item);
}


vector<vector<Tile>> AI::deserializeMap(string serializedMap) {

    serializedMap = serializedMap.substr(1, serializedMap.length() - 1);
    vector<string> rows = split(serializedMap, "[");
    vector<string> column = split(rows[1], "{");
    vector<vector<Tile>> map;

    for (int i = 0; i < rows.size() - 1; i++) {
        column = split(rows[i + 1], "{");
        vector<Tile> tileTmp;
        for (int j = 0; j < column.size() - 1; j++)
        {
            vector<string> infos = split(column[j + 1], ",");
            int content = atoi(infos[0].c_str());
            int x = atoi(infos[1].c_str());
            int end_index = infos[2].find("}");
            int y = atoi(infos[2].substr(0, end_index).c_str());
            Tile tile(content, x, y);
            tileTmp.push_back(tile);
        }
        map.push_back(tileTmp);
    }
    return map;

}


/*
3:55 am: ON PARSE DU JSON A BRAS

5:06 am: P-e une lib qui marche

5:32 am: deserializeMap fonctionne

*/
void AI::requestHandler(http_request request) {

    request.extract_string().then([=](string raw)

        {
            // ### Deserialiser la map ###
            string body =  g_uri_unescape_string(raw.c_str(), NULL);

            vector<string> bodySplit = split(body, "=");

            string gameInfoJsonRaw = bodySplit[1];

            cout << gameInfoJsonRaw << endl;

            vector<string> begin_split = split(gameInfoJsonRaw, "[[");

            string begin = begin_split[0];

            vector<string> end_split = split(begin_split[1], "]]");

            string end = end_split[1].substr(1,  end_split[1].size() - 1);

            string customSerializedMap = end_split[0] + "]]";

            string gameInfo = begin + "PARSER DU JSON A LA MAIN CEST LE FUN" + end;

            auto gameInfoJson = json::parse(gameInfo);
            gameInfoJson["CustomSerializedMap"] = customSerializedMap;

            vector<vector<Tile>> deserialized_map = deserializeMap(customSerializedMap);

            // ###  ###

            
        }

    );

    request.reply(status_codes::OK, U(createMoveAction(Point(0,1))));

}

void AI::run() {

    listener.support(methods::POST, bind(&AI::requestHandler, this, std::placeholders::_1));


}

int main(int argc, char* argv[]) {

    string playerName;

    AI* bot = new AI("Svieg");

    bot->run();

    cout << "Server is running." << endl;

    while (true) {
        this_thread::sleep_for(chrono::milliseconds(1));
    };

}
