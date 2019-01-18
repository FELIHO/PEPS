#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"


namespace Computations {

	/** \class Asian
	 * \brief Option Asiatique
	 */
	class Asian : public Option
	{

	public:

		// Attributes
	/**
	 * double strike_ : prix d'exercice de l'option
	 */
	double strike_; // strike K of the asian option
	/**
	 * PnlVect *weights_ : vecteur des coefficients de pondération des actifs sous-jacents
	 */
	PnlVect *weights_; // vector containing the proportions of each one of the D underlying asset


	/**
	Constructeur par défaut
	*/
	Asian();

	/**
	 *  \brief Constructeur par copie d'objet Asian
	 *  @param[in] source : l'objet Asian à copier
	 */
	Asian(const Asian& source);

	/** Methode d'affectation d'une Option Basket
	* @param[in] une image de l'Option Basket à affecter.
	* @param[out] la même référence Option Basket avec les mêmes paramètres que l'entrée
	*/
	Asian& operator=(const Asian &A);

	/**
	 *  \brief Destructeur de la classe Asian
	 */
	virtual ~Asian();

	/**
	 *  \brief Constructeur de la classe Asian permettant de préciser les caractéristiques du produits
	 *  @param[in] T : Maturité de l'option
	 *  @param[in] nbTimeSteps : nombre de dates de constatation des cours du sous-jacent
	 *  @param[in] size : nombre d'actifs sous-jacents
	 *  @param[in] strike : prix d'exercice de l'option
	 *  @param[in] weights : vecteur des coefficients de pondération des actifs sous-jacents
	 */
	Asian(double T, int nbTimeSteps, int size, double strike, PnlVect *weights);

	/**
	 * \brief Calcule la valeur du payoff sur la trajectoire
	 *
	 * @param[in] path est une matrice de taille (N+1) x d
	 * contenant une trajectoire du modèle telle que créée
	 * par la fonction asset.
	 * @return phi(trajectoire)
	 */
	 virtual double payoff(const PnlMat *path);

	virtual Asian* clone();

	};

}
