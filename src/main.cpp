#include "../include/GameHandler.hpp"
#include "../include/util/Debug.hpp"
#include <iostream>
#include <string>
#include <exception>


#include "../include/ml/networkAgent.hpp"
#include "../include/ml/network.hpp"
#include <tuple>
#include <utility>
#include "../include/ml/replayMemory.hpp"
#include "../include/game/Game.hpp"

static void printHelp(){
	std::cout << "-i --ip\t IP-Adress (default 127.0.0.1)" << std::endl;
	std::cout << "-p --port\t Port (default 7777)" << std::endl;
	std::cout << "-q --quiet\t Disable all output" << std::endl; 
	std::cout << "-h --help\t print help" << std::endl;
}

int main(int argc, char *argv[]) {
	Network net;
	NetworkAgent netAgent(net);

	Game game = GameFactory::create(7, 6, 2);
	game.setStone(game.getPlayer(1), 6, game.CurrentMap);
	game.setStone(game.getPlayer(2), 2, game.CurrentMap);
	netAgent.evalPosition(game.CurrentMap, 1);
	netAgent.save();

	ReplayMemory<std::tuple<int, int, int>> rm(2);
	std::tuple<int, int, int> t1 = std::make_tuple(3, 4, 5);
	std::tuple<int, int, int> t2 = std::make_tuple(6, 7, 8);
	rm.push_back(t1);
	rm.push_back(t2);
	std::vector<std::tuple<int, int, int>> v(1);
	if (!rm.getSample(1, v))
		std::cout << "getSample error" << std::endl;
	std::cout << get<0>(v.at(0)) << std::endl;

	std::string ip = "127.0.0.1";
	unsigned short port = 7777;

	return 0;

	Debug::setFlag(true);

	if(argc > 1){
		for(int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			if((arg == "-h") || (arg == "--help")){
				printHelp();
				return 0;
				break;
			}else if((arg == "-i") || (arg == "--ip")){
				if (i + 1 < argc) {
					ip = argv[i+1];
				}
			}else if((arg == "-p") || (arg == "--port")){
				if (i + 1 < argc) {
					std::string s = argv[i+1];
					port = std::stoi(s, nullptr);
				}				
			}
			else if((arg == "-d") || (arg == "--debug")){
				Debug::setFlag(true);				
			}
			else if((arg == "-q") || (arg == "--quiet")){
				Debug::setFlag(false);			
			}
		}
	}
	try {
		GameHandler *gameHandler = new GameHandler(ip, port);
		gameHandler->run();
	}catch(std::exception &err){
		std::cout << err.what() << std::endl;
	}catch(const char* c){
		std::cout << "Error: " << c << std::endl;
	}
	catch(...){
		std::cout << "Unknown error";
	}
	
	return 0; 
} 