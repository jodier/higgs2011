/*-------------------------------------------------------------------------*/

#include <cmath>
#include <cstring>
#include <iostream>

#include "main.h"

/*-------------------------------------------------------------------------*/

void core::execute(TChain *chain)
{
	std::cout << "/*-------------------------------------------------------------------------*/" << std::endl;
	std::cout << "/* EXECUTE                                                                 */" << std::endl;
	std::cout << "/*-------------------------------------------------------------------------*/" << std::endl;

	THiggsBuilder algo(chain);
	algo.Loop();
}

/*-------------------------------------------------------------------------*/

void THiggsBuilder::Loop(void)
{
	Int_t nPV2;
	Int_t nPV3;

	int elNr0 = 0;
	int elNr1 = 0;
	int elNr2 = 0;
	int elNr3 = 0;
	int elNr4 = 0;
	int elNr5 = 0;
	int elNr6 = 0;
	int elNr7 = 0;
	int elNr8 = 0;
	int elNr9 = 0;

	int muNr0 = 0;
	int muNr1 = 0;
	int muNr2 = 0;
	int muNr3 = 0;
	int muNr4 = 0;
	int muNr5 = 0;
	int muNr6 = 0;
	int muNr7 = 0;
	int muNr8 = 0;
	int muNr9 = 0;
	int muNr10 = 0;
	int muNr11 = 0;
	int muNr12 = 0;
	int muNr13 = 0;

	Float_t __el_et = core::configFltLookup("higgs_el_et");
	Float_t __mu_staco_pt = core::configFltLookup("higgs_mu_staco_pt");
	Float_t __mu_calo_pt = core::configFltLookup("higgs_mu_calo_pt");

	Int_t n;

	bool isOk;

	Float_t calo_eta;
	Float_t calo_phi;
	Float_t staco_eta;
	Float_t staco_phi;

	const Long64_t eventNr = fChain->GetEntries();

	for(Long64_t event = 0; event < eventNr; event++)
	{
		/*---------------------------------------------------------*/

		LoadEvent(event, eventNr);

		/*---------------------------------------------------------*/
		/* AT LEAST 3 PRIMARY TRACKS AND LAR ERROR		   */
		/*---------------------------------------------------------*/

		nPV2 = 0;
		nPV3 = 0;

		for(Int_t i = 0; i < Int_t(vxp_nTracks->size()); i++)
		{
			if(vxp_nTracks->at(i) >= 2) {
				nPV2++;
			}

			if(vxp_nTracks->at(i) >= 3) {
				nPV3++;
			}
		}

		/**/
#ifdef __IS_MC
		Bool_t isOkVertex = (nPV3 > 0) && (0x00000000001);
#else
		Bool_t isOkVertex = (nPV3 > 0) && (larError != 2);
#endif
		/*---------------------------------------------------------*/
		/* TRIGGERS						   */
		/*---------------------------------------------------------*/

		Bool_t isOkElTrigger = getElTrigger();
		Bool_t isOkMuTrigger = getMuTrigger();

		/*---------------------------------------------------------*/
		/* ELECTRON						   */
		/*---------------------------------------------------------*/

		for(Int_t index = 0; index < el_n; index++)
		{
			elNr0++;

			if(isOkVertex == false) {
				continue;
			}

			elNr1++;

			if(isOkElTrigger == false) {
				continue;
			}

			elNr2++;

			if(el_author->at(index) != 1
			   &&
			   el_author->at(index) != 3
			 ) {
				continue;
			}

			elNr3++;

			if(el_loosePP_at(index) == 0) {
				continue;
			}

			elNr4++;

			if(fabs(el_cl_eta->at(index)) > 2.47f) {
				continue;
			}

			elNr5++;

			if(electronGetEt(index) < __el_et) {
				continue;
			}

			elNr6++;

			if((el_OQ->at(index) & 1446) != 0) {
				continue;
			}

			elNr7++;

			if(fabs(el_trackz0pvunbiased->at(index)) > 10.0f) {
				continue;
			}

			elNr8++;

			isOk = true;

			for(Int_t xedni = 0; xedni < el_n; xedni++)
			{
				if(index != xedni
				   &&
				   el_trackd0->at(index) == el_trackd0->at(xedni)
				   &&
				   el_trackz0->at(index) == el_trackz0->at(xedni)
				   &&
				   el_tracktheta->at(index) == el_tracktheta->at(xedni)
				   &&
				   el_trackphi->at(index) == el_trackphi->at(xedni)
				   &&
				   el_trackqoverp->at(index) == el_trackqoverp->at(xedni)
				 ) {
					if(checkObject(xedni, TYPE_ELECTRON, __el_et, __mu_staco_pt, __mu_calo_pt) != false && electronGetEt(index) <= electronGetEt(xedni))
					{
						isOk = false;

						break;
					}
				}
			}

			if(isOk == false) {
				continue;
			}

			elNr9++;
		}

		/*---------------------------------------------------------*/
		/* MUON STACO						   */
		/*---------------------------------------------------------*/

		for(Int_t index = 0; index < mu_staco_n; index++)
		{
			muNr0++;

			if(isOkVertex == false) {
				continue;
			}

			muNr1++;

			if(isOkMuTrigger == false) {
				continue;
			}

			muNr2++;

			/**/ if((mu_staco_author->at(index) == 6 || mu_staco_author->at(index) == 7) && mu_staco_isStandAloneMuon->at(index) == 0)
			{
				muNr3++;

				if(fabs(mu_staco_eta->at(index)) > 2.7f) {
					continue;
				}

				muNr4++;

				if(mu_staco_pt->at(index) < __mu_staco_pt) {
					continue;
				}

				muNr5++;

				if(mu_staco_expectBLayerHit->at(index) != 0 && mu_staco_nBLHits->at(index) <= 0) {
					continue;
				}

				muNr6++;

				if(mu_staco_nPixHits->at(index) + mu_staco_nPixelDeadSensors->at(index) < 2) {
					continue;
				}

				muNr7++;

				if(mu_staco_nSCTHits->at(index) + mu_staco_nSCTDeadSensors->at(index) < 6) {
					continue;
				}

				muNr8++;

				if(mu_staco_nPixHoles->at(index) + mu_staco_nSCTHoles->at(index) > 2) {
					continue;
				}

				muNr9++;

				n = mu_staco_nTRTHits->at(index) + mu_staco_nTRTOutliers->at(index);

				if(fabs(mu_staco_eta->at(index)) < 1.9f)
				{
					if(n < 6 || mu_staco_nTRTOutliers->at(index) > 0.9f * n) {
						continue;
					}
				}
				else
				{
					if(n > 5 && mu_staco_nTRTOutliers->at(index) > 0.9f * n) {
						continue;
					}
				}

				muNr10++;

				if(fabs(mu_staco_d0_exPV->at(index)) > 1.0f) {
					continue;
				}

				muNr11++;

				if(fabs(mu_staco_z0_exPV->at(index)) > 10.0f) {
					continue;
				}

				muNr12++;
			}
			else if(mu_staco_author->at(index) == 6 && mu_staco_isStandAloneMuon->at(index) != 0)
			{
				muNr3++;

				if(fabs(mu_staco_eta->at(index)) < 2.5f
				   ||
				   fabs(mu_staco_eta->at(index)) > 2.7f
				 ) {
					continue;
				}

				muNr4++;

				if(mu_staco_pt->at(index) < __mu_staco_pt) {
					continue;
				}

				muNr5++;

				Int_t mu_cscetahits = mu_staco_nCSCEtaHits->at(index);
				Int_t mu_cscphihits = mu_staco_nCSCPhiHits->at(index);
				Int_t mu_emhits = mu_staco_nMDTEMHits->at(index);
				Int_t mu_eohits = mu_staco_nMDTEOHits->at(index);

				if((mu_cscetahits + mu_cscphihits) == 0 || mu_emhits == 0 || mu_eohits == 0) {
					continue;
				}

				muNr6++;
				muNr7++;
				muNr8++;
				muNr9++;
				muNr10++;
				muNr11++;
				muNr12++;
			}
			else {
				continue;
			}

			muNr13++;
		}

		/*---------------------------------------------------------*/
		/* MUON CALO						   */
		/*---------------------------------------------------------*/

		for(Int_t index = 0; index < mu_calo_n; index++)
		{
			muNr0++;

			if(isOkVertex == false) {
				continue;
			}

			muNr1++;

			if(isOkMuTrigger == false) {
				continue;
			}

			muNr2++;

			if(mu_calo_author->at(index) != 16) {
				continue;
			}

			if(mu_calo_caloMuonIdTag->at(index) < 11
			   &&
			   mu_calo_caloLRLikelihood->at(index) < 0.9f
			 ) {
				continue;
			}

			muNr3++;

			if(fabs(mu_calo_eta->at(index)) > 0.1f) {
				continue;
			}

			muNr4++;

			if(mu_calo_pt->at(index) < __mu_calo_pt) {
				continue;
			}

			muNr5++;

			if(mu_calo_expectBLayerHit->at(index) != 0 && mu_calo_nBLHits->at(index) <= 0) {
				continue;
			}

			muNr6++;

			if(mu_calo_nPixHits->at(index) + mu_calo_nPixelDeadSensors->at(index) < 2) {
				continue;
			}

			muNr7++;

			if(mu_calo_nSCTHits->at(index) + mu_calo_nSCTDeadSensors->at(index) < 6) {
				continue;
			}

			muNr8++;

			if(mu_calo_nPixHoles->at(index) + mu_calo_nSCTHoles->at(index) > 2) {
				continue;
			}

			muNr9++;

			n = mu_calo_nTRTHits->at(index) + mu_calo_nTRTOutliers->at(index);

			if(n > 5 && mu_calo_nTRTOutliers->at(index) > n * 0.9) {
				continue;
			}

			muNr10++;

			if(fabs(mu_calo_d0_exPV->at(index)) > 1.0f) {
				continue;
			}

			muNr11++;

			if(fabs(mu_calo_z0_exPV->at(index)) > 10.0f) {
				continue;
			}

			muNr12++;

			isOk = true;

			calo_eta = -logf(tanf(0.5f * mu_calo_id_theta->at(index)));
			calo_phi = mu_calo_id_phi->at(index);

			for(Int_t xedni = 0; xedni < mu_staco_n; xedni++)
			{
				if(true || mu_staco_author->at(xedni) == 6 || mu_staco_author->at(xedni) == 7)
				{
					staco_eta = -logf(tanf(0.5f * mu_staco_id_theta->at(xedni)));
					staco_phi = mu_staco_id_phi->at(xedni);

					if(__dR2(calo_eta, staco_eta, calo_phi, staco_phi) < 0.1f * 0.1f)
					{
						isOk = false;

						break;
					}
				}
			}

			if(isOk == false) {
				continue;
			}

			muNr13++;
		}

		/*---------------------------------------------------------*/
	}

	std::cout << std::endl;

	std::cout << "#############################################################################" << std::endl;
	std::cout << "# ELECTRON STACO                                                            #" << std::endl;
	std::cout << "#############################################################################" << std::endl;

	std::cout << "before any cut    : " << elNr0 << std::endl;
	std::cout << "after vertex      : " << elNr1 << std::endl;
	std::cout << "after trigger     : " << elNr2 << std::endl;
	std::cout << "after author=1||3 : " << elNr3 << std::endl;
	std::cout << "after loose++     : " << elNr4 << std::endl;
	std::cout << "after |η|<2.47    : " << elNr5 << std::endl;
	std::cout << "after pt>7        : " << elNr6 << std::endl;
	std::cout << "after OQ          : " << elNr7 << std::endl;
	std::cout << "after z0          : " << elNr8 << std::endl;
	std::cout << "after e-e overlap : " << elNr9 << std::endl;

	std::cout << "#############################################################################" << std::endl;
	std::cout << "# MUON STACO                                                                #" << std::endl;
	std::cout << "#############################################################################" << std::endl;

	std::cout << "before any cut     : " << muNr0 << std::endl;
	std::cout << "after vertex       : " << muNr1 << std::endl;
	std::cout << "after trigger      : " << muNr2 << std::endl;
	std::cout << "after author=6||7  : " << muNr3 << std::endl;
	std::cout << "after |η|<2.5      : " << muNr4 << std::endl;
	std::cout << "after pt>6         : " << muNr5 << std::endl;
	std::cout << "after b-Layer      : " << muNr6 << std::endl;
	std::cout << "after Pix          : " << muNr7 << std::endl;
	std::cout << "after SCT          : " << muNr8 << std::endl;
	std::cout << "after holes        : " << muNr9 << std::endl;
	std::cout << "after TRT/outliers : " << muNr10 << std::endl;
	std::cout << "after d0           : " << muNr11 << std::endl;
	std::cout << "after z0           : " << muNr12 << std::endl;
	std::cout << "after calo overlap : " << muNr13 << std::endl;

	std::cout << "#############################################################################" << std::endl;
}

/*-------------------------------------------------------------------------*/

