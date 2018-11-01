
#include "Grunt.h"
#include <iostream>

Grunt::Grunt(int startLane, TextureLoader* loadedTextures){
    lane = 0;

	grunt.setTexture(loadedTextures->mtSpriteSheet);
	grunt.setTextureRect(sf::IntRect(0,448,64,64));
	grunt.setOrigin(sf::Vector2f(32.f, 32.f));
	setLane(startLane);
	grunt.setPosition(1500, lane);
	grunt.setScale(sf::Vector2f(1.2f,1.2f));

	if (!gruntHitSound.loadFromFile("assets/Grunt_Hit.ogg")) // Loads and initializes all sounds based on impact
	    std::cout << "Could not load Grunt Hit Sound." << std::endl;
    if (!koratDeathSound.loadFromFile("assets/Korat_Death.ogg")) // Loads and initializes all sounds based on impact
		std::cout << "Could not load Korat Death Sound." << std::endl;
    if (!koratLeftSound.loadFromFile("assets/Korat_Left.ogg")) // Loads and initializes all sounds based on impact
		std::cout << "Could not load Korat Left Sound." << std::endl;
}

Grunt::~Grunt() {
	// TODO Auto-generated destructor stub

}

void Grunt::wasShot(int damage)
{
	health = health - damage;
	gruntWasHit.setBuffer(gruntHitSound);
	gruntWasHit.setVolume(75);
	gruntWasHit.play();
}

int Grunt::getLane()
{
    return lane;
}

std::string Grunt::getName()
{
    return "Grunt";
}

void Grunt::setLane(int givenLane)
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

void Grunt::moveCurrentKorat(float timePassed)
{
        if(grunt.getPosition().x > -100)
        {
            grunt.move(-speed * timePassed, 0);
        }
        else
        {
            survive = true;
        }
}

void Grunt::drawCurrentKorat(sf::RenderWindow& window)
{
    window.draw(grunt);
}

sf::Sprite Grunt::getKorat()
{
    return grunt;
}

float Grunt::getPositionX()
{
    return grunt.getPosition().x;
}

bool Grunt::checkDeath()
{
    if (health <= 0)
    {
		postDeathTime = postDeathClock.getElapsedTime().asSeconds();
		if (postDeathTime >= .5)
		{
			postDeathClock.restart();
			return true;
		}
		else
        {
			speed = 0;
			if (koratDeathSoundPlayed == false)
			{
				postDeathClock.restart();
				koratDied.setBuffer(koratDeathSound);
				koratDied.setVolume(100);
				koratDied.play();
				koratDeathSoundPlayed = true;
			}
		}
    }
    else
    {
        postDeathClock.restart();
        return false;

    }
}

bool Grunt::checkSurvive()
{
    return survive;
}

int Grunt::getHealth()
{
    return health;
}