#pragma once

namespace Base::KeyConfirmators
{
/** @class KeyConfirmation
* @brief Interface for key confirmation protocol
* @details Designed for used in Connection class to compare server's and client's keys after generating it
* by IKeyAgreement
*/
template <typename T = std::string>
class KeyConfirmation
{
public:
    KeyConfirmation() = default;

    KeyConfirmation(const T& verificationUnit_): _verificationUnit(verificationUnit_)
    {
    };
    /// @brief Method returns specific object used to verify the key
    T getVerificationUnit() const { return _verificationUnit; }
    /** @brief Method for comparing received test object with correct verification object
    * @details You need to overload operator== for template class, if there is none
    */
    bool compareWithTestUnit(const T& objToCompare) { return objToCompare == _verificationUnit; };

protected:
    const T _verificationUnit = "testMessage123";
};
}  // namespace Base::KeyConfirmation
