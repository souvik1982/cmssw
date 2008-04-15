#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/PFCandProducer/interface/PdgIdPFCandidateSelectorDefinition.h"

typedef ObjectSelector<PdgIdPFCandidateSelectorDefinition> PdgIdPFCandidateSelector;

DEFINE_ANOTHER_FWK_MODULE(PdgIdPFCandidateSelector);
