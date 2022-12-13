#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"

namespace database
{
    class User{
        private:
            long _id;
            std::string _first_name;
            std::string _last_name;
            std::string _email;
            std::string _title;
            std::string _type;

        public:
            Poco::JSON::Object::Ptr toJSON() const;
            static User fromJSON( const std::string& str );

            long             get_id() const;
            const std::string& get_first_name() const;
            const std::string& get_last_name() const;
            const std::string& get_email() const;
            const std::string& get_title() const;
            const std::string& get_type() const;

            long&        id();
            std::string& first_name();
            std::string& last_name();
            std::string& email();
            std::string& title();
            std::string& type();

            static void init();
            static void preload( const std::string& file );
            static User read_by_user_name( const std::string& user_name );
            static std::optional<User> read_from_cache_by_user_name( const std::string& user_name );

            static std::vector<User> search( std::string& first_name, std::string& last_name );
            void save_to_mysql();
            void save_to_cache();
            
            void send_to_queue();
    };
}

#endif