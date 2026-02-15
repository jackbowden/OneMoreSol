// Chris Wolinski

#include "TankBoss.h"
#include <iostream>
#include <algorithm>

using namespace std;

TankBoss::TankBoss(TextureLoader* loadedTextures)
{
    tankBoss.setTexture(loadedTextures->textureArray[0]);
    spriteFrame.left = 512;//x
	spriteFrame.top = 448;//y
	spriteFrame.width = 320;
	spriteFrame.height = 320;
    tankBoss.setTextureRect(spriteFrame);
    tankBoss.setOrigin(sf::Vector2f(160.f,160.f));
    tankBoss.setScale(1.2f,1.2f);
    tankBoss.setPosition(1756, lane3);

    healthBarBackground.setSize(sf::Vector2f(322.f, 14.f));
    healthBarBackground.setFillColor(sf::Color(30, 30, 30, 220));
    healthBarBackground.setOutlineColor(sf::Color::Black);
    healthBarBackground.setOutlineThickness(1.f);
    healthBarBackground.setOrigin(sf::Vector2f(161.f, 7.f));

    healthBarFill.setSize(sf::Vector2f(320.f, 12.f));
    healthBarFill.setFillColor(sf::Color(0, 220, 0));
    healthBarFill.setOrigin(sf::Vector2f(0.f, 6.f));

    dieBool = false;
}

void TankBoss::moveBoss(float timePassed)
{
    if(tankBoss.getPosition().x > -100)
    {
        tankBoss.move(-speed * timePassed, 0);
        if(!((int)tankBoss.getPosition().x % 3))//slows down the switching of frames
        {
            incrementRunFrameBoss(&spriteFrame, &tankBoss);
        }
    }
    else
    {
        survive = true;
    }
}

void TankBoss::incrementRunFrameBoss(sf::IntRect* sF, sf::Sprite* baddie)
{
    //start with the upper left coordinate, add the sprite width to it
    //to move to the next frame. To loop, make circular arith with mod
    // 3*192 is 576, as mod circles back to 0, add the initial value 832.
    //should 832 be added at every increment? no
    sF->left = ((sF->left +320)%1792); //+832
    if(sF->left == 0) {sF->left += 512;}
    baddie->setTextureRect(*sF);
}

bool TankBoss::checkDeath()
{
    if (health <= 0)
    {
		postDeathTime = postDeathClock.getElapsedTime().asSeconds();
		if (postDeathTime >= 5)
		{
			postDeathClock.restart();
			return true;
		}
		else
        {
			speed = 0;

            if(dieBool == false)
            {
                dieBool = true;
                spriteFrame.left = 384;//576;//x
                spriteFrame.top = 768;//y
                spriteFrame.width = 192;
                spriteFrame.height = 192;
                tankBoss.setOrigin(sf::Vector2f(96.f,96.f));
                tankBoss.setScale(sf::Vector2f(2.5,2.5));
            }

            spriteFrame.left = ((spriteFrame.left + 192)%1344);//adjust for sprite location
            if(spriteFrame.left == 0) {spriteFrame.left += 960;}//put on frame 3 to cycle plosion
            tankBoss.setTextureRect(spriteFrame);

			return false;
		}
    }
    else
    {
        postDeathClock.restart();
        return false;

    }
}

bool TankBoss::checkSurvive()
{
    return survive;
}

float TankBoss::getPositionX()
{
    return tankBoss.getPosition().x - 165;
}

sf::Sprite TankBoss::getBoss()
{
    return tankBoss;
}

void TankBoss::wasShot(int damage)
{
    health = std::max(0, health - damage);
}

int TankBoss::getSpeed()
{
    return speed;
}

bool TankBoss::queryToFire()
{
	bool readyToFire = false;
	lastBulletFired = fireBulletClock.getElapsedTime().asSeconds();

    // 0.67415730337
	if(lastBulletFired > 0.33707865168)
	{
	   readyToFire = true;
	   fireBulletClock.restart();
	}

return readyToFire;
}

int TankBoss::decideFiringLane()
{
    int lane = firingLane[index];
    index++;
    return lane;
}

void TankBoss::drawBoss(sf::RenderWindow& window)
{
    window.draw(tankBoss);

    if (health <= 0)
    {
        return;
    }

    float clampedHealth = static_cast<float>(std::clamp(health, 0, maxHealth));
    float healthRatio = clampedHealth / static_cast<float>(maxHealth);
    float fillWidth = 320.f * healthRatio;

    sf::Vector2f bossPosition = tankBoss.getPosition();
    sf::Vector2f barPosition(bossPosition.x, bossPosition.y - 198.f);

    healthBarBackground.setPosition(barPosition);
    healthBarFill.setPosition(sf::Vector2f(barPosition.x - 160.f, barPosition.y));
    healthBarFill.setSize(sf::Vector2f(fillWidth, 12.f));

    if (healthRatio > 0.6f)
        healthBarFill.setFillColor(sf::Color(0, 220, 0));
    else if (healthRatio > 0.3f)
        healthBarFill.setFillColor(sf::Color(240, 200, 0));
    else
        healthBarFill.setFillColor(sf::Color(220, 40, 40));

    window.draw(healthBarBackground);
    window.draw(healthBarFill);
}
