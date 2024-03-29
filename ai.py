from flask import Flask, request
from structs import *
import json
import numpy

app = Flask(__name__)


def create_action(action_type, target):
    actionContent = ActionContent(action_type, target.__dict__)
    return json.dumps(actionContent.__dict__)


def create_move_action(target):
    return create_action("MoveAction", target)


def create_attack_action(target):
    return create_action("AttackAction", target)


def create_collect_action(target):
    return create_action("CollectAction", target)


def create_steal_action(target):
    return create_action("StealAction", target)


def create_heal_action():
    return create_action("HealAction", "")


def create_purchase_action(item):
    return create_action("PurchaseAction", item)


def deserialize_map(serialized_map):
    """
    Fonction utilitaire pour comprendre la map
    """
    serialized_map = serialized_map[1:]
    rows = serialized_map.split('[')
    column = rows[0].split('{')
    deserialized_map = [[Tile() for x in range(40)] for y in range(40)]
    for i in range(len(rows) - 1):
        column = rows[i + 1].split('{')

        for j in range(len(column) - 1):
            infos = column[j + 1].split(',')
            end_index = infos[2].find('}')
            content = int(infos[0])
            x = int(infos[1])
            y = int(infos[2][:end_index])
            deserialized_map[i][j] = Tile(content, x, y)

    return deserialized_map


def reverse_range(range):
    x = range - 1
    while (x >= 0):
        yield x;
        x -= 1


def bot():
    """
    Main de votre bot.
    """
    map_json = request.form["map"]

    # Player info

    encoded_map = map_json.encode()
    map_json = json.loads(encoded_map)
    p = map_json["Player"]
    print "player:{}".format(p)
    pos = p["Position"]
    x = pos["X"]
    y = pos["Y"]
    house = p["HouseLocation"]
    player = Player(p["Health"], p["MaxHealth"], Point(x, y),
                    Point(house["X"], house["Y"]), p["Score"],
                    p["CarriedResources"], p["CarryingCapacity"])

    # Map
    serialized_map = map_json["CustomSerializedMap"]
    deserialized_map = deserialize_map(serialized_map)

    otherPlayers = []

    for players in map_json["OtherPlayers"]:
        player_info = players["Value"]
        p_pos = player_info["Position"]
        player_info = PlayerInfo(player_info["Health"],
                                 player_info["MaxHealth"],
                                 Point(p_pos["X"], p_pos["Y"]))

        otherPlayers.append(player_info)

    target = Point(0, 0)
    nextPosition = Point(0, 0)
    for i in range(20):
        for j in range(20):
            if deserialized_map[j][i].Content == TileContent.Resource:
                distanceRestant = target.__sub__(player.Position)
                target.X = i + (distanceRestant.X)
                target.Y = j + (distanceRestant.Y)

                distancePoint = target.__sub__(player.Position)

            print deserialized_map[j][i].Content,
        print

        if distancePoint.X < 0:
            nextPosition = player.Position + Point(-1, 0)
        elif distancePoint.X > 0:
            nextPosition = player.Position + Point(1, 0)
        elif distancePoint.Y < 0 & distancePoint.X == 0:
            nextPosition = player.Position + Point(0, -1)
        elif distancePoint.Y > 0 & distancePoint.X == 0:
            nextPosition = player.Position + Point(0, 1)

        print player.Position
        print target

    return create_move_action(nextPosition)
    # return create_move_action(Point(0, 0))


@app.route("/", methods=["POST"])
def reponse():
    """
    Point d'entree appelle par le GameServer
    """
    return bot()


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080, debug=True)
