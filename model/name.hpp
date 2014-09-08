#ifndef NAME_HPP
#define NAME_HPP

#include <string>
#include <memory>

namespace Api { namespace Model {

class Name
{
public:
    Name();
    virtual ~Name();

    std::string longName();
    void setLongName(std::string longName);

    std::string shortName();
    void setShortName(std::string shortName);

private:
    std::string mLongName;
    std::string mShortName;
};

} } // namespace Api::Model

#endif // NAME_HPP
