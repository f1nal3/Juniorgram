#include "Message.hpp"
#include "Connection.hpp"

namespace Network
{
 
    template <typename Archive>
    void serialize(Archive & ar, Message::MessageHeader & o)
    {
        ar& o.mMessageType& o.mBodySize& o.mTimestamp;
    }
    std::ostream& operator<<(std::ostream& os, const Message& message)
    {
        std::tm formattedTimestamp = Utility::safe_localtime(
            std::chrono::system_clock::to_time_t(message.mHeader.mTimestamp));

        os << "ID:" << size_t(message.mHeader.mMessageType) << " Size:" << message.mHeader.mBodySize
           << "Timestamp:" << std::put_time(&formattedTimestamp, "%F %T");
        return os;
    }

    bool operator<(const Message& lhs, const Message& rhs) 
    {
        return lhs.mHeader.mTimestamp < rhs.mHeader.mTimestamp;
    }

    bool operator>(const Message& lhs, const Message& rhs)
    {
        return lhs.mHeader.mTimestamp > rhs.mHeader.mTimestamp;
    }
    
}  // namespace Network
