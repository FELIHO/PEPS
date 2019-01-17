#pragma once
#define DLLEXP   __declspec( dllexport )
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_cdf.h"
#include "Option/Basket.hpp"
#include <cmath>

namespace Computations {
	class Call : public Basket
	{

	public:

		/**
		* Default Constructor
		* Initialise everything to zero. Shouldn't be used.
		*/
		DLLEXP Call();

		/*
		* Copy Constructor
		*/
		DLLEXP Call(const Call &C);

		/*
		* Affectation
		*/
		DLLEXP Call& operator=(const Call &C);

		/**
		* Destructor
		*/
		DLLEXP ~Call();

		/**
		* Constructor with parameters
		*/
		DLLEXP Call(double T, double strike_);

		virtual Call* clone();

	};

}