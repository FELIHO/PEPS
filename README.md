######################################################
################# Pricer MonteCarlo ##################
######################################################


### Compilation ###
###################

# in /build , type in the following commands :

ENSIMAG Machines :
    cmake3 -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_PREFIX_PATH=<YourPathToPnlBuild> ..

    make

    make doc

# View the Doxygen Documentation

    firefox doc/html/index.html


# run ./price as demanded
  # without -c option with only option data file

    ./price ../data-soutenance.call.dat

  # with -c option with both market and option data files

    ./price -c ../data-soutenance/market_call.dat ../data-soutenance.call.dat

# Tests
make test permet d'executer tous les tests à la suite.

Pour tester:

- se placer dans le dossier /build/test
- ouvrir un terminal 
- taper la commande: ./nom_du_test


