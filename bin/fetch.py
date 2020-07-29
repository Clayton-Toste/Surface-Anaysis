from sys import argv, exit, path
from pymol import cmd, stored

stored.classification_dict = {
    'ASP': 19,
    'GLU': 19,
    'ARG': 3,
    'LYS': 3,
    'HIS': 3,
    'ASN': 2,
    'SER': 2,
    'GLN': 2,
    'PRO': 2,
    'THR': 2, 
    'TYR': 2, 
    'CYS': 2,
    'TRP': 4,
    'ALA': 4,
    'ILE': 4,
    'LEU': 4,
    'MET': 4, 
    'PHE': 4,
    'VAL': 4,
    'GLY': 4,
}

def check_best(x, y, z):
    if y>stored.best_y: 
        stored.best_x = x
        stored.best_y = y 
        stored.best_z = z

stored.check_best = check_best

stored.best_x = float('-inf')
stored.best_y = float('-inf')
stored.best_z = float('-inf')

if (argv[3] == 'c'):
    cmd.fetch(argv[2])
else:
    cmd.load(argv[2])

cmd.show_as('surface', cmd.get_object_list('all')[0])
cmd.alter('all', 'color=stored.classification_dict.get(resn, 0)')
cmd.recolor()
cmd.save(argv[1]+".wrl", cmd.get_object_list('all')[0])

cmd.iterate_state(0, cmd.get_object_list('all')[0], "stored.check_best(x, y+vdw, z,)")

file = open('.protein', 'w')
file.write('{0}\n{1}\n{2}'.format(stored.best_x, stored.best_y, stored.best_z))
file.close()