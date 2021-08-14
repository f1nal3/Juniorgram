#include "Application.hpp"
#include "message.hpp"

int main(int argc, char* argv[])
{
    Application* qapp = new Application(argc, argv);
    qapp->create();
    qapp->show();
    /// Server
    flatbuffers::FlatBufferBuilder builder;
    {
        auto header      = Monster::CreateHeader(builder, 54, 54);
        auto channels    = {Monster::CreateChannelDirect(builder, 4, "test2"), Monster::CreateChannelDirect(builder, 3, "test1"),
                         Monster::CreateChannelDirect(builder, 5, "test5")};
        auto vector      = builder.CreateVector(channels.begin(), 3);
        auto channelList = Monster::CreateChannelListAnswer(builder, vector);

        auto monster = Monster::CreateMessage(builder, header, Monster::MessageBody::ChannelListAnswer, channelList.Union());
        builder.Finish(monster);
    }

    /// Client
    auto msg1 = Monster::GetMessageA(builder.GetBufferPointer());
    std::cout << builder.GetSize() << std::endl;

    std::cout << msg1->body_as_ChannelListAnswer()->channels()->size() << std::endl;

    return qapp->exec();
}
