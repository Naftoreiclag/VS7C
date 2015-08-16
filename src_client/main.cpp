
#include <iostream>
#include <vector>
#include <cstdlib>

#include "Version.h"

#include "InputManager.h"
#include "StringUtils.h"

#include "irrlicht.h"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

int main() {
    skm::Version version(0, 0, 0);

    std::cout << "SK Client" << std::endl;

    sf::UdpSocket clientSocket;

    unsigned short port = 25566;
    sf::IpAddress receiver = sf::IpAddress::LocalHost;

    sf::Packet connectReq;
    sf::Uint8 packType = 0;
    std::string username;

    std::cout << "Enter a name: " << std::endl;
    std::cin >> username;

    connectReq << packType << username;

    if(clientSocket.send(connectReq, receiver, port) != sf::Socket::Done) {
        std::cout << "Failed to connect to server!" << std::endl;
        std::system("pause");
    }

    skm::InputManager inputMgr;
    irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_SOFTWARE, irr::core::dimension2d<irr::u32>(640, 480), 16, false, false, false, &inputMgr);
    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();
    irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	if(!device) {
		return 1;
	}

    const wchar_t* caption = skm::StringUtils::toWideString("SK MMO Client Ver " + version.str());

	device->setWindowCaption(caption);

    irr::scene::IAnimatedMesh* mesh = smgr->getMesh("example_media/sydney.md2");
    if(!mesh) {
        device->drop();
        return 1;
    }
    irr::scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node->setMD2Animation(irr::scene::EMAT_STAND);
    node->setMaterialTexture(0, driver->getTexture("example_media/sydney.bmp"));

    smgr->addCameraSceneNode(0, irr::core::vector3df(0, 30, -40), irr::core::vector3df(0, 5, 0));

    while(device->run()) {

        if(inputMgr.isKeyDown(irr::KEY_KEY_Q)) {
            std::cout << "QQ" << std::endl;

        }

        driver->beginScene(true, true, irr::video::SColor(255, 101, 102, 141));
        smgr->drawAll();
        driver->endScene();

        /*
        sf::Packet chatPacket;

        std::string message;
        sf::Uint8 chatType = 1;
        std::cin >> message;
        chatPacket << chatType << message;

        if(clientSocket.send(chatPacket, receiver, port) != sf::Socket::Done) {
            // Explode
        }

        if(message == "stop") {
            runClient = false;
        }
        */
    }
}
