#include <Utility/Utility.hpp>

namespace Utility
{
    std::string removeSpaces(const std::string& input)
    {
        std::string result;
        result.reserve(input.size());

        auto sequenceStart = std::find_if_not(input.cbegin(), input.cend(), std::isspace);
        auto sequenceEnd = sequenceStart;

        while (sequenceStart < input.end())
        {
            bool spaceSequence = std::isspace(*sequenceStart);

            if (spaceSequence)
            {
                char singleSpace = ' ';

                for (; (sequenceEnd < input.end()) && std::isspace(*sequenceEnd); ++sequenceEnd)
                {
                    if (*sequenceEnd == '\n')
                    {
                        singleSpace = '\n';
                    }
                }

                result.push_back(singleSpace);
            }
            else
            {
                sequenceEnd = std::find_if(sequenceStart, input.cend(), std::isspace);
                result.append(sequenceStart, sequenceEnd);
            }

            sequenceStart = sequenceEnd;
        }

        if (!result.empty() && std::isspace(result.back()))
        {
            result.pop_back();
        }

        return result;
    }
}
