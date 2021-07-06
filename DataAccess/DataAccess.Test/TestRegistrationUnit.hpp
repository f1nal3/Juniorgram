#include <DataAccess.Static/PostgreTable.hpp>
#include <DataAccess.Static/RepositoryUnits.hpp>
#include <string>

class TestRegistrationUnit : public RegistrationUnit
{
    TestRegistrationUnit() : RegistrationUnit(new Table("testing", "addr port dbname")) 
    { }
};
