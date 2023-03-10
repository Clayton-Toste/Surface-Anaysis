from sys import argv, exit, path
from pymol import cmd, stored

stored.classification_dict = {
    'ASP': 19, #Charged -
    'GLU': 19,
    'ARG': 3, #Charged +
    'LYS': 3,
    'HIS': 3,
    'ASN': 2, #Hydrophilic
    'SER': 2,
    'GLN': 2,
    'PRO': 2,
    'THR': 2, 
    'TYR': 2, 
    'CYS': 2,
    'TRP': 4, #Hydrophobic
    'ALA': 4,
    'ILE': 4,
    'LEU': 4,
    'MET': 4, 
    'PHE': 4,
    'VAL': 4,
    'GLY': 4,
    # For Zhiyuan
    'SMA': 3,
    'OMA': 2,
    'MMA': 4,
    'EMA': 4

}
if len(argv) > 1:
    stored.best_y = float('-inf')
    if (argv[3] == 'c'):
        cmd.fetch(argv[2])
    else:
        cmd.load(argv[2])

cmd.remove("sol")

cmd.show_as('surface', cmd.get_object_list('all')[0])
cmd.alter('all', 'color=stored.classification_dict.get(resn, 0)')
cmd.remove('color white')
cmd.recolor()
cmd.save(argv[1]+".wrl", cmd.get_object_list('all')[0])

cmd.png("icon")

file = open('.protein', 'w')
file.write((argv[2] if argv[3] == 'c' else "Unknown") + "\n")
file.write('{0}\n{1}\n{2}\n{3}'.format(*cmd.centerofmass()))
file.close()