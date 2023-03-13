#include <string>
#include <iostream>
#include <stdlib.h>

#include <wx/filefn.h> 

void saFetch(wxString protein_name, wxString protein_identifier, bool isFile=false)
{
    if (!wxDirExists("proteins/"+protein_name))
        if (wxFileExists("proteins/"+protein_name+"/.protein"))
        {
            cout<<"Protein, "+protein_name+", already exists, skiping"<<endl;
            return;
        }
    wxSetWorkingDirectory("proteins/"+protein_name);
    std::string temp{"../../PyMOL/python.exe ../../fetch.py "};
    temp += '\"' + protein_name + "\" \"" + protein_identifier + "\" " + (isFile ? 'f' : 'c');
    char * command = new char [temp.length()+1];
    strcpy(command, temp.c_str());
    system(command);
    delete command;
    wxSetWorkingDirectory("../../");
}