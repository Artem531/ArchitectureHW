#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <iostream>
#include <memory>


namespace database
{
    class Cache
    {
        private:
            std::shared_ptr<std::iostream> _stream;

            Cache();
        public:
            static Cache get();
            void put( const std::string& email, const std::string& val );
            bool get( const std::string& email, std::string& val );

    };
}

#endif