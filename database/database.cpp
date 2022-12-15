#include "database.h"
#include "../config/config.h"

namespace database {
    Database::Database() {
        _connection_string_read+="host=";
        _connection_string_read+=Config::get().get_read_request_ip();
        _connection_string_read+=";port=";
        _connection_string_read+=Config::get().get_port();
        _connection_string_read+=";user=";
        _connection_string_read+=Config::get().get_login();
        _connection_string_read+=";db=";
        _connection_string_read+=Config::get().get_database();
        _connection_string_read+=";password=";
        _connection_string_read+=Config::get().get_password();

        _connection_string_write+="host=";
        _connection_string_write+=Config::get().get_write_request_ip();
        _connection_string_write+=";port=";
        _connection_string_write+=Config::get().get_port();
        _connection_string_write+=";user=";
        _connection_string_write+=Config::get().get_login();
        _connection_string_write+=";db=";
        _connection_string_write+=Config::get().get_database();
        _connection_string_write+=";password=";
        _connection_string_write+=Config::get().get_password();

        Poco::Data::MySQL::Connector::registerConnector();
        _pool = std::make_unique<Poco::Data::SessionPool>( Poco::Data::MySQL::Connector::KEY, _connection_string );
    }

    Database& Database::get() {
        static Database _instance;
        return _instance;
    }
    
    Poco::Data::Session Database::create_session() {
        return Poco::Data::Session( _pool->get() );
    }
    
    int Database::get_max_shard() {
        return 3;
    }
    
    std::vector<std::string> Database::get_all_hints() {
        std::vector<std::string> result;
        for( int i = 0; i < get_max_shard(); ++i ) {
            std::string shard_name = "-- sharding:";
            shard_name += std::to_string( i );
            std::cout << shard_name;
            result.push_back( shard_name );
        }
        return result;
    }

    std::string Database::sharding_hint( const std::string& login, const std::string& name ) {
        std::string key;
        key = login + ';' + name;

        size_t shard_number = std::hash<std::string>{}( key ) % get_max_shard();

        std::string result = "-- sharding:";
        result += std::to_string( shard_number );

        return result;
    }

    Poco::Data::Session Database::create_session_read()
    {
        std::cout << _connection_string_read << " _connection_string_read \n";
        return Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, _connection_string_read));
    }

    Poco::Data::Session Database::create_session_write()
    {
        std::cout << _connection_string_write << " _connection_string_write \n"; 
        return Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, _connection_string_write));
    }
}