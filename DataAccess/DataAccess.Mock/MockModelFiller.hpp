#pragma once
#include <Models/UnifyedModel.hpp>


namespace MockObject
{
	template <class TEnum, class TResult = void>
	class MockModelFiller final: Models::UnifiedModel<TEnum, TResult>
	{
    public:
        MockModelFiller(const std::string& modelName, size_t amountFields)
            : Models::UnifiedModel<TEnum, TResult>(modelName, amountFields)
        {}

    public:
        void fill(const TResult& responce) const final
        {
            /// do nothing
        }
    protected:
        virtual TEnum getNumEnum(size_t) const = 0;
	};
}
