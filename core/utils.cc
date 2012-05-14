/*-------------------------------------------------------------------------*/

#include <cstdlib>

#include <TMath.h>

#include "include/core.h"

/*-------------------------------------------------------------------------*/

Char_t getlumiPeriod(Int_t RunNumber)
{
	Char_t result;

	/**/ if(RunNumber >= 177986 && RunNumber <= 178109) {
		result = 'B';
	}
	else if(RunNumber >= 178163 && RunNumber <= 178264) {
		result = 'C';
	}
	else if(RunNumber >= 179710 && RunNumber <= 180481) {
		result = 'D';
	}
	else if(RunNumber >= 180614 && RunNumber <= 180776) {
		result = 'E';
	}
	else if(RunNumber >= 182013 && RunNumber <= 182519) {
		result = 'F';
	}
	else if(RunNumber >= 182726 && RunNumber <= 183462) {
		result = 'G';
	}
	else if(RunNumber >= 183544 && RunNumber <= 184169) {
		result = 'H';
	}
	else if(RunNumber >= 185353 && RunNumber <= 186493) {
		result = 'I';
	}
	else if(RunNumber >= 186516 && RunNumber <= 186755) {
		result = 'J';
	}
	else if(RunNumber >= 186873 && RunNumber <= 187815) {
		result = 'K';
	}
	else if(RunNumber >= 188902 && RunNumber <= 190343) {
		result = 'L';
	}
	else if(RunNumber >= 190503 && RunNumber <= 191933) {
		result = 'M';
	}
	else
	{
		result = 0x0;
	}

	return result;
}

/*-------------------------------------------------------------------------*/

Float_t __dR2(
	Float_t eta1, Float_t eta2,
	Float_t phi1, Float_t phi2
) {
	Float_t dEta = eta1 - eta2;
	Float_t dPhi = phi1 - phi2;

	while(dPhi < -M_PI) {
		dPhi += 2.0 * M_PI;
	}

	while(dPhi >= +M_PI) {
		dPhi -= 2.0 * M_PI;
	}

	return dEta * dEta + dPhi * dPhi;
}

/*-------------------------------------------------------------------------*/

TLeptonClass getLeptonClass(TLeptonType type)
{
	TLeptonClass result;

	switch(type)
	{
		case TYPE_ELECTRON:
			result = CLASS_ELECTRON;
			break;

		case TYPE_MUON_STACO:
			result = CLASS_MUON;
			break;

		case TYPE_MUON_CALO:
			result = CLASS_MUON;
			break;

		default:
			std::cout << "Oula !!!\n" << std::endl;
			exit(1);
	}

	return result;
}

/*-------------------------------------------------------------------------*/

Bool_t buildPair(
	Int_t pair[2],
	Float_t charge1,
	Float_t charge2
) {
	/**/ if(charge1 < 0.0f
		&&
		charge2 > 0.0f
	 ) {
		pair[0] = 1;
		pair[1] = 0;
	}
	else if(charge1 > 0.0f
		&&
		charge2 < 0.0f
	 ) {
		pair[0] = 0;
		pair[1] = 1;
	}
	else
	{
		pair[0] = 0;
		pair[1] = 1;

		return false;
	}

	return true;
}

/*-------------------------------------------------------------------------*/

Bool_t buildPairs(
	Int_t pair1[2],
	Int_t pair2[2],
	Int_t pair3[2],
	Int_t pair4[2],
	TLeptonType type1,
	TLeptonType type2,
	TLeptonType type3,
	TLeptonType type4,
	Float_t charge1,
	Float_t charge2,
	Float_t charge3,
	Float_t charge4
) {
	/**/ if(getLeptonClass(type1) == getLeptonClass(type2)
		&&
		getLeptonClass(type2) == getLeptonClass(type3)
		&&
		getLeptonClass(type3) == getLeptonClass(type4)
	 ) {
		Int_t negatives[4];
		Int_t positives[4];

		Int_t negativeNr = 0;
		Int_t positiveNr = 0;

		/**/ if(charge1 < 0.0f) {
			negatives[negativeNr++] = 0;
		}
		else if(charge1 > 0.0f) {
			positives[positiveNr++] = 0;
		}

		/**/ if(charge2 < 0.0f) {
			negatives[negativeNr++] = 1;
		}
		else if(charge2 > 0.0f) {
			positives[positiveNr++] = 1;
		}

		/**/ if(charge3 < 0.0f) {
			negatives[negativeNr++] = 2;
		}
		else if(charge3 > 0.0f) {
			positives[positiveNr++] = 2;
		}

		/**/ if(charge4 < 0.0f) {
			negatives[negativeNr++] = 3;
		}
		else if(charge4 > 0.0f) {
			positives[positiveNr++] = 3;
		}

		if(positiveNr != 2
		   ||
		   negativeNr != 2
		 ) {
			return false;
		}

		/* 1st quadruplet */

		pair1[0] = positives[0];
		pair1[1] = negatives[0];

		pair2[0] = positives[1];
		pair2[1] = negatives[1];

		/* 2nd quadruplet */

		pair3[0] = positives[0];
		pair3[1] = negatives[1];

		pair4[0] = positives[1];
		pair4[1] = negatives[0];
	}
	else if(getLeptonClass(type1) == getLeptonClass(type2)
		&&
		getLeptonClass(type3) == getLeptonClass(type4)
	 ) {
		/* 1st quadruplet */

		if(buildPair(pair1, charge1, charge2) == false
		   ||
		   buildPair(pair2, charge3, charge4) == false
		 ) {
			return false;
		}

		pair1[0] += 0;
		pair1[1] += 0;

		pair2[0] += 2;
		pair2[1] += 2;

		/* 2nd quadruplet */

		pair3[0] = pair1[0];
		pair3[1] = pair1[1];

		pair4[0] = pair2[0];
		pair4[1] = pair2[1];
	}
	else
	{
		return false;
	}

	return true;
}

/*-------------------------------------------------------------------------*/

void reIndex(
	Int_t L[4],
	Float_t M[4],
	Int_t (* func)(const void *, const void *)
) {
	Float_t U[4] = {
		M[0],
		M[1],
		M[2],
		M[3],
	};

	Float_t V[4] = {
		M[0],
		M[1],
		M[2],
		M[3],
	};

	/**/

	qsort(U, 4, sizeof(Float_t), func);

	/**/

	for(Int_t i = 0; i < 4; i++)
	{
		for(Int_t j = 0; j < 4; j++)
		{
			if(U[i] == V[j])
			{
				L[i] =     j    ;
				V[j] = -999999.0;

				break;
			}
		}
	}
}

/*-------------------------------------------------------------------------*/

Double_t binomialError(Double_t x, Double_t y)
{
	Double_t eff = x / y;

	return TMath::Sqrt(eff * (1.0 - eff) / y);
}

/*-------------------------------------------------------------------------*/

