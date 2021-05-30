#include "QSQLCipherAdapter.hpp"

namespace DataAccess
{
std::shared_ptr<QSQLCipherAdapter> QSQLCipherAdapter::Instance(const std::string_view& options)
{
    return std::shared_ptr<QSQLCipherAdapter>();
}
std::optional<std::any> QSQLCipherAdapter::query(const std::string_view& query)
{
    return std::optional<std::any>();
}
pqxx::connection& QSQLCipherAdapter::getConnection(void)
{
    // TODO: вставьте здесь оператор return
}
bool QSQLCipherAdapter::isConnected(void) const { return false; }

void QSQLCipherAdapter::closeConnection(void) {}
}  // namespace DataAccess
