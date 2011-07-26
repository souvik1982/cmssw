#ifndef VHbbCandidate__H 
#define VHbbCandidate__H 

#include <TLorentzVector.h>
#include <TVector2.h>
#include <vector>

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"

class VHbbCandidate {
 public:
  enum CandidateType{Zmumu, Zee, Wen, Wmun, Znn, UNKNOWN};

    VHbbCandidate(){candidateType=UNKNOWN;}

  class VectorCandidate {
  public:

    TLorentzVector p4;
    std::vector<VHbbEvent::MuonInfo> muons;
    std::vector<VHbbEvent::ElectronInfo> electrons;
    std::vector<VHbbEvent::TauInfo> taus;
    std::vector<VHbbEvent::METInfo> mets;
    
  };
  
  class HiggsCandidate {
  public:
   TLorentzVector p4;
    std::vector<VHbbEvent::SimpleJet> jets;
    float deltaTheta;
    std::vector <float> helicities;
 public:
    VHbbEvent::SimpleJet& firstJet(){return jets[0];}
    VHbbEvent::SimpleJet& secondJet(){return jets[1];}
 };
  

  void setCandidateType (CandidateType c){candidateType = c;}
  
 public:
  TLorentzVector p4(){return V.p4+H.p4;}
  CandidateType candidateType;
  HiggsCandidate H;
  VectorCandidate V;
  std::vector<VHbbEvent::SimpleJet> additionalJets;
};


#endif
