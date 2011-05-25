from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

sep_line = "-" * 50
print
print sep_line
print "CMGTools : testing multiple analysis sequences"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
#        input = cms.untracked.int32(-1)
        input = cms.untracked.int32(1000)
        )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

process.source.fileNames = cms.untracked.vstring(
    # 'file:patTuple_PF2PAT.root'
    'file:/afs/cern.ch/user/c/cbern/scratch0/patTuple_PF2PAT.root'
    )

# process.load("CMGTools.Common.sources.relval.RelValQCD_FlatPt_15_3000.CMSSW_3_11_2.MC_311_V2.source_cff")
# process.load("CMGTools.Common.sources.relval.RelValTTbar.CMSSW_3_11_2.MC_311_V2.source_cff")
# process.source.fileNames = cms.untracked.vstring(
#    'file:input.root'
#    )
# process.load("CMGTools.Common.sources.relval.RelValTTbar.CMSSW_3_11_2.MC_311_V2.source_cff")

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_testMultipleSequences.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( everything ) 
    

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_testMultipleSequences.root"))


# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')

# now, we're going to tune the default analysis sequence to our needs
# by modifying the parameters of the modules present in this sequence. 

# Select events with 2 jet ...  
# process.cmgPFJetCount.minNumber = 2
# with pT > 50.
# process.cmgPFJetSel.cut = "pt()>50"
# and MET larger than 50
# process.cmgPFMETSel.cut = "pt()>50"

# note: we're reading ttbar events

runStdPAT = False
runOnMC = False

# if not runStdPAT:
#    process.analysisSequence.remove( process.caloJetSequence )
#    process.analysisSequence.remove( process.caloMetSequence )

if runOnMC:
    process.load("CMGTools.Common.runInfoAccounting_cfi")
    process.outpath += process.runInfoAccounting

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
from CMGTools.Common.Tools.visitorUtils import replacePostfix


cloneProcessingSnippet(process, getattr(process, 'analysisSequence'), 'AK5LC')
replacePostfix(getattr(process,"analysisSequenceAK5LC"),'AK5','AK5LC') 

cloneProcessingSnippet(process, getattr(process, 'analysisSequence'), 'AK7')
replacePostfix(getattr(process,"analysisSequenceAK7"),'AK5','AK7') 

from CMGTools.Common.Tools.tuneCMGSequences import * 
tuneCMGSequences(process)

process.p = cms.Path(
    # process.analysisSequence + 
    process.analysisSequence
    + process.analysisSequenceAK5LC
    + process.analysisSequenceAK7
)

process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 



