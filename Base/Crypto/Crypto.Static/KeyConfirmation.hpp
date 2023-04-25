#pragma once

namespace Base::KeyConfirmation
{
/** @class KeyConfirmation
* @brief Interface for key confirmation protocol
* @details Designed for used in Connection class to compare server's and client's keys after generating it
* by IKeyAgreement
*/
template <typename T>
class KeyConfirmation
{
public:
    KeyConfirmation(const T& verificationUnit_): _verificationUnit(verificationUnit_)
    {
    };
    /// @brief Method returns specific object used to verify the key
    T getVerificationUnit() const { return _verificationUnit; };
    /** @brief Method for comparing received test object with correct verification object
    * @details You need to overload operator== for template class, if there is none
    */
    bool compareWithTestUnit(const T& objToCompare) { return objToCompare == _verificationUnit; };

protected:
    const T _verificationUnit;
};
}  // namespace Base::KeyConfirmation
