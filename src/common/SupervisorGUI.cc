#include "DTCSupervisor/SupervisorGUI.h"

using namespace Ph2TkDAQ;

SupervisorGUI::SupervisorGUI (xgi::framework::UIManager* pManager, std::string& pURN) :
    fManager (pManager),
    fURN (pURN),
    fHWDescriptionFile (nullptr),
    fXLSStylesheet (nullptr),
    fHWFormString (""),
    fCurrentPageView (Tab::MAIN)
{
    //bind xgi and xoap commands to methods
    //methods for tab navigation
    //xgi::bind (this, &SupervisorGUI::Default, "Default");
    xgi::bind (this, &SupervisorGUI::MainPage, "MainPage");
    xgi::bind (this, &SupervisorGUI::ConfigPage, "ConfigPage");
    xgi::bind (this, &SupervisorGUI::CalibrationPage, "CalibrationPage");
    xgi::bind (this, &SupervisorGUI::DAQPage, "DAQPage");

    //helper methods for buttons etc
    //xgi::bind (this, &SupervisorGUI::reloadHWFile, "reloadHWFile");
    //xgi::bind (this, &SupervisorGUI::handleHWFormData, "handleHWFormData");
    xgi::bind (this, &SupervisorGUI::lastPage, "lastPage");
}

void SupervisorGUI::MainPage (xgi::Input* in, xgi::Output* out) throw (xgi::exception::Exception)
{
    std::string url = fURN + "reloadHWFile";

    // stream for logger
    std::ostringstream cLogStream;

    //define view and create header
    fCurrentPageView = Tab::MAIN;
    this->createHtmlHeader (in, out, fCurrentPageView);

    // generate the page content
    *out << cgicc::h3 ("DTC Supervisor Main Page") << std::endl;
    this->displayLoadForm (in, out);

    //LOG4CPLUS_INFO (this->getApplicationLogger(), cLogStream.str() );
    this->createHtmlFooter (in, out);
}

void SupervisorGUI::ConfigPage (xgi::Input* in, xgi::Output* out) throw (xgi::exception::Exception)
{
    //define view and create header
    fCurrentPageView = Tab::CONFIG;
    this->createHtmlHeader (in, out, fCurrentPageView);

    //string defining action
    std::string url = fURN + "handleHWFormData";

    this->displayLoadForm (in, out);
    // Display the HwDescription HTML form
    *out << cgicc::form().set ("method", "POST").set ("action", url).set ("enctype", "multipart/form-data") << std::endl;
    *out << cgicc::input().set ("type", "submit").set ("title", "submit the entered values").set ("value", "Submit") << std::endl;
    *out << cgicc::input().set ("type", "reset").set ("title", "reset the form").set ("value", "Reset") << std::endl;
    *out << fHWFormString << std::endl;
    *out << cgicc::form() << std::endl;


    this->createHtmlFooter (in, out);
}

void SupervisorGUI::createHtmlHeader (xgi::Input* in, xgi::Output* out, Tab pTab)
{
    // Create the Title, Tab bar
    std::ostringstream cLogStream;

    fManager->getHTMLHeader (in, out);
    //Style this thing
    *out << cgicc::style() << std::endl;
    *out << parseStylesheetCSS (expandEnvironmentVariables ("${DTCSUPERVISOR_ROOT}/html/Stylesheet.css"), cLogStream) << std::endl;
    *out << cgicc::style() << std::endl;

    *out << cgicc::title ("DTC Supervisor")  << std::endl;

    std::ostringstream cTabBarString;
    std::string url = fURN;

    // switch to show the current tab
    switch (pTab)
    {
        case Tab::MAIN:
            cTabBarString << "<a href='" << url << "MainPage' class=\"button active\">MainPage</a>  <a href='" << url << "ConfigPage' class=\"button\">ConfigPage</a>  <a href='" << url << "CalibrationPage' class=\"button\">CalibrationPage</a>  <a href='" << url << "DAQPage' class=\"button\">DAQPage</a>" << std::endl;
            break;

        case Tab::CONFIG:
            cTabBarString << "<a href='" << url << "MainPage' class=\"button\">MainPage</a>  <a href='" << url << "ConfigPage' class=\"button active\">ConfigPage</a>  <a href='" << url << "CalibrationPage' class=\"button\">CalibrationPage</a>  <a href='" << url << "DAQPage' class=\"button\">DAQPage</a>" << std::endl;
            break;

        case Tab::CALIBRATION:
            cTabBarString << "<a href='" << url << "MainPage' class=\"button\">MainPage</a>  <a href='" << url << "ConfigPage' class=\"button\">ConfigPage</a>  <a href='" << url << "CalibrationPage' class=\"button active\">CalibrationPage</a>  <a href='" << url << "DAQPage' class=\"button\">DAQPage</a>" << std::endl;
            break;

        case Tab::DAQ:
            cTabBarString << "<a href='" << url << "MainPage' class=\"button\">MainPage</a>  <a href='" << url << "ConfigPage' class=\"button\">ConfigPage</a>  <a href='" << url << "CalibrationPage' class=\"button\">CalibrationPage</a>  <a href='" << url << "DAQPage' class=\"button active\">DAQPage</a>" << std::endl;
            break;
    }

    *out << "<div class=\"tab\">" << std::endl;
    *out << cTabBarString.str() << std::endl;
    *out << "</div>" << std::endl;
    *out << "<div class=\"main\">" << std::endl;
    this->showStateMachineStatus (out);
    *out << "<div class=\"content\">" << std::endl;

    //LOG4CPLUS_INFO (this->getApplicationLogger(), cLogStream.str() );
}

void SupervisorGUI::createHtmlFooter (xgi::Input* in, xgi::Output* out)
{
    //close the main and content div
    *out << "</div class=\"content\"></div class=\"main\">" << std::endl;
    fManager->getHTMLFooter (in, out);

}

void SupervisorGUI::showStateMachineStatus (xgi::Output* out) throw (xgi::exception::Exception)
{
    // create the FSM Status bar showing the current state
    try
    {
        //std::string action = toolbox::toString ("/%s/fsmTransition", getApplicationDescriptor()->getURN().c_str() );

        // display FSM
        //std::set<std::string> possibleInputs = fsm_.getInputs (fsm_.getCurrentState() );
        //std::set<std::string> allInputs = fsm_.getInputs();

        *out << "<div class=\"sidenav\">" << std::endl;
        *out << "<p class=\"state\">Current State: " << "Somestate" << "</p>" << std::endl;
        *out << cgicc::br() << std::endl;
        *out << "<a href=\"#\" class=\"button\"> Initialize </a>" << cgicc::br() << std::endl;
        *out << "<a href=\"#\" class=\"button\"> Configure </a>" << cgicc::br() << std::endl;
        *out << "<a href=\"#\" class=\"button\"> Start </a>" << cgicc::br() << std::endl;
        *out << "<a href=\"#\" class=\"button\"> Stop </a>" << cgicc::br() << std::endl;
        *out << "<a href=\"#\" class=\"button\"> Pause </a>" << cgicc::br() << std::endl;
        *out << "<a href=\"#\" class=\"button\"> Resume </a>" << cgicc::br() << std::endl;
        *out << "<a href=\"#\" class=\"button\"> Halt </a>" << cgicc::br() << std::endl;
        *out << "<a href=\"#\" class=\"button\"> Destroy </a>" << cgicc::br() << std::endl;
        *out << "</div>" << std::endl;
    }
    catch (xgi::exception::Exception& e)
    {
        XCEPT_RETHROW (xgi::exception::Exception, "Exception caught in WebShowRun", e);
    }
}


void SupervisorGUI::displayLoadForm (xgi::Input* in, xgi::Output* out)
{
    std::string url = fURN + "reloadHWFile";
    *out << cgicc::div().set ("padding", "10px") << std::endl;
    *out << cgicc::form().set ("method", "POST").set ("action", url).set ("enctype", "multipart/form-data").set ("autocomplete", "on") << std::endl;
    *out << "<label for=\"HwDescriptionFile\">Hw Descritpion FilePath: </label>" << std::endl;
    //if(state==halted)
    *out << cgicc::input().set ("type", "text").set ("name", "HwDescriptionFile").set ("id", "HwDescriptionFile").set ("size", "70").set ("value", fHWDescriptionFile->toString() ) << std::endl;
    *out << cgicc::input().set ("type", "submit").set ("title", "change the Hw Description File").set ("value", "Load") << std::endl;
    //else
    //*out << cgicc::input().set ("type", "text").set ("name", "HwDescriptionFile").set ("id", "HwDescriptionFile").set ("size", "70").set ("value", fHWDescriptionFile.toString() ).set ("disabled", "disabled") << std::endl;
    //*out << cgicc::input().set ("type", "submit").set ("title", "change the Hw Description File").set ("value", "Load").set ("disabled", "disabled") << std::endl;
    *out << cgicc::form() << std::endl;
    *out << cgicc::div() << std::endl;
}
