
########################################################################
################# Projet Evaluation Produit Structuré ##################
########################################################################


### Compilation ###
###################

# in /build , type in the following commands :

ENSIMAG Machines :

    cmake3 -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_PREFIX_PATH=<YourPathToPnlBuild> ..

    make

# View the Doxygen Documentation

Always in build, type in the following commands :

    make doc

    firefox doc/html/index.html


