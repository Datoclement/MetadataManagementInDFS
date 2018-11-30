#include "mmetadataserver.hpp"
#include "merror.hpp"

#include <vector>
#include <string>

using namespace std;

mMetadataServer::mMetadataServer()
{}

void mMetadataServer::hi(const vector<string>& argv, string& placeholder)
{
    placeholder = "hi";
}

void mMetadataServer::run_command_line(const vector<string>& argv, string& placeholder)
{
    if (argv.size() == 0)
    {
        error("argv size should never be zero. debug it.\n");
    }
    const string& command = argv[0];
    if (command == "hi")
    {
        this->hi(argv, placeholder);
    }
    else
    {
        placeholder = "Error: command " + command + " is not supported.\n";
    }

}
