#include "pch.h"
#include "crow_app.h"
#include "gui/controller/main_controller.h"


int main(int argc, char** argv)
{
    using namespace sore;
   
    CrowApp application(argc, argv);

    MainController controller;
    controller.execute();
    
    return application.exec();
}
