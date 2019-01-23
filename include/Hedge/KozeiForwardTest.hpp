#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"

namespace Computations {
  /** \class ForwardTest
   * \brief Portefeuille de Couverture
   */
  class KozeiForwardTest
  {
  public:
	  /**
    * Priceur par méthode Monte Carlo utilisé pour la couverture
    */
	  MonteCarlo *monteCarlo_;

    /**
	   * Le spot sur lequel se base la simualtion des données de marchés
	  */
	  PnlVect* Spot_;

    /**
	   * fenetre d'estimation des paramètres
	  */
	  int estimationWindow_;

    


	  /**
     * Matrice contenant les cours passés des sous-jacents
     */
	  PnlMat *past_;
	  /**
	   * Vecteur contenant les delta
	   */
	  PnlVect *delta_;
	  /**
	   * cours à la date courante
	  */
	  PnlVect* S_current;
	  /**
	  * date courante
	  */
    int currentRebalancingIndex_;
    /**
     * Prix calculé par Monte Carlo
     */
    double prix_;
    /**
     * Intervalle de confiance
     */
    double ic_;
    /**
     * Taux sans risque
     */
    double investTauxSansRisque_;
    /**
     * nombre de dates de rebalancement
     */
    double H_;

    /**
		Constructeur par défaut
		*/
		KozeiForwardTest();


    /**
     * \brief Constructeur par copie de la classe ForwardTest
     *
     * @param[in] ForwardTest, portefeuille de couverture à copier
    */
    KozeiForwardTest(const KozeiForwardTest &KFT);

    /** Methode d'affectation ForwardTest
		* @param[in] une image de la classe HedgePortfoli à affecter.
		* @param[out] la même référence HedgePortfoli avec les mêmes paramètres que l'entrée
		*/
		KozeiForwardTest& operator= (const KozeiForwardTest &KFT);

    /**
     * \brief Destructeur de la classe ForwardTest
     */
    ~KozeiForwardTest();

    /**
     * \brief Constructeur de la classe ForwardTest
     *
     * @param[in] Data, la matrice de données à utiliser pour les calculs
     * @param[in] monteCarlo, un object de la classe monteCarlo pour les calculs du price et delta
     *
    */
    KozeiForwardTest(PnlMat *Data, MonteCarlo *monteCarlo);

    /**
     * \brief updateCompo met à jour la composition du portefeuille couverture
     *
     * @param[in] Data, la matrice de données à utiliser pour les calculs
     *
    */
    void updateCompo(PnlMat *Data);

    /**
     * \brief HedgeError boucle sur les données aux dates de rebalancement et renvoie le P&L
     *
     * @param[in] marketData, la matrice de données à utiliser pour les calculs
     *
    */
    double HedgeError(PnlMat *marketData);

    /**
     * \brief met à jour la matrice past avec les données aux dates de constatations et à la date actuelle
     *
     * @param[in] marketData, la matrice de données à utiliser pour les calculs
     * @param[in] indexRebalancement, l'indice des données correspondant à la date courante dans la matrice marketData
     * @param[in] indexToUpdate, l'indice des données correspondant à la ligne à modifier dans la past_
     *
    */
    void updatePast(PnlMat* marketData,int indexRebalancement,int indexToUpdate);
  };
}
