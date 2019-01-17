#pragma once

#ifndef HEDGEPORTFOLIO_HPP
#define HEDGEPORTFOLIO_HPP


/*
 * \file HedgePortfolio.hpp
 * \brief Portefeuille de Couverture
 * \author arahouy jmarian benhamoa letaillc
 * \date 18/09/2018
 *
 */

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
using namespace Computations;
/** \class HedgePortfolio
 * \brief Portefeuille de Couverture
 */
class HedgePortfolio
{
public:
  /**
   * Priceur par méthode Monte Carlo utilisé pour la couverture
   */
  MonteCarlo* monteCarlo_;
  /**
   * Matrice contenant les cours passés des sous-jacents
   */
  PnlMat* past_;
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
  //double currentDate_;
  int currentIndexRebalancement_;
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
   * nombre de dates de constatations
   */
  double H_;


  /**
   * \brief Destructeur de la classe HedgePortfolio
   */
  ~HedgePortfolio();
  /**
   * \brief Constructeur par copie de la classe HedgePortfolio
   *
   * @param[in] HedgePortfolio, portefeuille de couverture à copier
  */
  HedgePortfolio(const HedgePortfolio &HedgePortfolio);

  /**
   * \brief Constructeur de la classe HedgePortfolio
   *
   * @param[in] Data, la matrice de données à utiliser pour les calculs
   * @param[in] monteCarlo, un object de la classe monteCarlo pour les calculs du price et delta
   *
  */
  HedgePortfolio(PnlMat* Data, MonteCarlo* monteCarlo);

  /**
   * \brief updateCompo met à jour la composition du portefeuille couverture
   *
   * @param[in] Data, la matrice de données à utiliser pour les calculs
   *
  */
  void updateCompo(PnlMat* Data);

  /**
   * \brief HedgeError boucle sur les données aux dates de rebalancement et renvoie le P&L
   *
   * @param[in] marketData, la matrice de données à utiliser pour les calculs
   *
  */
  double HedgeError(PnlMat * marketData);

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


#endif
