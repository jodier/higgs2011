/*-------------------------------------------------------------------------*/

#include "../include/core.h"

#include <HiggsZZ4lUtils/IsEMPlusPlusH4lDefs.h>

/*-------------------------------------------------------------------------*/

Bool_t TLeptonAnalysis::el_loosePP_at(Int_t index)
{
	float eta_s2 = el_etas2->at(index);

	float Et_cl_s2 = el_cl_E->at(index) / coshf(eta_s2);

	return isLoosePlusPlusH4l(
		eta_s2,
		Et_cl_s2,
		el_Ethad->at(index) / Et_cl_s2,
		el_Ethad1->at(index) / Et_cl_s2,
		el_reta->at(index),
		el_weta2->at(index),
		el_f1->at(index),
		el_wstot->at(index),
		(el_emaxs1->at(index) - el_Emax2->at(index)) / (el_emaxs1->at(index) + el_Emax2->at(index)),
		el_deltaeta1->at(index),
		el_nSiHits->at(index),
		el_nPixelOutliers->at(index) + el_nSCTOutliers->at(index),
		el_nPixHits->at(index),
		el_nPixelOutliers->at(index),
		false,
		false
	);
}

/*-------------------------------------------------------------------------*/

