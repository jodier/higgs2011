#!/bin/sh

OUTDS=user.etiouchi.l_analysis_Muon.data11_p997.version1_test2

INDS=\
data11_7TeV.00183081.physics_Muons.merge.NTUP_HSG2.r2603_p659_p996_p997/


cp core/include/THiggsD3PD_DATA.h core/include/THiggsD3PD.h

prun --exec "l_analysis --grid=prun --enable-ER -o output.root \ %IN" --bexec "make ALL" \
--athenaTag 17.0.4 \
--writeInputToTxt "IN:input.txt" \
--outDS $OUTDS \
--inDS $INDS \
--extFile analysis/ilumicalc_period_AllYear_Higgs_4l_2e2mu.root,analysis/MC11c.prw.root \
--outputs output.root \
#--excludedSite ANALY_GLASGOW

