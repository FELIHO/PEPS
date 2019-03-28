#pragma once

#ifndef KOZEI_HPP
#define KOZEI_HPP


#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"
#include "Tools.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>

/** \class Kozei
 * \brief Option Kozei
 */

class Kozei : public Option {
	
public:
	// Attributes
	/**
 	* inv_init_ : l'investissement initial
 	*/
	double inv_init_; //investissement initial
	/**
 	* *niveauxInitiaux_ : vecteur contenant les niveaux initiaux des actifs,
	Pour chaque Action, le Niveau Initial s'obtient en faisant la moyenne arithmétique des cours de clôture de chaque
	Actions publiés les 11, 14 et 15 avril 2014
 	*/
	PnlVect* niveauxInitiaux_;
	/**
	 * constationDates_ : vecteur contenant les dates de constattion du produit Kozei
	*/
	PnlVect* constationDates_;
	/**
	 * r_CHF,r_USD,r_JPY,r_GBP,r_BRL : les taux d'intérêt étranger pour chaque pays
	*/
	double r_CHF = 0.0005;
	double r_USD = 0.025;
	double r_JPY = 0.001;
	double r_GBP = 0.0075;
	double r_BRL = 0.065;
	/**
	Constructeur par défaut
	*/
	Kozei();
	/**
	*  \brief Constructeur par copie d'objet Kozei
	*  @param[in] param source : l'objet Kozei à copier
	*/
	Kozei(const Kozei &K);
	/** Methode d'affectation d'une Option Kozei
	* @param[in] une image de l'Option Kozei à affecter.
	* @param[out] la même référence Option Kozei avec les mêmes paramètres que l'entrée
	*/
	Kozei& operator=(const Kozei &K);
	/**
	*  \brief Destructeur de la classe Kozei
	*/
	~Kozei();

	Kozei* clone();
	/**
	*  \brief Constructeur de la classe Kozei permettant de préciser les caractéristiques du produits
	*  @param[in] inv_init : l'investissement initial
	*/
	Kozei(double inv_init);
	/**
	*  \brief Constructeur de la classe Kozei permettant de préciser les caractéristiques du produits
	*  @param[in] inv_init : l'investissement initial
	*  @param[in] niveauxInitiaux : le vecteur des niveaux Initiaux
	*/
	Kozei(double inv_init, PnlVect* niveauxInitiaux);
	/**
	*  \brief Constructeur de la classe Kozei permettant de préciser les caractéristiques du produits
	*  @param[in] inv_init : l'investissement initial
	*  @param[in] marketData : Une matrice contenant les prix des actifs 
	*  @param[in] firstDateIndex :  Représente l'index de la ligne correspondante à la date t = 0 dans la matrice marketData 
	*/

	Kozei(double inv_init, PnlMat* marketData, int firstDateIndex=0);
	/**
	*  \brief Methode permetant d'insérer le vecteur des nivaux initiaux à la matrice marketData
	*  @param[in] marketData : Une matrice contenant les prix des actifs 
	*  @param[in] firstDateIndex : Représente l'index de la ligne correspondante à la date t = 0 dans la matrice marketData 
	*/
	void SetNivauxInitiaux(PnlMat* marketData, int firstDateIndex=0);
	/**
 	*  \brief Methode d'initialiser le vecteur contenant les valeurs du zero-coupon aux differents dates
	 * @param[in] number_of_dates : nombre de dates 
	 * @param[in] rate : Le taux d'intérêt
	*/
	PnlVect* ZeroCoupon(int number_of_dates,double rate);
	/**
 	*  \brief Methode d'initialisation de la matrice path contenant les prix des actifs (les 30 premières colonnes ) 
	 * concaténée avec la matrice contenant les valeur des taux de change
	 * @param[in] assets_path : matrice des actifs 
	 * @param[in] ExR_path : matrice des taux de change
	 * @return la matrice path ,résultat de la concaténation des deux matrices

	*/
	PnlMat* path_matrix(const PnlMat* assets_path,const PnlMat* ExR_path);
	/**
 	* \brief Calcule la valeur du payoff sur la trajectoire,sans prendre en considération les taux de change
 	*
 	* @param[in] path est une matrice
 	* contenant une trajectoire du modèle
 	* @return payoff la valeur du payoff
 	*/
	double payoff(const PnlMat *path);
	double payoff_without_ExR(const PnlMat *path);
	/**
 	* \brief Calcule la valeur du payoff sur la trajectoire,en prenant en considération les taux de change
 	*
 	* @param[in] path est une matrice
 	* contenant une trajectoire du modèle
 	* @return payoff la valeur du payoff
 	*/
	double payoff_with_ExR(const PnlMat *path);
};

#endif
