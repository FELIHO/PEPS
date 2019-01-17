#pragma once
#include "pch.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace Computations {

	class ParameterEstimation
	{
	public:
		ParameterEstimation() {}
		static PnlMat* getLogRendementMatrix(const PnlMat *path);

		static PnlMat* getCovarianceMatrix(const PnlMat *path);

		static PnlMat* getCorrelationMatrix(const PnlMat *path);

		static PnlVect* getVolatilitiesVector(const PnlMat *path);

	private:
	};



}


double makeLogonAllElements(double vectorElement);