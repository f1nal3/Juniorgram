#include <Utility/Utility.hpp>

namespace Utility
{
    std::string removeSpaces(const std::string& input)
    {
        auto isSpace = [] (char c) -> bool { return std::isspace(static_cast<unsigned char>(c));  };

        std::string result;
        result.reserve(input.size());

        auto sequenceStart = std::find_if_not(input.cbegin(), input.cend(), isSpace);
        auto sequenceEnd = sequenceStart;

        while (sequenceStart < input.end())
        {
            bool spaceSequence = isSpace(*sequenceStart);

            if (spaceSequence)
            {
                char singleSpace = ' ';

                for (; (sequenceEnd < input.end()) && isSpace(*sequenceEnd); ++sequenceEnd)
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
                sequenceEnd = std::find_if(sequenceStart, input.cend(), isSpace);
                result.append(sequenceStart, sequenceEnd);
            }

            sequenceStart = sequenceEnd;
        }

        if (!result.empty() && isSpace(result.back()))
        {
            result.pop_back();
        }

        return result;
    }
}
