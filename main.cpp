#include "../web_server/http_web_server.h"


int main(int argc, char*argv[]) 
{
    std::cout << "Start Web server";
    HTTPWebServer app;
    return app.run(argc, argv);
}