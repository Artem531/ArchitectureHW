#ifndef USERHANDLER_H
#define USERHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/user.h"
#include "../web_server/utils.h"

class UserHandler : public HTTPRequestHandler
{
private:
    bool check_name( const std::string &name, std::string &reason )
    {
        if( name.length() < 3 ) {
            reason = "Name must be at leas 3 signs";
            return false;
        }

        if( name.find(' ') != std::string::npos ) {
            reason = "Name can't contain spaces";
            return false;
        }

        if( name.find('\t') != std::string::npos ) {
            reason = "Name can't contain spaces";
            return false;
        }

        return true;
    };

    bool check_email( const std::string &email, std::string &reason )
    {
        if( email.find('@') == std::string::npos ) {
            reason = "Email must contain @";
            return false;
        }

        if( email.find(' ') != std::string::npos ) {
            reason = "Email can't contain spaces";
            return false;
        }

        if( email.find('\t') != std::string::npos ) {
            reason = "Email can't contain spaces";
            return false;
        }

        return true;
    };

public:
    UserHandler( const std::string &format ) : _format( format )
    {
    }

    void handleRequest( HTTPServerRequest &request,
                       HTTPServerResponse &response )
    {
        HTMLForm form(request, request.stream());
        std::cout << "user handler \n";

        if( startsWith(request.getURI(), "/user/search" ) ) {
            try {
                response.setStatus( Poco::Net::HTTPResponse::HTTP_OK );
                response.setChunkedTransferEncoding( true );
                response.setContentType( "application/json" );
                std::ostream &ostr = response.send();

                std::string fn = form.get( "first_name" );
                std::string ln = form.get( "last_name" );
                auto results = database::User::search( fn, ln );
                Poco::JSON::Array arr;
                for ( auto s : results )
                    arr.add( s.toJSON() );
      
                Poco::JSON::Stringifier::stringify( arr, ostr );
            } catch (...) {
                response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
                std::ostream &ostr = response.send();
                ostr << "{ \"result\": false , \"reason\": \"not found\" }";
                response.send();
                return;
            }
            return;
        } else if( startsWith( request.getURI(), "/user" ) ) {
            std::cout << "/user \n";
            if( request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST ) {

                std::cout << "post\n";
                if ( form.has( "first_name" ) && form.has( "last_name" ) && form.has( "email" ) && form.has( "title" ) && form.has( "user_type" ) )
                    std::cout << "Check form ok \n";
            
                database::User user;
                user.first_name() = form.get( "first_name" );
                user.last_name() = form.get( "last_name" );
                user.email() = form.get( "email" );
                user.title() = form.get( "title" );
                user.type() = form.get( "user_type" );

                bool use_cache = true;
                if( form.has("no_cache" ) ) {
                    if ( form.get( "no_cache" ) == "true" ) {
                        use_cache = false;
                    }
                }

                if ( use_cache ) {
                    user.save_to_cache();
                    std::cout << "Saved user " + user.first_name() + " in cache and database." << std::endl;
                } else {
                    std::cout << "Saved user " + user.first_name() + " in database." << std::endl;
                }

                std::cout << form.get( "first_name" ) <<
                    form.get( "last_name" ) <<
                    form.get( "email" )  << "\n";
                
                bool check_result = true;
                std::string message;
                std::string reason;

                if (!check_name( user.get_first_name(), reason) ) {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if( !check_name( user.get_last_name(), reason ) ) {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if( !check_email( user.get_email(), reason ) ) {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if( check_result ) {
                    try {
                        //user.save_to_mysql();
                        
                        static int i=0;
                        user.send_to_queue();
                        std::cout << "send to queue: " << std::to_string(++i)  << std::endl;
                        
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_CREATED );
                        response.setChunkedTransferEncoding( true );
                        response.setContentType( "application/json" );
                        std::ostream &ostr = response.send();
                        ostr << user.get_id();
                        return;
                    } catch (...) {
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
                        std::ostream &ostr = response.send();
                        ostr << "database error";
                        response.send();
                        return;
                    }
                } else {
                    response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
                    std::ostream &ostr = response.send();
                    ostr << message;
                    response.send();
                    return;
                }  
            } else {
                if( form.has( "user_name" ) ) {
                    try {
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_OK );
                        response.setChunkedTransferEncoding( true );
                        response.setContentType( "application/json" );
                        std::ostream &ostr = response.send();
                        std::cout << "read by id \n";
                        
                        bool use_cache = true;
                        if (form.has("no_cache")) {
                            if ( form.get("no_cache") == "true" ) {
                                use_cache = false;
                            }
                        }
                        std::string user_name = form.get( "user_name" ).c_str();
                        if( use_cache ) {
                            try {
                                std::optional<database::User> result = database::User::read_from_cache_by_user_name( user_name );
                                if( result ) {
                                    Poco::JSON::Stringifier::stringify( result->toJSON(), ostr );
                                    std::cout << "Cache used for user " << user_name << std::endl;
                                    return;
                                }
                            } catch (...) {
                                std::cout << "Cache faild to find data for user " << user_name << std::endl;
                            }
                        }

                        database::User result = database::User::read_by_user_name( user_name );
                        if( use_cache ) {
                            result.save_to_cache();
                            std::cout << "Update cache for user " << user_name << std::endl;
                        }
                        
                        Poco::JSON::Stringifier::stringify( result.toJSON(), ostr );
                        return;
                    } catch (...) {
                        response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
                        std::ostream &ostr = response.send();
                        ostr << "{ \"result\": false , \"reason\": \"not found\" }";
                        response.send();
                        return;
                    }
                }
            }
        }
        
        response.setStatus( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
        std::ostream &ostr = response.send();
        ostr << "request error";
        response.send();
    }

private:
    std::string _format;
};
#endif // !USERHANDLER_H