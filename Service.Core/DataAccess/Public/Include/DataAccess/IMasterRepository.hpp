#pragma once

#include <iostream>
#include <memory>

namespace DataAccess
{

/**
*  @class IMasterRepository
 * @brief The IMasterRepository base abstract class
 * @details Class used as a base of interfaces for repositories on client
 * and service sides. The class uses polymorphism to register existing repositories.
 * The repositories can be located at client or service sides. The name of repository is defined
 * by class of object, which will be processed (e.g. ChannelsRepository handle object of ChannelInfo class). \n
 * Each database has it`s own full set of implemented repositories .
 */
struct IMasterRepository
{
    /**
    * @brief virtual destructor.
    */
    virtual ~IMasterRepository() = default;
};
}  // namespace DataAccess
