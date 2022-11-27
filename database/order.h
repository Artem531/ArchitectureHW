#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"

namespace database
{
    class Order{
        private:
            long _id;
            long _user_id;
            //std::string _created;

        public:

            static Order fromJSON(const std::string & str);

            long get_id() const;
            long get_user_id() const;

            long& id();
            long& user_id();

            static void init();
            static std::vector<Order> read_by_id(long id);
            static std::vector<Order> read_all();
            
            static void add_to_order(long _id, long service_id);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif