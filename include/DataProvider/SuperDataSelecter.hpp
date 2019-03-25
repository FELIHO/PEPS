#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


class SuperDataSelecter
{
public:
/*
* Tableau contenant les dates de rebalancement
*/
PnlVectInt *ConstationDate_;

	/*
	* Constructeur par défaut
	*/
	SuperDataSelecter();

	/*
	* Constructeur avec prise en compte de date de constatation
	*/
	SuperDataSelecter(PnlVectInt *ConstationDate);

	/*
	* Constructeur par recpie
	*/
	SuperDataSelecter(const SuperDataSelecter &D);

	/*
	* Destructeur
	*/
	~SuperDataSelecter();

	/** Methode renvoyant une matrice ne contenant que les dates de beginningDate
	* @param[in] les cours journaliers d'un produit financier
	* @param[out] le vecteur de volatilités des Log-Rendements associés
	*/
	PnlMat * getPast(const PnlMat *allData, const PnlVectInt *dateIndexes, const int Date);

	PnlMat * getApproximatePath(const PnlMat *FullData, int nbTimeSteps);

	/** Methode calculant le vecteur de volatilités des Log-Rendements à partir d'une matrice path avec un nombre de dates(lignes) 
	1
	* @param[in] allData les cours journaliers de plusieurs actifs financiers
	* @param[in] dateIndexes la matrice associant dates et index
	* @param[out] l'index de la matrice dans allData correspondant à la beginningDate
	*/
	int getIndexDate(const PnlVectInt *dateIndexes, const int Date);

	/** Methode calculant le vecteur de volatilités des Log-Rendements à partir d'une matrice path avec un nombre de dates(lignes) >
	* @param[in] dateIndexes la matrice associant dates et index
	* @param[out] l'index de la matrice dans allData correspondant à la FirstSpotDate
	*/
	int getIndexSpotDate(const PnlVectInt *dateIndexes);

	PnlVectInt* getRebalecementDateIndexes(const PnlVectInt *dateIndexes, const int Date);

	PnlVectInt* getRebalecementDateIndexestoDate(const PnlVectInt *dateIndexes, const int Date);

	PnlVect* getSpotFromData(const PnlMat *allData, const PnlVectInt *dateIndexes, const int Date);

	PnlMat getWindowForEstimation(const PnlMat *allData, const PnlVectInt *dateIndexes, const int DateBegin, const int timeEstimation);

	PnlMat getWindowPreviousEstimation(const PnlMat *allData, const PnlVectInt *dateIndexes, const int DateEnd, const int timeEstimation);

	double computeTvalue(const PnlVectInt *dateIndexes, const int DateZero, const int DateT, const double T);

private:
};