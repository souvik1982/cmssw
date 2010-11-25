
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_10_1_TN9.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_11_1_0OK.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_12_1_SgQ.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_13_1_UwN.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_14_1_12w.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_17_1_0wE.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_18_1_da6.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_19_1_3Cn.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_1_1_kqD.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_20_1_bz7.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_21_1_eEs.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_22_1_Rip.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_23_1_mdr.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_24_1_bhy.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_25_1_YxN.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_26_1_X2Z.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_27_1_5il.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_28_1_LTD.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_29_1_KTw.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_2_1_mns.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_30_1_NHS.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_31_1_ryB.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_32_1_AMH.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_33_1_4e7.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_34_1_noT.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_35_1_b9I.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_36_1_i6G.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_37_1_deS.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_38_1_85o.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_39_1_yRk.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_3_1_aiv.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_40_1_60G.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_41_1_CLU.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_4_1_h2K.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_5_1_SpX.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_6_1_Pyx.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_7_1_bIJ.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_8_1_yeM.root?svcClass=cmst3&stageHost=castorcms',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_9_1_JiK.root?svcClass=cmst3&stageHost=castorcms',
])
