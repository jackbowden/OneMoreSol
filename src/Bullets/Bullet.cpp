#include "Bullet.h"

/** \brief Bullet constructor
 *
 * \param positionX,PositionY location of bullet
 * \param damage effect of bullet
 * \param velocity speed of bullet
 *
 */
Bullet::Bullet(sf::Vector2f size)
{
    bullet.setSize(size);
    float positionX = 5;
    float positionY = 10;
    float damage = 100;
    float velocity = 100;
}

/** \brief Getter for bullet velocity
 *
 * \return Velocity float
 *
 */

float Bullet::getVelocity()
{
    return velocity;
}

/** \brief Setter for bullet velocity
 *
 * \param newVelocity new velocity for bullet
 *
 */

void Bullet::setVelocity(float newVelocity)
{
    velocity = newVelocity;
}

float Bullet::getPositionX()
{
    return positionX;
}

float Bullet::getPositionY()
{
    return positionY;
}

float Bullet::setPosition(sf::Vector2f newPos)
{
    positionX = newPos.x;
    positionY = newPos.y;
}

void Bullet::fire(float timePassed)
{
    bullet.move(velocity*timePassed, 0);
}

Bullet::doDamage()
{

}

Bullet::~Bullet()
{
    //dtor
}
