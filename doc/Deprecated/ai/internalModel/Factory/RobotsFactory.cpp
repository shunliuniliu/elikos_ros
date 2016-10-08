/*
 * RobotsFactory.cpp
 *
 *  Created on: 2015-02-27
 *      Author: myriam
 */

#include "RobotsFactory.hpp"
#include "TargetRobotFactory.hpp"
#include "ObstacleRobotFactory.hpp"
#include "QuadRobotFactory.hpp"
#include <utility>

namespace elikos_ai {

/* *************************************************************************************************
 * ***           STATIC ATTRIBUTES INITIALIZATION
 * *************************************************************************************************
 */

RobotsFactory RobotsFactory::instance_ = RobotsFactory();


/* *************************************************************************************************
 * ***           CONSTRUCTER, DESTRUCTOR
 * *************************************************************************************************
 */

RobotsFactory::RobotsFactory()
{
    // Create all robots' factories
    factoriesMap_[groundRobot] = new TargetRobotFactory();
    factoriesMap_[obstacleRobot] = new ObstacleRobotFactory();
    factoriesMap_[quadRobot] = new QuadRobotFactory();
}

RobotsFactory::~RobotsFactory()
{
    // Delete all robots' factories
    for ( std::map<RobotType, RobotFactoryBase*>::iterator it = factoriesMap_.begin(); it != factoriesMap_.end(); ++it )
    {
        delete it->second;
        it->second = 0;
    }

    factoriesMap_.clear();
}

/* *************************************************************************************************
 * ***           PUBLIC FACTORY FUNCTIONS
 * *************************************************************************************************
 */

/**
 * @fn      newRobot( RobotType robotType, int id, tf::Point relativePosition, float orientation )
 * @brief   Creates a robot of type robotType. Otherwise returns 0.
 */
Robot* RobotsFactory::newRobot( RobotType robotType, int id, tf::Point relativePosition, float orientation )
{
    std::map<RobotType, RobotFactoryBase*>::iterator it = factoriesMap_.find( robotType );
    if ( it != factoriesMap_.end() )
    {
        return it->second->newRobot( id, relativePosition, orientation );
    }
    else return 0;
}

/**
 * @fn      newRobot( RobotType robotType, int id, tf::Point relativePosition, float orientation, tf::Vector3 speed )
 * @brief   Creates a robot of type robotType. Otherwise returns 0.
 */
Robot* RobotsFactory::newRobot( RobotType robotType, int id, tf::Point relativePosition, float orientation, tf::Vector3 speed )
{
    std::map<RobotType, RobotFactoryBase*>::iterator it = factoriesMap_.find( robotType );
    if ( it != factoriesMap_.end() )
    {
        return it->second->newRobot( id, relativePosition, orientation, speed );
    }
    else return 0;
}


} // namespace elikos_ai