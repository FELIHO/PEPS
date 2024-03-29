#pragma once


#ifndef CALL_HPP
#define CALL_HPP


#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_cdf.h"
#include "Basket.hpp"
#include <cmath>
#include "../AssetModel/BlackScholesModel.hpp"

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

	double price_formuleBS( BlackScholesModel *bc,double T, double strike,double r,double sigma);

	virtual Call* clone();

};

#endif
