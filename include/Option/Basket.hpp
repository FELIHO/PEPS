#pragma once

#ifndef BASKET_HPP
#define BASKET_HPP


#include "Option.hpp"

/** \class Basket
 * \brief Option Vanille
 */
class Basket: public Option
{

public:

  // Attributes
/**
 * double strike_ : prix d'exercice de l'option
 */
double strike_; // strike K of the basket option
/**
 * PnlVect *weights_ : vecteur des coefficients de pondération des actifs sous-jacents
 */
PnlVect *weights_; // vector containing the proportions of each one of the D underlying asset

/**
Constructeur par défaut
*/
Basket();

/**
*  \brief Constructeur par copie d'objet Basket
*  @param[in] param source : l'objet Basket à copier
*/
Basket(const Basket& source);

/** Methode d'affectation d'une Option Basket
* @param[in] une image de l'Option Basket à affecter.
* @param[out] la même référence Option Basket avec les mêmes paramètres que l'entrée
*/
Basket& operator=(const Basket &B);

/**
*  \brief Destructeur de la classe Basket
*/
virtual ~Basket();

/**
*  \brief Constructeur de la classe Basket permettant de préciser les caractéristiques du produits
*  @param[in] T : Maturité de l'option
*  @param[in] nbTimeSteps : nombre de dates de constatation des cours du sous-jacent
*  @param[in] size : nombre d'actifs sous-jacents
*  @param[in] strike : prix d'exercice de l'option
*  @param[in] weights : vecteur des coefficients de pondération des actifs sous-jacents
*/
Basket(double T, int nbTimeSteps, int size, double strike, PnlVect *weights);

/**
 * \brief Calcule la valeur du payoff sur la trajectoire
 *
 * @param[in] path est une matrice de taille (N+1) x d
 * contenant une trajectoire du modèle telle que créée
 * par la fonction asset.
 * @return phi(trajectoire)
 */
  virtual double payoff(const PnlMat *path);

virtual Basket* clone();

};

#endif
