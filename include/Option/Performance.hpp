#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"


namespace Computations {
	/** \class Performance
	 * \brief Option Performance
	 */
	class Performance : public Option
	{

	public:

		// Attributes
	/**
	 * PnlVect *weights_ : vecteur des coefficients de pondération des actifs sous-jacents
	 */
	PnlVect *weights_; // vector containing the proportions of each one of the D underlying asset



	/**
	Constructeur par défaut
	*/
	Performance();

	/**
	 *  \brief Constructeur par copie d'objet Performance
	 *  @param[in] source : l'objet Performance à copier
	 */
	Performance(const Performance& source);

	/** Methode d'affectation d'une Option Performance
	* @param[in] une image de l'Option Performance à affecter.
	* @param[out] la même référence Option Performance avec les mêmes paramètres que l'entrée
	*/
	Performance& operator=(const Performance &P);

	/**
	 *  \brief Destructeur de la classe Basket
	 */
	virtual ~Performance();

	/**
	 *  \brief Constructeur de la classe Performance permettant de préciser les caractéristiques du produits
	 *  @param[in] T : Maturité de l'option
	 *  @param[in] nbTimeSteps : nombre de dates de constatation des cours du sous-jacent
	 *  @param[in] size : nombre d'actifs sous-jacents
	 *  @param[in] weights : vecteur des coefficients de pondération des actifs sous-jacents
	 */
	Performance(double T, int nbTimeSteps, int size, PnlVect *weights);

	/**
	 * \brief Calcule la valeur du payoff sur la trajectoire
	 *
	 * @param[in] path est une matrice de taille (N+1) x d
	 * contenant une trajectoire du modèle telle que créée
	 * par la fonction asset.
	 * @return phi(trajectoire)
	 */
	virtual double payoff(const PnlMat *path);

	virtual Performance* clone();

	};
}
