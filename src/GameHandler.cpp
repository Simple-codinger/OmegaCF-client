#include "../include/GameHandler.hpp"

GameHandler::GameHandler(std::string host, unsigned short port){
    Debug::printLine("Init ai");
    
    if(DataHandlingService::getInstance().start(host, port) < 0){
        //cant connect to the server
        Debug::printLine("Problems connecting to the server");
        exit(0);
    }
    
}

void GameHandler::run(){
    Debug::printLine("Run AI");
    // receive playerNumber
    ServerNetworkMessage configurationMessage = DataHandlingService::getInstance().receiveMessage();
    while (configurationMessage.getMessageType() != NetworkMessageType::Configuration) {
        if(configurationMessage.getMessageType() == NetworkMessageType::Disqualification) {
            handleDisqualification(configurationMessage);
        } else {
            Debug::printLine("Unexpected message type!");
            exit(0);
        }
        configurationMessage = DataHandlingService::getInstance().receiveMessage();
    }
    // init player
    this->_playerNumber = configurationMessage.Move.playerNumber;
    // init game
    this->_game = GameFactory::create(configurationMessage.Move.x, configurationMessage.Move.y, 2);

    //this is for testing purposes only!!!
    this->_game.setStone(this->_game.Players[0], 0, this->_game.CurrentMap);
    this->_game.setStone(this->_game.Players[1], 0, this->_game.CurrentMap);

    for(PossibleMove move : this->_game.getPossibleMoves(this->_game.Players[0], this->_game.CurrentMap)){
        std::cout << "Possible Move: "  << move.Move << std::endl;
        std::cout << "Map (after it):" << std::endl;
        std::cout << move.AfterGrid << std::endl;
    }
    
    Debug::printLine("End of the game. Bye!");
}

void GameHandler::handleDisqualification(ServerNetworkMessage message) {
    uint8_t disNumber = message.getMessage().at(0);
    if (this->_playerNumber == disNumber) {
        Debug::printLine("DISQUALIFIED!");
        exit(0);
    }
}

GameHandler::~GameHandler(){
}