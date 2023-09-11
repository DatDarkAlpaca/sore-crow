#include "pch.h"
#include "crow_app.h"


int main(int argc, char** argv)
{
    using namespace sore;
   
    CrowApp application(argc, argv);
    application.execute();
    
    return application.exec();
}
