
#include "Minitank.h"
#include <iostream>

Minitank::Minitank(int startLane, TextureLoader* loadedTextures){

    lane = 0;

	biker.setTexture(loadedTextures->textureArray[0]);
	spriteFrame.left = 0;//x
	spriteFrame.top = 832;//y
	spriteFrame.width = 64;
	spriteFrame.height = 62;
	biker.setTextureRect(spriteFrame);
	biker.setOrigin(sf::Vector2f(32.f,32.f));
	setLane(startLane);
	biker.setPosition(1500, lane);
	biker.setScale(sf::Vector2f(1.2f,1.2f));

}

Minitank::~Minitank() {
	// TODO Auto-generated destructor stub

}

void Minitank::wasShot(int damage)
{
    health = health - damage;
}

int Minitank::getLane()
{
    return lane;
}

std::string Minitank::getName()
{
    return "Minitank";
}

/** \brief
 *
 * \param givenLane int
 * \return void
 *
 */
void Minitank::setLane(int givenLane)
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

void Minitank::moveCurrentKorat(float timePassed)
{
        if(biker.getPosition().x > -100)
        {
            biker.move(-speed * timePassed, 0);
            if(!((int)biker.getPosition().x % 10))
                KoratEmpire::incrementRunFrame(&spriteFrame, &biker);
        }
        else
        {
            survive = true;
        }
}

void Minitank::drawCurrentKorat(sf::RenderWindow& window)
{
    window.draw(biker);
}

sf::Sprite Minitank::getKorat()
{
    return biker;
}

float Minitank::getPositionX()
{
    return biker.getPosition().x;
}

/** \brief
 *
 * \return bool
 *
 */
bool Minitank::checkDeath()
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
				koratDeathSoundPlayed = true;
			}
			spriteFrame.left = 192;//x
            spriteFrame.top = 832;//y
            spriteFrame.width = 64;
            spriteFrame.height = 64;
            biker.setTextureRect(spriteFrame);
			return false;
		}
    }
    else
    {
        postDeathClock.restart();
        return false;

    }
}

bool Minitank::checkSurvive()
{
    return survive;
}

int Minitank::getHealth()
{
    return health;
}

int Minitank::getSpeed()
{
    return speed;
}

double Minitank::getFireRate()
{
	return fireRate;
}

void Minitank::setFireRate(double givenFireRate)
{
	fireRate = givenFireRate;
}

/** \brief
 *
 * \return bool
 *
 */
bool Minitank::queryToFire()
{
	bool readyToFire = false;
	lastBulletFired = fireBulletClock.getElapsedTime().asSeconds();

	if(lastBulletFired > getFireRate())
	{
	   readyToFire = true;

	   double randomFireRate = Random() * 3;
	   if(randomFireRate > 1 && randomFireRate < 2)
		  setFireRate(2);
	   else if(randomFireRate > 2)
		  setFireRate(3);
	   else
		   setFireRate(1);
	   setFireRate(randomFireRate * 2);
	   fireBulletClock.restart();
	}


	return readyToFire;
}
