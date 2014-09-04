#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <string>
#include <map>
#include "model/baseObject.hpp"

namespace Api
{
    namespace Model
    {
        class Namespace
        {
        public:
            Namespace();

            std::string name();
            void setName(std::string name);

            BaseObjectPtr object(std::string name);
            std::map<std::string, BaseObjectPtr> objects();
            void addObject(std::string name, BaseObjectPtr object);

        private:
            std::string mName;
            std::map<std::string, BaseObjectPtr> mObjects;
        };
    }
}

#endif // NAMESPACE_H
