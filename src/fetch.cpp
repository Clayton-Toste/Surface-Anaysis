#include "fetch.hpp"

void fetch(string name, string protein, bool isFile)
{
    if (!wxDirExists("proteins/"+name))
        if (wxFileExists("proteins/"+name+"/.protein"))
        {
            cout<<"Protein, "+name+", already exists, skiping"<<endl;
            return;
        }
    else
        cout<<wxMkdir("proteins/"+name);
    wxSetWorkingDirectory("proteins/"+name);
    if (isFile)
        wxCopyFile(protein, protein = name + '.' + filesystem::path(protein).extension().generic_string());
#ifdef unix
    string temp{"wine ../../PyMOL/python.exe ../../fetch.py "};
#else
    string temp{"wine ../../PyMOL/python.exe ../../fetch.py "};
#endif
    temp += name + ' ' + protein + ' ' + (isFile ? 'f' : 'c');
    char * command = new char [temp.length()+1];
    strcpy(command, temp.c_str());
    cout<<command<<endl;
    system(command);
    wxSetWorkingDirectory("../../");
}