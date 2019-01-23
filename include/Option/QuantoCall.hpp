#pragma once
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_cdf.h"
#include "QuantoBasket.hpp"
#include <cmath>
#include <assert.h>

namespace Computations {
	class QuantoCall : public QuantoBasket
	{

	public:

		/**
		* Default Constructor
		* Initialise everything to zero. Shouldn't be used.
		*/
		 QuantoCall();

		/*
		* Copy Constructor
		*/
		 QuantoCall(const QuantoCall &C);

		/*
		* Affectation
		*/
		 QuantoCall& operator=(const QuantoCall &C);

		/**
		* Destructor
		*/
		 ~QuantoCall();

		/**
		* Constructor with parameters
		*/
		 QuantoCall(double T, double strike_);

		virtual QuantoCall* clone();

	};

}