#include <string>
#include <filesystem>
#include <iostream>
#include <stdlib.h>

#include <wx/filefn.h> 

using namespace std;

void saFetch(wxString protein_name, wxString protein_identifier, bool isFile=false)
{
    if (!wxDirExists("proteins/"+protein_name))
        if (wxFileExists("proteins/"+protein_name+"/.protein"))
        {
            cout<<"Protein, "+protein_name+", already exists, skiping"<<endl;
            return;
        }
    else
        cout<<wxMkdir("proteins/"+protein_name);
    wxSetWorkingDirectory("proteins/"+protein_name);
    string temp{"../../PyMOL/python.exe ../../fetch.py "};
    temp += '\"' + protein_name + "\" \"" + protein_identifier + "\" " + (isFile ? 'f' : 'c');
    char * command = new char [temp.length()+1];
    strcpy(command, temp.c_str());
    system(command);
    delete command;
    wxSetWorkingDirectory("../../");
}