
######################################################
################# Pricer MonteCarlo ##################
######################################################


### Compilation ###
###################

# in /build , type in the following commands :

ENSIMAG Machines :
    cmake3 -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_PREFIX_PATH=<YourPathToPnlBuild> ..
(c compiler is not needed in this project)

    make

<<<<<<< HEAD
    make doc

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_PREFIX_PATH=<YourPathToPnlBuild> ..

=======
>>>>>>> 9a8ffe9109fcec3e8c498e1e8f6aa9996aa9c098
# View the Doxygen Documentation

    make doc
    firefox doc/html/index.html


# run ./pricer as demanded
  # without -c option with only option data file

    ./pricer ../data-soutenance/call.dat

  # with -c option with both market and option data files

    ./pricer -c ../market-soutenance/market_call.dat ../data-soutenance/call.dat

# Tests
make test permet d'executer tous les tests à la suite.

Pour tester:

- se placer dans le dossier /build/test
- ouvrir un terminal 
- taper la commande: ./nom_du_test

