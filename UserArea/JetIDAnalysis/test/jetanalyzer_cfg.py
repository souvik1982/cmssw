import FWCore.ParameterSet.Config as cms


from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *


process = cms.Process("analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#       'file:/data1/malberti/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/patTuple_PF2PAT_999.root'
#       'file:/data1/malberti/MC_DYJetsToLL_97_1.root'
        'file:/tmp/malberti/MC_DYJetsToLL_9_1.root',
#        'root://eoscms//eos/cms/store/cmst3/user/querten/12_02_20_HZZ2l2v_pat/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/MC_DYJetsToLL_97_1.root',
#        'root://eoscms//eos/cms/store/cmst3/user/querten/12_02_20_HZZ2l2v_pat/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/MC_DYJetsToLL_98_1.root'
       ),
    skipEvents = cms.untracked.uint32(0)                        
)


# muon fileter
process.load('PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi')

process.MuonsFilter = countPatMuons.clone(
    src       = cms.InputTag("selectedPatMuonsPFlow"),
    minNumber = cms.uint32(2)
)

from CMG.JetIDAnalysis.jetanalyzer_cfi import *

process.pfjetanalyzer = jetanalyzer.clone(
    JetTag      = cms.InputTag("selectedPatJets",""),            
)

process.chspfjetanalyzer = jetanalyzer.clone(
    JetTag      = cms.InputTag("selectedPatJetsPFlow",""),            
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("mytree.root"),
    closeFileFast = cms.untracked.bool(True)
)

process.ana = cms.Sequence(process.pfjetanalyzer+process.chspfjetanalyzer)
process.p = cms.Path(process.MuonsFilter*process.ana)

