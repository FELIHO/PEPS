using System;
using System.Collections.Generic;
using System.Linq;
using PricingLibrary.Utilities.MarketDataFeed;
using System.Runtime.InteropServices;


namespace ProjetNet.Models
{
    internal class ParametersEstimation
    {

        #region Private Fields

        private double[,] correlation;
        private double[] volatility;

        #endregion Private Fields

        #region Public Properties

        public double[,] Correlation
        {
            get { return correlation; }
        }

        public double[] Volatility
        {
            get { return volatility; }
        }
        #endregion Private Properties



        #region Public Constructors

        public ParametersEstimation(List<DataFeed> dataFeedList, DateTime rebalancingDate, int estimationWindow)
        {
            List<DataFeed> usefulData = usefulDataFeeds(dataFeedList, rebalancingDate, estimationWindow);
            double[,] assetValues = getAssetValues(usefulData);
            double[,] logAssests = computeLogAssets(assetValues);
            double[,] corrMatrix = computeCorrelationMatrix(logAssests);
            double[] volTable = computeWREVolatilityTable(logAssests);
            this.correlation = corrMatrix;
            this.volatility = volTable;
        }

        #endregion Public Constructors

        //assetReturns

        private List<DataFeed> usefulDataFeeds(List<DataFeed> dataFeedList, DateTime rebalancingDate, int estimationWindow)
        {
            int indice = 0;
            while(DateTime.Compare(dataFeedList[indice].Date, rebalancingDate) != 0) { indice++;}
            return dataFeedList.GetRange(indice - estimationWindow+1, estimationWindow);
        }

        private double[,] getAssetValues(List<DataFeed> dataFeedList)
        {
            int nbDate = dataFeedList.Count;
            int nbUnderlings = dataFeedList[0].PriceList.Count;
            String[] underlyingIds = dataFeedList[0].PriceList.Keys.ToArray();
            double[,] assets = new double[nbDate, nbUnderlings];
            for (int i = 0; i < nbDate; i++)
            {
                for (int j = 0; j < nbUnderlings; j++)
                {
                    assets[i, j] = (double)dataFeedList[i].PriceList[underlyingIds[j]];
                }
            }
            return assets;
        }

        private double[,] computeLogAssets(double[,] assetValues)
        {
            int nbValues = assetValues.GetLength(0);
            int nbAssets = assetValues.GetLength(1);
            double[,] assetReturns = new double[nbValues - 1, nbAssets];
            for (int i = 0; i < nbValues - 1; i++)
            {
                for (int j = 0; j < nbAssets; j++)
                {
                    assetReturns[i, j] = (double)Math.Log(assetValues[i + 1, j] / assetValues[i, j]);
                }
            }
            return assetReturns;
        }

        private double[,] computeCovarianceMatrix(double[,] returns)
        {
            return Accord.Statistics.Measures.Covariance(returns);
        }


        private double[,] computeCorrelationMatrix(double[,] returns)
        {
            var covarianceMatrix = computeCovarianceMatrix(returns);
            int size = covarianceMatrix.GetLength(0);
            double[,] correlationMatrix = new Double[size, size];
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    correlationMatrix[i, j] = covarianceMatrix[i, j] / (Math.Sqrt(covarianceMatrix[i, i]) * Math.Sqrt(covarianceMatrix[j, j]));
                }
            }
            return correlationMatrix;
        }

        private double[] computeVolatilityTable(double[,] returns)
        {
            var covarianceMatrix = computeCovarianceMatrix(returns);
            int numberOfDaysPerYear = 252;
            int size = covarianceMatrix.GetLength(0);
            double[] volatilityTable = new Double[size];
            for (int i = 0; i < size; i++)
            {
                volatilityTable[i] = Math.Sqrt(covarianceMatrix[i, i] * numberOfDaysPerYear);
            }
            return volatilityTable;
        }

    }
}
