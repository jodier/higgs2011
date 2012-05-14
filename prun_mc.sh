#!/bin/sh

OUTDS=user.etiouchi.l_analysis_ZJets.version1

INDS=\
mc11_7TeV.107650.AlpgenJimmyZeeNp0_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107651.AlpgenJimmyZeeNp1_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107652.AlpgenJimmyZeeNp2_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107653.AlpgenJimmyZeeNp3_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107654.AlpgenJimmyZeeNp4_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107655.AlpgenJimmyZeeNp5_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107660.AlpgenJimmyZmumuNp0_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107661.AlpgenJimmyZmumuNp1_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107662.AlpgenJimmyZmumuNp2_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107663.AlpgenJimmyZmumuNp3_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107664.AlpgenJimmyZmumuNp4_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/,\
mc11_7TeV.107665.AlpgenJimmyZmumuNp5_pt20.merge.NTUP_HSG2.e835_s1299_s1300_r3043_r2993_p956/

cp core/include/THiggsD3PD_MC.h core/include/THiggsD3PD.h

prun --exec "l_analysis --grid=prun --enable-ER --enable-SF -o output.root \ %IN" --bexec "make ALL" \
--athenaTag 17.0.4 \
--writeInputToTxt "IN:input.txt" \
--outDS $OUTDS \
--inDS $INDS \
--extFile analysis/ilumicalc_period_AllYear_Higgs_4l_2e2mu.root,analysis/Mu_MC11bprime_analysis.root \
--outputs output.root \
--excludedSite ANALY_GLASGOW

