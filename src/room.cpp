#include "room.h"

Room::Room(int argc, char *argv[]) {
    switch (argc)
    {
    case 3:
         prepare(std::move(argv[1]),std::move(argv[2]));
        break;
    case 2:
        prepare(std::move(argv[1]));
    default:
        prepare();
        break;
    }
}



Room::~Room() {
    access.reset();
    name.reset();
    hash.reset();
}


void Room::prepare(string _name, string _secret) {
 name = make_unique<Name>(_name);

 if(name->verify()) {
     name->setName(_name);
 }
 access = make_unique<Seed>((string)(_secret + name->getName() + access->genRandInit()));
 access->genSeed();
 hash = make_shared<string>(access->getSeed());
}


//sobrecarga de PREAPRE
void Room::prepare(string _secret) {
    name = make_unique<Name>();
    access = make_unique<Seed>((string)(_secret+name->getName() + access->genRandInit()));
    access->genSeed(); 
    hash = make_shared<string>(access->getSeed());
}



string Room::getMeSecret() {
    string seed = access->genRandInit();
    return access->SHA(seed);
}


void Room::prepare() {
    name = make_unique<Name>();
     access = make_unique<Seed>((string)("public"+name->getName() + access->genRandInit()));
     access->genSeed();
     hash = make_shared<string>(access->getSeed());
}


string Room::Launch() {

  message_secret = make_shared<string>(getMeSecret());

    PARAMS<string> post_field = {
        "hash=",
         *hash,
        "name=",
         name->getName(),
         "secret=",
         *message_secret
    };

    http = make_unique<HTTP>(RWNIO_HTTP_URL);
    http->post("launch", post_field.transform());
    auto res = http->Response();
    http.reset();
    return res;
}
  
