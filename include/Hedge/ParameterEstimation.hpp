#pragma once

#ifndef PARAMETERESTIMATION_HPP
#define PARAMETERESTIMATION_HPP


#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

class ParameterEstimation
{
public:
	/*
	* Default Constructor
	*/
	ParameterEstimation() {}

	/** Methode calculant la matrice des Log-Rendements à partir d'une matrice path avec un nombre de dates(lignes) > 1
	* @param[in] les cours journaliers d'un produit financier
	* @param[out] la matrice des log-rendements associés
	*/
	static PnlMat* getLogRendementMatrix(const PnlMat *path);

	/** Methode calculant la matrice de Covariance des Log-Rendements à partir d'une matrice path avec un nombre de dates(lignes) > 1
	* @param[in] les cours journaliers d'un produit financier
	* @param[out] la matrice do covariance des log-rendements associés
	*/
	static PnlMat* getCovarianceMatrix(const PnlMat *path);

	/** Methode calculant la matrice de Corrélation des Log-Rendements à partir d'une matrice path avec un nombre de dates(lignes) > 1
	* @param[in] les cours journaliers d'un produit financier
	* @param[out] la matrice do corrélation des log-rendements associés
	*/
	static PnlMat* getCorrelationMatrix(const PnlMat *path);

	/** Methode calculant le vecteur de volatilités des Log-Rendements à partir d'une matrice path avec un nombre de dates(lignes) > 1
	* @param[in] les cours journaliers d'un produit financier
	* @param[out] le vecteur de volatilités des Log-Rendements associés
	*/
	static PnlVect* getVolatilitiesVector(const PnlMat *path);

	/** Methode calculant le sigma entre deux paramètres
	* @param[in]  Sigma_X, double volatilité de l'actif X
	* @param[in]  Sigma_Y, double volatilité de l'actif Y
	* @param[in]  rhoXY, double corrélation entre l'actif X et l'actif Y
	* @param[out] le sigma de du couple liant l'actif X et l'actif Y
	*/
	static double getSigmaCorreled(const double Sigma_X, const double Sigma_Y, const double rhoXY);

	/** Methode calculant la corrélation entre deux vecteurs, ces derniers doivent avoir la même taille
	* @param[in]  ShareX, PnlVect* trajectoire de l'actif X
	* @param[in]  ShareY, PnlVect* trajectoire de l'actif Y
	* @param[out] la corrélation rhoXY entre deux trajectoires
	*/
	static double getCorrelation(const PnlVect* shareX, const PnlVect* shareY);

	static PnlMat* getDomesticAssetPrices(const PnlMat *pricesforeigncurr,const PnlMat* exchangerates);

	static PnlVect* getTrend(const PnlMat *path);

private:
};





double makeLogonAllElements(double vectorElement);

#endif
