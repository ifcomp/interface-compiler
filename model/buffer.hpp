#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <memory>
#include "model/namespace_member.hpp"

namespace Api { namespace Model {

class Buffer : public NamespaceMember
{
public:
    enum class BufferType
    {
        BUFFER,
        CONST_BUFFER
    };

private:
    BufferType mType;

public:
    Buffer();
    virtual ~Buffer();

    BufferType type();
    void setType(BufferType type);
};

typedef std::shared_ptr<Buffer> BufferPtr;

} } // namespace Api::Model

#endif // BUFFER_HPP
