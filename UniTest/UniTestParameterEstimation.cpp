#include "stdafx.h"
#include "CppUnitTest.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestPricer
{		
	TEST_CLASS(UnitTestCall)
	{
		PnlMat *path = pnl_mat_create_from_file();
	public:

		TEST_METHOD(TestPricerCallatZero)
		{
			Assert::AreEqu
		}

		TEST_METHOD(TestDeltaCallatZero)
		{
			Assert::AreEqual(0,1);
		}

		TEST_METHOD(TestPricerCallnotZero)
		{
			Assert::AreEqual(0, 1);
		}


		TEST_METHOD(TestPricerDeltanotZero)
		{
			Assert::AreEqual(1, 1);
		}

	};
}