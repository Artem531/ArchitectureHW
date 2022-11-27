#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"

namespace database
{
    class Service{
        private:
            long _id;
            std::string _service_special_id;
            std::string _name;
            double _price;

        public:

            static Service fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_service_special_id() const;
            const std::string &get_name() const;
            double get_price() const;

            long&        id();
            std::string &service_special_id();
            std::string &name();
            double &price();

            static void init();
            static Service read_by_id(long id);
            static Service read_by_special_id(std::string service_special_id);
            static std::vector<Service> read_all();
            static std::vector<Service> search(std::string name);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif