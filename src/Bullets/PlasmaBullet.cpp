
#include "PlasmaBullet.h"
#include <iostream>

PlasmaBullet::PlasmaBullet(int startLane) {
    //if(!gruntPlasma.loadFromFile("assets/plasmaGrunt.png"))
        //std::cout << "Failed to load plasmaGrunt." << std::endl;
    plasma.setSize(sf::Vector2f(16,16));
	//plasma.setTexture(&gruntPlasma);
	plasma.setOrigin(plasma.getSize().x / 2, plasma.getSize().y /2);
	setLane(startLane);
	plasma.setPosition(100, lane);
}

PlasmaBullet::~PlasmaBullet() {
	// TODO Auto-generated destructor stub
}

int PlasmaBullet::getLane()
{
    return lane;
}

void PlasmaBullet::setLane(int givenLane)
{
     switch(givenLane)
	{
		case 1:
			lane = lane1;
			break;
		case 2:
			lane = lane2;
			break;
		case 3:
			lane = lane3;
			break;
		case 4:
			lane = lane4;
			break;
		case 5:
			lane = lane5;
			break;
		default:
			lane = lane1;
			break;
	}
}

void PlasmaBullet::moveCurrentBullet(float timePassed)
{
        if(plasma.getPosition().x < 2000)
        {
            plasma.move(speed * timePassed, 0);
        }
        else
        {

        }
}

void PlasmaBullet::drawCurrentBullet(sf::RenderWindow& window)
{
    window.draw(plasma);
}

sf::RectangleShape PlasmaBullet::getBullet()
{
    return plasma;
}

