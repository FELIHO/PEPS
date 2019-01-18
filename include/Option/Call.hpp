#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_cdf.h"
#include "Basket.hpp"
#include <cmath>

namespace Computations {
	class Call : public Basket
	{

	public:

		/**
		* Default Constructor
		* Initialise everything to zero. Shouldn't be used.
		*/
		 Call();

		/*
		* Copy Constructor
		*/
		 Call(const Call &C);

		/*
		* Affectation
		*/
		 Call& operator=(const Call &C);

		/**
		* Destructor
		*/
		 ~Call();

		/**
		* Constructor with parameters
		*/
		 Call(double T, double strike_);

		virtual Call* clone();

	};

}
