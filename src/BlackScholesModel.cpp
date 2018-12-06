#pragma once

#include "BlackScholesModel.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>
using namespace std;

/**
Constructeur par défaut
*/
Computations::BlackScholesModel::BlackScholesModel()
{
    size_ = 0; /// nombre d'actifs du modèle
    r_ = 0; /// taux d'intérêt
    rho_ = 0; /// paramètre de corrélation
    sigma_ = pnl_vect_new (); /// vecteur de volatilités
    spot_ = pnl_vect_new (); /// valeurs initiales des sous-jacents
    trend_ = pnl_vect_new();
    initalizeChol();
}



/**
Constructeur complet
*/
Computations::BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot)
{
  size_ = size; /// nombre d'actifs du modèle
  r_ = r; /// taux d'intérêt
  rho_ = rho; /// paramètre de corrélation
  sigma_ = pnl_vect_copy(sigma); /// vecteur de volatilités
  spot_ = pnl_vect_copy(spot);
  trend_ = pnl_vect_create_from_zero(size);
  initalizeChol();
}

/**
Constructeur complet avec trend
*/
Computations::BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot, PnlVect *trend)
{
  size_ = size; /// nombre d'actifs du modèle
  r_ = r; /// taux d'intérêt
  rho_ = rho; /// paramètre de corrélation
  sigma_ = pnl_vect_copy(sigma); /// vecteur de volatilités
  spot_ = pnl_vect_copy(spot);
  trend_ = trend;
  initalizeChol();
}

/**
Constructeur par recopie.
*/
Computations::BlackScholesModel::BlackScholesModel(const BlackScholesModel &BSM){
  size_ = BSM.size_;
  r_ = BSM.r_;
  rho_ = BSM.rho_;
  sigma_ = pnl_vect_copy(BSM.sigma_);
  spot_ = pnl_vect_copy(BSM.spot_);
  trend_ = pnl_vect_copy(BSM.trend_);
  chol_ = pnl_mat_copy(BSM.chol_);
}

/**
Destructeur
*/

Computations::BlackScholesModel::~BlackScholesModel()
{
  pnl_vect_free(&sigma_);
  pnl_vect_free(&spot_);
  pnl_vect_free(&trend_);
  pnl_mat_free(&chol_);
}


/** Methode d'affectation d'un BlackScholesModel
* @param[in] une image de la classe BlackScholesModel à affecter.
* @param[out] la même référence BlackScholesModel avec les mêmes paramètres que l'entrée
*/
Computations::BlackScholesModel& Computations::BlackScholesModel::operator = (const BlackScholesModel &BSM) //le const c'est pour traduire le fait que cet opérateur ne modifie pas le Dvector
{
  size_ = BSM.size_;
  r_ = BSM.r_;
  rho_ = BSM.rho_;
  sigma_ = BSM.sigma_;
  spot_ = BSM.spot_;
  chol_ = BSM.chol_;
  trend_ = BSM.trend_;
  return *this;
}

void Computations::BlackScholesModel::initalizeChol(){
  /** Initialisation de la matrice de corrélation et de la matrice de cholesky*/

  PnlVect *corrVect = pnl_vect_create_from_scalar(size_,1-rho_);
  chol_ =  pnl_mat_create_diag(corrVect);
  pnl_vect_free(&corrVect);

  PnlMat *rhoMatrix = pnl_mat_create_from_scalar(size_,size_,rho_);
  pnl_mat_axpy(1,rhoMatrix, chol_);
  pnl_mat_free(&rhoMatrix);

  PnlVect *eigenValues = pnl_vect_create(size_);
  PnlMat *eigenVectors = pnl_mat_create(size_, size_);
  bool validatedRho = false;

  pnl_mat_eigen(eigenValues, eigenVectors, chol_, 1);
  for(int i = 0; i<size_; i++){
    if(GET(eigenValues,i) <= 0){
      validatedRho = true;
      break;
    }
  }

  pnl_vect_free(&eigenValues);
  pnl_mat_free(&eigenVectors);
  //lever une exeption si rho n'est pas dans l'intervalle [(1/(1-D)) , 1] ou la matrice de corrlation n'est pas diagonalisable
  /*if ((rho_ <= (1/(1-size_))) | (rho_ >= 1) | (validatedRho)){
    throw length_error("Invalide rho");
  }*/

  pnl_mat_chol(chol_);

}

/**
* Génère une trajectoire du modèle et la stocke dans path
*
* @param[out] path contient une trajectoire du modèle.
* C'est une matrice de taille (nbTimeSteps+1) x d
* @param[in] T  maturité
* @param[in] nbTimeSteps nombre de dates de constatation
*/
void Computations::BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng){

  /** Initialisation des vecteurs de calcul */
  PnlVect *G = pnl_vect_new();
  PnlVect *V = pnl_vect_new();
  double timestep = T/nbTimeSteps;
  double sqrtTimestep = sqrt(timestep);
  double sigmaJ = 0.0;

  // Première ligne
  pnl_mat_set_row(path, spot_, 0);

  /** Calcul de la matrice des trajectoires des actifs */
  for (int i = 0; i < path->m -1; i++){
    pnl_vect_rng_normal (G, size_, rng);
    for (int j = 0; j < path->n; j++){
      pnl_mat_get_row(V, chol_, j);
      sigmaJ = pnl_vect_get(sigma_,j);
      pnl_mat_set(path, i+1, j, pnl_mat_get(path,i,j) * exp( (r_-(sigmaJ*sigmaJ)/2) * timestep + sigmaJ*sqrtTimestep * pnl_vect_scalar_prod(V,G)) );
    }
  }

/** Libération de la mémoire */
  pnl_vect_free(&G);
  pnl_vect_free(&V);

}


/**
* Calcule une trajectoire du sous-jacent connaissant le
* passé jusqu' à la date t
*
* @param[out] path  contient une trajectoire du sous-jacent
* donnée jusqu'à l'instant t par la matrice past
* @param[in] t date jusqu'à laquelle on connait la trajectoire.
* t n'est pas forcément une date de discrétisation
* @param[in] nbTimeSteps nombre de pas de constatation
* @param[in] T date jusqu'à laquelle on simule la trajectoire
* @param[in] past trajectoire réalisée jusqu'a la date t
*/
void Computations::BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past){
  if ((path->n != past->n) || (t > T)){
    cout << "valeur de path->n : " << path->n << endl;
    cout << "valeur de past->n : " << past->n << endl;
    cout << "valeur de t : " << t << endl;
    cout << "valeur de T : " << T << endl;
    throw length_error("invalide taille");
  }

  /** Initialisation des vecteurs de calcul */
  PnlVect *G = pnl_vect_new();
  PnlVect *V = pnl_vect_create(size_);
  PnlVect *W = pnl_vect_create(size_);

  double timestep = T/(nbTimeSteps);
  double sqrtTimestep = sqrt(timestep);
  double sigmaJ = 0.0;


  /** Initialisation des paramètres */
  double timeSpend = 0;
  int counter = 0;

  /** Remplissage de la matrice path par la matrice past jusqu'à t*/
  while ((t >= timeSpend)/*&&( timeSpend < T)*/){
    pnl_mat_get_row(V, past, counter);
    pnl_mat_set_row(path, V, counter);
    timeSpend += timestep;
    counter += 1;
  }

  // On récupère St
  pnl_mat_get_row(W, past, past->m -1);

  /** Calcul de la matrice des trajectoires des actifs */
  for (int i = counter ; i < path->m ; i++){
    pnl_vect_rng_normal (G, size_, rng);
    for (int j = 0; j < path->n; j++){
      pnl_mat_get_row(V, chol_, j);
      sigmaJ = pnl_vect_get(sigma_,j);
      if (i == counter){
        // on place St en première ligne
        pnl_mat_set(path, i, j, pnl_vect_get(W,j) * exp( (r_-(sigmaJ*sigmaJ)/2) * (timeSpend  - t) + sigmaJ*sqrt(timeSpend - t) * pnl_vect_scalar_prod(V,G)) );
      }
      else {
        if (i != path->m ){
        pnl_mat_set(path, i, j, pnl_mat_get(path,i-1,j) * exp( (r_-(sigmaJ*sigmaJ)/2) * timestep + sigmaJ*sqrtTimestep * pnl_vect_scalar_prod(V,G)) );
        }
      }
    }
  }

/** Libération de la mémoire */
  pnl_vect_free(&W);
  pnl_vect_free(&G);
  pnl_vect_free(&V);

}

/**
* Shift d'une trajectoire du sous-jacent
*
* @param[in]  path contient en input la trajectoire
* du sous-jacent
* @param[out] shift_path contient la trajectoire path
* dont la composante d a été shiftée par (1+h)
* à partir de la date t.
* @param[in] t date à partir de laquelle on shift
* @param[in] h pas de différences finies
* @param[in] d indice du sous-jacent à shifter
* @param[in] timestep pas de constatation du sous-jacent
*/
void Computations::BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep){
  if ((shift_path->m != path->m)||(path->n != shift_path->n)){
    throw length_error("Invalide taille");
  }
  /** Initialisation des paramètres */
  int indiceRow = 0;
  double timeSpend = 0.0;
  while (timeSpend <= t){
    timeSpend += timestep;
    indiceRow += 1;
  }

  /** Pour récupérer directement les colonnes de la matrice avec un indice différent de d */
  PnlVect *U = pnl_vect_create(path->m);

  for (int j = 0; j < path->n; j++){
      if ((j+1) != d){
        /** On récupère les colonnes différentes de d */
        pnl_mat_get_col(U, path, j);
        pnl_mat_set_col(shift_path,U,j);
      }else{
        /* On remplit la dernière colonne d */
        for (int i = 0; i < path->m; i++){
          double shift = 1.0;
          /** Si l'indice est après t on fait un décale de 1+h*/
          if (i >= indiceRow){
            shift += h;
          }
            pnl_mat_set(shift_path, i, j,pnl_mat_get(path,i,j)*shift);
          }
        }
      }
      pnl_vect_free(&U);
}

void Computations::BlackScholesModel::simul_market(PnlMat *path, double T, int H, PnlRng *rng){
  /** Initialisation des vecteurs de calcul */
  PnlVect *G = pnl_vect_new();
  PnlVect *V = pnl_vect_new();
  double timestep = T/H;
  double sqrtTimestep = sqrt(timestep);
  double sigmaJ = 0.0;
  double inrestJ = 0.0;

  // Première ligne
  pnl_mat_set_row(path, spot_, 0);

  /** Calcul de la matrice des trajectoires des actifs */
  for (int i = 0; i < path->m -1; i++){
    pnl_vect_rng_normal (G, size_, rng);
    for (int j = 0; j < path->n; j++){
      pnl_mat_get_row(V, chol_, j);
      sigmaJ = pnl_vect_get(sigma_,j);
      inrestJ = pnl_vect_get(trend_,j);
      pnl_mat_set(path, i+1, j, pnl_mat_get(path,i,j) * exp( (inrestJ-(sigmaJ*sigmaJ)/2) * timestep + sigmaJ*sqrtTimestep * pnl_vect_scalar_prod(V,G)) );
    }
  }

/** Libération de la mémoire */
  pnl_vect_free(&G);
  pnl_vect_free(&V);
}
