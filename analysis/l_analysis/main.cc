/*-------------------------------------------------------------------------*/

#include <cmath>
#include <cstring>
#include <iostream>

#include <egammaAnalysisUtils/CaloIsoCorrection.h>

#include "main.h"

/*-------------------------------------------------------------------------*/

void core::execute(TChain *chain)
{
	std::cout << "/*-------------------------------------------------------------------------*/" << std::endl;
	std::cout << "/* EXECUTE                                                                 */" << std::endl;
	std::cout << "/*-------------------------------------------------------------------------*/" << std::endl;

	TLeptonFinder algo(chain);
	algo.Loop();

	/**/

	TFile file(core::output.c_str(), "recreate");

	file.cd();

	algo.m_tree1.Write();
	algo.m_tree2.Write();

	file.Close();

}
/*-------------------------------------------------------------------------*/

extern Float_t getMassCut(Float_t H_mass);

/*-------------------------------------------------------------------------*/

void TLeptonFinder::Loop(void)
{
	Int_t elStacoNr = 0;
	Int_t muStacoNr = 0;

	Int_t elStacoIndexNr;
	Int_t muStacoIndexNr;

	Int_t elStacoIndexArray[1024];
	Int_t muStacoIndexArray[1024];

	TLeptonType elStacoTypeArray[1024];
	TLeptonType muStacoTypeArray[1024];

	Float_t __el_et = core::configFltLookup("higgs_el_et");
	Float_t __mu_staco_pt = core::configFltLookup("higgs_mu_staco_pt");
	Float_t __mu_calo_pt = core::configFltLookup("higgs_mu_calo_pt");

	const Long64_t eventNr = fChain->GetEntries();

	for(Long64_t event = 0; event < eventNr; event++)
	{
		/*---------------------------------------------------------*/

		LoadEvent(event, eventNr);

		if(event == 0)
		{
			triggerInit();
		}

		fixeEnergy();

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

		//std::cout << "RunNumber" << RunNumber << std::endl;

		/*---------------------------------------------------------*/
		/* SELECTIONS						   */
		/*---------------------------------------------------------*/

		elStacoIndexNr = 0;
		muStacoIndexNr = 0;

		if((isOkElTrigger != false || isOkMuTrigger != false) && isOkVertex != false)
		{
			for(Int_t i = 0; i < mu_staco_n; i++)
			{
				if(checkObject(i, TYPE_MUON_STACO, __el_et, __mu_staco_pt, __mu_calo_pt) != false)
				{
					muStacoIndexArray[muStacoIndexNr] = i;
					muStacoTypeArray[muStacoIndexNr] = TYPE_MUON_STACO;

					if(isOkMuTrigger) {
						muStacoNr++;
					}
					muStacoIndexNr++;
				}
			}

			for(Int_t i = 0; i < el_n; i++)
			{
				if(checkObject(i, TYPE_ELECTRON, __el_et, __mu_staco_pt, __mu_calo_pt) != false)
				{
					if(checkOverlapping(i, TYPE_ELECTRON, __el_et, __mu_staco_pt, __mu_calo_pt, muStacoIndexNr, muStacoIndexArray, muStacoTypeArray) != false)
					{
						elStacoIndexArray[elStacoIndexNr] = i;
						elStacoTypeArray[elStacoIndexNr] = TYPE_ELECTRON;

						if(isOkElTrigger) {
							elStacoNr++;
						}
						elStacoIndexNr++;
					}
				}
			}
		}

		/*---------------------------------------------------------*/
		/* Loop electron (staco)				   */
		/*---------------------------------------------------------*/

		m_l[0].n = elStacoIndexNr;

		for(Int_t i = 0; i < elStacoIndexNr; i++)
		{
			Int_t index = elStacoIndexArray[i];

			/**/

			m_l[0].RunNumber = RunNumber;
			m_l[0].EventNumber = EventNumber;
			m_l[0].LumiBlock = lbn;

			m_l[0].nPV2 = nPV2;
			m_l[0].nIntPerXing = averageIntPerXing;

			m_l[0].elTrigger = isOkElTrigger;
			m_l[0].muTrigger = isOkMuTrigger;

			/**/

			m_l[0].l_z0[i] = el_trackz0pvunbiased->at(index);
			m_l[0].l_d0[i] = el_trackd0pvunbiased->at(index);

			m_l[0].weight1[i] = eventGetWeight1();
			m_l[0].weight2[i] = eventGetWeight2();
			m_l[0].weight3[i] = eventGetWeight3(index, TYPE_ELECTRON);

			m_l[0].l_tight[i] = el_tight->at(index) != 0;

			m_l[0].l_triggerMatch[i] = triggerMatch(index, TYPE_ELECTRON);

			m_l[0].l_charge[i] = el_charge->at(index);
			m_l[0].l_e[i] = el_cl_E->at(index);
			m_l[0].l_pt[i] = electronGetEt(index);
			m_l[0].l_eta[i] = electronGetEtaDirection(index);
			m_l[0].l_phi[i] = electronGetPhiDirection(index);

			m_l[0].l_clIso20[i] = CaloIsoCorrection::GetNPVCorrectedIsolation(nPV2, el_etas2->at(index), 20, __isMC, el_Etcone20->at(index), CaloIsoCorrection::ELECTRON) / electronGetEt(index);
			m_l[0].l_tkIso20[i] = el_ptcone20->at(index) / electronGetEt(index);
			m_l[0].l_d0sigma[i] = fabs(el_trackd0pvunbiased->at(index) / el_tracksigd0pvunbiased->at(index));

			for(Int_t j = i + 1; j < elStacoIndexNr; j++)
			{
				Int_t index2 = elStacoIndexArray[j];

				if(sqrtf(__dR2(m_l[0].l_eta[i], electronGetEtaDirection(index2), m_l[0].l_phi[i], electronGetPhiDirection(index2))) < 0.20f)
				{
					m_l[0].l_tkIso20[i] -= el_trackpt->at(index2) / m_l[0].l_pt[i];
				}

				if(sqrtf(__dR2(m_l[0].l_eta[i], electronGetEtaDirection(index2), m_l[0].l_phi[i], electronGetPhiDirection(index2))) < 0.18f)
				{
					m_l[0].l_clIso20[i] -= electronGetEt(index2) / m_l[0].l_pt[i];
				}
			}

			m_l[0].l_f1[i] = el_f1->at(index);
			m_l[0].l_rphi[i] = el_rphi->at(index);
			m_l[0].l_nBlayerHits[i] = el_nBLHits->at(index);
			m_l[0].l_nPixelHits[i] = el_nPixHits->at(index);
			m_l[0].l_rTRT[i] = (el_nTRTHits->at(index) + el_nTRTOutliers->at(index)) > 0 ? float(el_nTRTHighTHits->at(index) + el_nTRTHighTOutliers->at(index)) / float(el_nTRTHits->at(index) + el_nTRTOutliers->at(index)) : 0.0f;

			/**/
#ifdef __IS_MC
			m_l[0].l_type[i] = el_type->at(index);
			m_l[0].l_origin[i] = el_origin->at(index);
			m_l[0].l_typebkg[i] = el_typebkg->at(index);
			m_l[0].l_originbkg[i] = el_originbkg->at(index);
			m_l[0].l_truth_type[i] = el_truth_type->at(index);
			m_l[0].l_truth_mothertype[i] = el_truth_mothertype->at(index);
#else
			m_l[0].l_type[i] = -999999;
			m_l[0].l_origin[i] = -999999;
			m_l[0].l_typebkg[i] = -999999;
			m_l[0].l_originbkg[i]= -999999;
			m_l[0].l_truth_type[i] = -999999;
			m_l[0].l_truth_mothertype[i] = -999999;
#endif
		}

		/*---------------------------------------------------------*/
		/* Loop Muon (STACO)					   */
		/*---------------------------------------------------------*/

		m_l[1].n = muStacoIndexNr;

		for(Int_t i = 0; i < muStacoIndexNr; i++)
		{
			Int_t index = muStacoIndexArray[i];

			/**/

			m_l[1].RunNumber = RunNumber;
			m_l[1].EventNumber = EventNumber;
			m_l[1].LumiBlock = lbn;

			m_l[1].nPV2 = nPV2;
			m_l[1].nIntPerXing = averageIntPerXing;

			m_l[1].elTrigger = isOkElTrigger;
			m_l[1].muTrigger = isOkMuTrigger;

			/**/

			m_l[1].l_z0[i] = mu_staco_z0_exPV->at(index);
			m_l[1].l_d0[i] = mu_staco_d0_exPV->at(index);

			m_l[1].weight1[i] = eventGetWeight1();
			m_l[1].weight2[i] = eventGetWeight2();
			m_l[1].weight3[i] = eventGetWeight3(index, TYPE_MUON_STACO);

			m_l[1].l_tight[i] = mu_staco_tight->at(index) != 0;
			m_l[1].l_triggerMatch[i] = triggerMatch(index, TYPE_MUON_STACO);

			m_l[1].l_charge[i] = mu_staco_charge->at(index);
			m_l[1].l_e[i] = mu_staco_E->at(index);
			m_l[1].l_pt[i] = mu_staco_pt->at(index);
			m_l[1].l_eta[i] = mu_staco_eta->at(index);
			m_l[1].l_phi[i] = mu_staco_phi->at(index);

			m_l[1].l_clIso20[i] = (mu_staco_etcone20->at(index) - Float_t(nPV2 - 1) * 33.5f) / mu_staco_pt->at(index);
			m_l[1].l_tkIso20[i] = mu_staco_ptcone20->at(index) / mu_staco_pt->at(index);
			m_l[1].l_d0sigma[i] = fabs(mu_staco_trackIPEstimate_d0_unbiasedpvunbiased->at(index) / mu_staco_trackIPEstimate_sigd0_unbiasedpvunbiased->at(index));

			for(Int_t j = i + 1; j < muStacoIndexNr; j++)
			{
				Int_t index2 = muStacoIndexArray[j];

				if(sqrtf(__dR2(m_l[1].l_eta[i], mu_staco_eta->at(index2), m_l[1].l_phi[i], mu_staco_phi->at(index2))) < 0.20f)
				{
					m_l[1].l_tkIso20[i] -= ((mu_staco_id_qoverp_exPV->at(index2) != 0.0f) ? sinf(mu_staco_id_theta_exPV->at(index2)) / fabs(mu_staco_id_qoverp_exPV->at(index2)) : 0.0f) / m_l[1].l_pt[i];
				}
			}

			m_l[1].l_f1[i] = -999999;
			m_l[1].l_rphi[i] = -999999;
			m_l[1].l_nBlayerHits[i] = -999999;
			m_l[1].l_nPixelHits[i] = -999999;
			m_l[1].l_rTRT[i] = -999999;

			/**/

			m_l[1].l_type[i] = -999999;
			m_l[1].l_origin[i] = -999999;
			m_l[1].l_typebkg[i] = -999999;
			m_l[1].l_originbkg[i] = -999999;
#ifdef __IS_MC
			m_l[1].l_truth_type[i] = mu_staco_truth_type->at(index);
			m_l[1].l_truth_mothertype[i] = mu_staco_truth_mothertype->at(index);
#else
			m_l[1].l_truth_type[i] = -999999;
			m_l[1].l_truth_mothertype[i] = -999999;
#endif
		}

		/*---------------------------------------------------------*/

		if(elStacoIndexNr >= 2
		   ||
		   muStacoIndexNr >= 2
		 ) {
			m_tree1.Fill();
			m_tree2.Fill();
		}

		/*---------------------------------------------------------*/
	}

	std::cout << "elStacoNr: " << elStacoNr << "    " << std::endl;
	std::cout << "muStacoNr: " << muStacoNr << "    " << std::endl;
}

/*-------------------------------------------------------------------------*/

