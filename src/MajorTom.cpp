// Chris Wolinski

#include "MajorTom.h"
#include <iostream>
#include <algorithm>

using namespace std;

MajorTom::MajorTom(TextureLoader* loadedTextures)
{
    setGun(1);

    majorTom.setTexture(loadedTextures->textureArray[0]);
    spriteFrame.left = 0;//x
	spriteFrame.top = 0;//y
	spriteFrame.width = 64;
	spriteFrame.height = 64;
	majorTom.setTextureRect(spriteFrame);

    majorTom.setOrigin(sf::Vector2f(32.f, 32.f));
    majorTom.setPosition(156,508);
    majorTom.setScale(sf::Vector2f(1.2f,1.2f));

    healthBarBackground.setSize(sf::Vector2f(66.f, 8.f));
    healthBarBackground.setFillColor(sf::Color(30, 30, 30, 220));
    healthBarBackground.setOutlineColor(sf::Color::Black);
    healthBarBackground.setOutlineThickness(1.f);
    healthBarBackground.setOrigin(sf::Vector2f(33.f, 4.f));

    healthBarFill.setSize(sf::Vector2f(64.f, 6.f));
    healthBarFill.setFillColor(sf::Color(0, 220, 0));
    healthBarFill.setOrigin(sf::Vector2f(0.f, 3.f));

    setHealth(100);
    setSurvivors(20);
    setScore(0);
}

void MajorTom::drawTom (sf::RenderWindow& window)
{
    window.draw(majorTom);

    float clampedHealth = static_cast<float>(std::clamp(currentHealth, 0, maxHealth));
    float healthRatio = clampedHealth / static_cast<float>(maxHealth);
    float fillWidth = 64.f * healthRatio;

    sf::Vector2f tomPosition = majorTom.getPosition();
    sf::Vector2f barPosition(tomPosition.x, tomPosition.y - 52.f);

    healthBarBackground.setPosition(barPosition);
    healthBarFill.setPosition(sf::Vector2f(barPosition.x - 32.f, barPosition.y));
    healthBarFill.setSize(sf::Vector2f(fillWidth, 6.f));

    if (healthRatio > 0.6f)
        healthBarFill.setFillColor(sf::Color(0, 220, 0));
    else if (healthRatio > 0.3f)
        healthBarFill.setFillColor(sf::Color(240, 200, 0));
    else
        healthBarFill.setFillColor(sf::Color(220, 40, 40));

    window.draw(healthBarBackground);
    window.draw(healthBarFill);
}

float MajorTom::getTomPosition()
{
    return majorTom.getPosition().y;
}

float MajorTom::getTomPositionX()
{
    return majorTom.getPosition().x;
}

void MajorTom::setTomPositionX(float positionPassed)
{
    majorTom.setPosition(positionPassed ,majorTom.getPosition().y);
}

void MajorTom::setTomPositionY(float positionPassed)
{
    majorTom.setPosition(majorTom.getPosition().x, positionPassed);
}

/** \brief
 *
 * \return void
 *
 */
void MajorTom::incrementRunFrame()
{
    spriteFrame.left = (spriteFrame.left+64)%192;
    majorTom.setTextureRect(spriteFrame);
    if((majorTom.getPosition().y == 335)||(majorTom.getPosition().y == 422)||(majorTom.getPosition().y == 508)||(majorTom.getPosition().y == 594)||(majorTom.getPosition().y == 680))
    {
        spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
    }
}

/** \brief
 *
 * \param timePassed float
 * \param direction string
 * \return bool
 *
 */
bool MajorTom::initMove(float timePassed, string direction)
{
    if(direction == "Up")
    {
        if(majorTom.getPosition().y == 335)
        {
            //At Top
            return false;
        }
        else if(majorTom.getPosition().y == 422)
        {
            setTomPositionY(421);
            return keepMoving(timePassed, direction);
        }
        else if(majorTom.getPosition().y == 508)
        {
            setTomPositionY(507);
            return keepMoving(timePassed, direction);
        }
        else if(majorTom.getPosition().y == 594)
        {
            setTomPositionY(593);
            return keepMoving(timePassed, direction);
        }
        else if(majorTom.getPosition().y == 680)
        {
            setTomPositionY(679);
            return keepMoving(timePassed, direction);
        }
        else
        {
            return false;
        }
    }
    else if(direction == "Down")
    {
        if(majorTom.getPosition().y == 335)
        {
            setTomPositionY(336);
            return keepMoving(timePassed, direction);
        }
        else if(majorTom.getPosition().y == 422)
        {
            setTomPositionY(423);
            return keepMoving(timePassed, direction);
        }
        else if(majorTom.getPosition().y == 508)
        {
            setTomPositionY(509);
            return keepMoving(timePassed, direction);
        }
        else if(majorTom.getPosition().y == 594)
        {
            setTomPositionY(595);
            return keepMoving(timePassed, direction);
        }
        else if(majorTom.getPosition().y == 680)
        {
            //At Bottom
            return false;
        }
        else
        {
            return false;
        }
    }
    else return false;
}

/** \brief
 *
 * \param timePassed float
 * \param direction string
 * \return bool
 *
 */
bool MajorTom::keepMoving(float timePassed, string direction)
{
    if(direction == "Up")
    {
        if (majorTom.getPosition().y < 335)
        {
            setTomPositionX(238);
            setTomPositionY(335);
            return false;
        }
        else if (majorTom.getPosition().y < 422 && majorTom.getPosition().y > 335)
        {
            if(majorTom.getPosition().y > 335 && majorTom.getPosition().y < 345)
            {
                setTomPositionX(238);
                setTomPositionY(335);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
            else
            {
                majorTom.move(timePassed * 250, timePassed * -500);
                if(!((int)majorTom.getPosition().x % 5))
                    incrementRunFrame();
                return true;
            }
        }
        else if (majorTom.getPosition().y < 508 && majorTom.getPosition().y > 422)
        {
            if(majorTom.getPosition().y > 422 && majorTom.getPosition().y < 432)
            {
                setTomPositionX(197);
                setTomPositionY(422);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
            else
            {
                majorTom.move(timePassed * 250, timePassed * -500);
                if(!((int)majorTom.getPosition().x % 5))
                    incrementRunFrame();
                return true;
            }
        }
        else if (majorTom.getPosition().y < 594 && majorTom.getPosition().y > 508)
        {
            if(majorTom.getPosition().y > 508 && majorTom.getPosition().y < 518)
            {
                setTomPositionX(155);
                setTomPositionY(508);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
            else
            {
                majorTom.move(timePassed * 250, timePassed * -500);
                if(!((int)majorTom.getPosition().x % 5))
                    incrementRunFrame();
                return true;
            }
        }
        else if (majorTom.getPosition().y < 680 && majorTom.getPosition().y > 594)
        {
            if(majorTom.getPosition().y > 594 && majorTom.getPosition().y < 604)
            {
                setTomPositionX(114);
                setTomPositionY(594);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
            else
            {
                majorTom.move(timePassed * 250, timePassed * -500);
                if(!((int)majorTom.getPosition().x % 5))
                    incrementRunFrame();

                return true;
            }
        }
    }
    else if(direction == "Down")
    {
        if (majorTom.getPosition().y < 422 && majorTom.getPosition().y > 335)
        {

            if(majorTom.getPosition().y < 422 && majorTom.getPosition().y > 412)
            {
                setTomPositionX(197);
                setTomPositionY(422);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
            else
            {
                majorTom.move(timePassed * -250, timePassed * 500);
                if(!((int)majorTom.getPosition().x % 5))
                    incrementRunFrame();
                return true;
            }
        }
        else if (majorTom.getPosition().y < 508 && majorTom.getPosition().y > 422)
        {
            if(majorTom.getPosition().y < 508 && majorTom.getPosition().y > 498)
            {
                setTomPositionX(155);
                setTomPositionY(508);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
            else
            {
                majorTom.move(timePassed * -250, timePassed * 500);
                if(!((int)majorTom.getPosition().x % 5))
                    incrementRunFrame();
                return true;
            }
        }
        else if (majorTom.getPosition().y < 594 && majorTom.getPosition().y > 508)
        {
            if(majorTom.getPosition().y < 594 && majorTom.getPosition().y > 584)
            {
                setTomPositionX(114);
                setTomPositionY(594);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
            else
            {
                majorTom.move(timePassed * -250, timePassed * 500);
                if(!((int)majorTom.getPosition().x % 5))
                    incrementRunFrame();
                return true;
            }
        }
        else if (majorTom.getPosition().y < 680 && majorTom.getPosition().y > 594)
        {
            if(majorTom.getPosition().y < 680 && majorTom.getPosition().y > 670)
            {
                setTomPositionX(72);
                setTomPositionY(680);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
            else
            {
                majorTom.move(timePassed * -250, timePassed * 500);
                if(!((int)majorTom.getPosition().x % 5))
                    incrementRunFrame();
                return true;
            }
        }
        else if (majorTom.getPosition().y > 680)
            {
                setTomPositionX(72);
                setTomPositionY(680);
                spriteFrame.left = 0;
        majorTom.setTextureRect(spriteFrame);
                return false;
            }
        }
}

sf::Sprite MajorTom::getTom()
{
    return majorTom;
}

void MajorTom::wasShot(int damage)
{
    currentHealth = std::max(0, currentHealth - damage);

}

int MajorTom::getHealth()
{
    return currentHealth;
}

void MajorTom::setHealth(int newHealth)
{
    currentHealth = std::clamp(newHealth, 0, maxHealth);
}

int MajorTom::getScore()
{
    return score;
}

void MajorTom::setScore(int num)
{
    score = num;
}

int MajorTom::getGun()
{
    return currentGun;
}

/** \brief
 *
 * \param gunNumber int
 * \return void
 *
 */
void MajorTom::setGun(int gunNumber)
{
    switch(gunNumber)
    {
        case 1:
            currentGun = 1;
            break;
        case 2:
            currentGun = 2;
            break;
        case 3:
            currentGun = 3;
            break;
        case 4:
            currentGun = 4;
            break;
        case 5:
            currentGun = 5;
            break;
        case 6:
            currentGun = 6;
            break;
        case 7:
            currentGun = 7;
            break;
        default:
            currentGun = 1;
            break;

    }
    //change the frame series to the correct gun and reset the texture
    spriteFrame.top = (gunNumber - 1)*64;
    majorTom.setTextureRect(spriteFrame);
}

int MajorTom::getSurvivors()
{
    return survivorCnt;
}

void MajorTom::setSurvivors(int num)
{
    survivorCnt = num;
}

bool MajorTom::checkDeath()
{
    if (currentHealth <= 0)
        return true;
    return false;
}

