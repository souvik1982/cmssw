import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PostProcessing.pfPostProcessingEventContent_cff import pfPostProcessingEventContent


eventCleaning = pfPostProcessingEventContent
eventCleaning += cms.untracked.vstring(
    'keep *_HBHENoiseFilterResultProducer_*_*',
    'keep *_BeamHaloSummary_*_*'
    )

