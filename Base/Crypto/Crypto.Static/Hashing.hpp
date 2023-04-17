#include <string>

namespace Base::Hashing
{
/**@brief SHA256 calculating method with salt
* @details The typical use is calculating user's password consider his login
*/
std::string SHA_256(const std::string& message, const std::string& componentForSalt);

/**@brief SHA256 calculating method without salt
 * @details The typical use is calculating connection verification hash.
 */
std::string SHA_256(const std::string& plainText);
}
