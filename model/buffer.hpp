#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include "model/baseObject.hpp"

namespace Api
{
    namespace Model
    {
        class Buffer : public BaseObject
        {
        public:
            enum BufferType
            {
                BUFFER,
                CONST_BUFFER
            };

        private:
            BufferType mType;
            std::string mMapTo;

        public:
            Buffer();

            BufferType type();
            void setType(BufferType type);

            std::string mapTo();
            void setMapTo(std::string mapTo);
        };
    }
}

#endif // BUFFER_HPP
