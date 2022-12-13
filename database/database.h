#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>

namespace database {
    class Database {
        private:
            std::string _connection_string;
            std::unique_ptr<Poco::Data::SessionPool> _pool;

            std::string _connection_string_read;
            std::string _connection_string_write;
            
            Database();
        public:
            static Database& get();
            Poco::Data::Session create_session();

            Poco::Data::Session create_session_read();
            Poco::Data::Session create_session_write();

            static int get_max_shard();
            static std::string sharding_hint( const std::string& login, const std::string& name );
            static std::vector<std::string> get_all_hints();
    };
}
#endif