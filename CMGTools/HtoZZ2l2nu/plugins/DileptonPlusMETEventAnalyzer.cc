#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/EventHypothesis.h"
#include "DataFormats/PatCandidates/interface/EventHypothesisLooper.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"

//
class DileptonPlusMETEventAnalyzer : public edm::EDAnalyzer 
{
public:
  DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig);
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  
private:
  inline TH1 *getHist(TString key)
  {
    if(results_.find(key)==results_.end()) return 0;
    return (TH1 *)results_[key];
  }
  std::map<TString, TObject *>  results_;
  std::map<std::string, edm::ParameterSet> objConfig_;
  ReducedMETComputer rmet_;
  ZZ2l2nuSummaryHandler summaryHandler_;
};


using namespace std;

//
DileptonPlusMETEventAnalyzer::DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig)
  : rmet_(1.0,1.0,0.0,0.0,1.0)
{
  try{
    
    edm::Service<TFileService> fs;

    summaryHandler_.initTree(  fs->make<TTree>("data","Event Summary") );

    objConfig_["Vertices"] = iConfig.getParameter<edm::ParameterSet>("Vertices");
    
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    
    TString streams[]={"ee","mumu","emu"};
    TString jetmult[]={"eq0jets","eq1jets","geq2jets"};
    TString selsteps[]={"Reco","2 leptons","2 good leptons","|M-M_{Z}|<15","3^{rd} lepton veto","=0 jets","=1 jet","#geq 2 jets"};
    const size_t nselsteps=sizeof(selsteps)/sizeof(TString);
    for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      TString cat=streams[istream];
      TFileDirectory newDir=baseDir.mkdir(cat.Data());
            
      results_[cat+"_cutflow"]=formatPlot( newDir.make<TH1F>(cat+"_cutflow", ";Steps; Events", nselsteps, 0.,nselsteps), 1,1,1,20,0,false,true,1,1,1);
      for(size_t istep=0; istep<nselsteps; istep++)
	{
	  ((TH1F *)results_[cat+"_cutflow"])->GetXaxis()->SetBinLabel(istep+1,selsteps[istep]);
	}
  
      //vertex control
      results_[cat+"_vertex_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_othervertex_sumpt"] = formatPlot( newDir.make<TH1F>(cat+"_othervertex_sumpt", ";#Sigma_{tracks} p_{T} [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_vertex_pt"] = formatPlot( newDir.make<TH1F>(cat+"_vertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_othervertex_pt"] = formatPlot( newDir.make<TH1F>(cat+"_othervertex_pt", ";|#Sigma_{tracks} #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_ngoodvertex"] = formatPlot( newDir.make<TH1F>(cat+"_ngoodvertex", ";Vertices; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);

      //genpileup
      results_[cat+"_ngenpileup"] = formatPlot( newDir.make<TH1F>(cat+"_ngenpileup", ";Pileup; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_ngoodvertex_ngenpileup"]  = formatPlot( newDir.make<TH2F>(cat+"_ngoodvertex_ngenpileup",";Pileup;Vertices; Events",25,0.,25.,25,0,25.), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_ngenpileupOOT"] = formatPlot( newDir.make<TH1F>(cat+"_ngenpileupOOT", ";Out-of-time Pileup; Events", 25, 0.,25.), 1,1,1,20,0,false,true,1,1,1);

      //lepton control
      results_[cat+"_nleptons"]    = formatPlot( newDir.make<TH1F>(cat+"_nleptons",";Lepton multiplicity; Events",4,2,6), 1,1,1,20,0,false,true,1,1,1);
      for(int ibin=1; ibin<=((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->GetNbins(); ibin++)
        {
          TString ilabel(""); ilabel+=(ibin+1);
          if(ibin==((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	  ((TH1F *)results_[cat+"_nleptons"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	}
      
      //jets
      results_[cat+"_jetpt"]    = formatPlot( newDir.make<TH1F>(cat+"_jetpt",";p_{T} [GeV/c]; Jets",50,0,200), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_jeteta"]    = formatPlot( newDir.make<TH1F>(cat+"_jeteta",";#eta; Jets",50,-2.5,2.5), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_jetfassoc"]    = formatPlot( newDir.make<TH1F>(cat+"_jetfassoc",";f_{assoc}; Jets",50,0,1), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_njets"]    = formatPlot( newDir.make<TH1F>(cat+"_njets",";Jet multiplicity; Events",4,0,4), 1,1,1,20,0,false,true,1,1,1);
      results_[cat+"_bmult"]    = formatPlot( newDir.make<TH1F>(cat+"_bmult",";b tag multiplicity (TCHEL); Events",4,0,4), 1,1,1,20,0,false,true,1,1,1);
      for(int ibin=1; ibin<=((TH1F *)results_[cat+"_njets"])->GetXaxis()->GetNbins(); ibin++)
	{
	  TString ilabel(""); ilabel+=(ibin-1);
	  if(ibin==((TH1F *)results_[cat+"_njets"])->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	  ((TH1F *)results_[cat+"_njets"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	  ((TH1F *)results_[cat+"_bmult"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	}
      results_[cat+"_btags"]             = formatPlot( newDir.make<TH1F>(cat+"_btags",";b tags (TCHE); Jets",50,-5,45), 1,1,1,20,0,false,true,1,1,1);

      //final control plots (inclusive and per jet multiplicity)
      for(size_t jstream=0; jstream<=sizeof(jetmult)/sizeof(TString); jstream++)
	{
	  TString subcat=cat;
	  if(jstream) subcat+= jetmult[jstream-1];
	  results_[subcat+"_dilepton_mass"]     = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton_sumpt"]    = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton_dphi"]    = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_dphi", ";|#Delta #phi| [rad]; Events", 50, 0.,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_dilepton_pt"]       = formatPlot( newDir.make<TH1F>(subcat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 50, 0.,300.), 1,1,1,20,0,false,true,1,1,1);
	  
	  results_[subcat+"_met"]               = formatPlot( newDir.make<TH1F>(subcat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,500.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_metresol"]          = formatPlot( newDir.make<TH1F>(subcat+"_metresol", ";#slash{E}_{T}-|#sum_{#nu}#vec{p}_{T}| [GeV/c]; Events", 50,  -100.,100.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_metsig"]            = formatPlot( newDir.make<TH1F>(subcat+"_metsig", ";#slash{E}_{T} significance; Events", 50,  0.,100.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_cormet"]            = formatPlot( newDir.make<TH1F>(subcat+"_cormet", ";jet corrected #slash{E}_{T} [GeV/c]; Events", 100,  0.,500.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_tmet"]              = formatPlot( newDir.make<TH1F>(subcat+"_tmet", ";track #slash{E}_{T} [GeV/c]; Events", 100,0.,500.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_rmet"]              = formatPlot( newDir.make<TH1F>(subcat+"_rmet", ";reduced #slash{E}_{T} [GeV/c]; Events", 100,0.,500.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_relmet"]            = formatPlot( newDir.make<TH1F>(subcat+"_relmet", ";relative #slash{E}_{T} [GeV/c]; Events", 100,0.,500.), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_met2vertex_dphi"] = formatPlot( newDir.make<TH1F>(subcat+"_met2vertex_dphi", ";#Delta #phi(#slash{E}_{T},vertex) [rad]; Events", 50, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_met2dilepton_dphi"] = formatPlot( newDir.make<TH1F>(subcat+"_met2dilepton_dphi", ";#Delta #phi(#slash{E}_{T},vertex) [rad]; Events", 50, 0,3.2), 1,1,1,20,0,false,true,1,1,1);
	  
	  if(jstream==0) results_[subcat+"_mT_otherlep"]= formatPlot( newDir.make<TH1F>(subcat+"_mT_otherlep",";Transverse mass(other leptons,MET) [GeV/c^{2}]; Events",50,0,500), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT_individualsum"]  = formatPlot( newDir.make<TH1F>(subcat+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",50,0,500), 1,1,1,20,0,false,true,1,1,1);
	  results_[subcat+"_mT_individualcorr"]           = formatPlot( newDir.make<TH2F>(subcat+"_mT_individualcorr",";Transverse mass(leading lepton,MET) [GeV/c^{2}];Transverse mass(trailer lepton,MET) [GeV/c^{2}]; Events",50,0,500,50,0,500), 1,1,1,20,0,false,true,1,1,1);
	  
	  results_[subcat+"_mT"]                = formatPlot( newDir.make<TH1F>(subcat+"_mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",50,0,1000), 1,1,1,20,0,false,true,1,1,1);
	}
    }
  }catch(std::exception &e){
    cout << e.what() << endl;
  }  
}

//
void DileptonPlusMETEventAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) {


  try{

    //get the weight for the event
    float weight=1;
    if(!event.isRealData())
      {
	edm::Handle<float> puWeightHandle;
	event.getByLabel("puWeights","puWeight",puWeightHandle);
	if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());
      }

    //get objects for this event
    edm::Handle<std::vector<pat::EventHypothesis> > evHandle;
    event.getByLabel(edm::InputTag("cleanEvent:selectedEvent"),evHandle);
    if(!evHandle.isValid()) 
      {
	cout << "No valid event hypothesis" << endl;
	return;
      }
    const pat::EventHypothesis &evhyp = (*(evHandle.product()))[0];
    
    edm::Handle<std::vector<reco::Vertex> > vertexHandle;
    event.getByLabel(edm::InputTag("cleanEvent:selectedVertices"),vertexHandle);
    if(!vertexHandle.isValid()) 
      {
	cout << "No vertex selected" << endl;
	return;
      }

    //get other vertices
    std::vector<reco::VertexRef> selVertices;
    try{
      edm::Handle<reco::VertexCollection> hVtx;
      event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx);
      selVertices = vertex::filter(hVtx,objConfig_["Vertices"]);
    }catch(std::exception &e){
    }
        
    edm::Handle< std::vector<int> > selInfo;
    event.getByLabel(edm::InputTag("cleanEvent:selectionInfo"),selInfo);
    if(!selInfo.isValid()) 
      {
	cout << "No valid selection info" << endl;
	return;
      }
    int selPath = (*(selInfo.product()))[0];
    int selStep = (*(selInfo.product()))[1];

    edm::Handle< double > rho;
    event.getByLabel(edm::InputTag("kt6PFJets:rho"),rho);

    edm::Handle< std::vector<reco::PFCandidate> > pfCands;
    event.getByLabel(edm::InputTag("particleFlow"),pfCands);
    
    edm::Handle<std::vector<reco::PFMET> > hPfMET; 
    event.getByLabel(edm::InputTag("pfMet"), hPfMET);
    
    //require that a dilepton has been selected
    if(selPath==0 or selStep<3) return;
    std::string istream="mumu";
    if(selPath==2) istream="ee";
    if(selPath==3) istream="emu";
    getHist(istream+"_cutflow")->Fill(2,weight);

    //the met
    const pat::MET *themet=evhyp.getAs<pat::MET>("met"); 
    LorentzVector metP(themet->px(),themet->py(),0,themet->pt());

    //
    // VERTEX KINEMATICS
    //

    //vertex quantities
    const reco::Vertex *primVertex = &(*(vertexHandle.product()))[0];
    getHist(istream+"_ngoodvertex")->Fill(selVertices.size(),weight);
    getHist(istream+"_vertex_sumpt")->Fill(vertex::getVertexMomentumFlux(primVertex),weight);
    getHist(istream+"_vertex_pt")->Fill(primVertex->p4().pt(),weight);
    for(std::vector<reco::VertexRef>::iterator vit=selVertices.begin(); vit != selVertices.end(); vit++)
      {
	if(vit->get()->position()==primVertex->position()) continue;
	getHist(istream+"_othervertex_sumpt")->Fill(vertex::getVertexMomentumFlux(vit->get()),weight);
	getHist(istream+"_othervertex_pt")->Fill(vit->get()->p4().pt(),weight);
      }
    
    //MC truth on pileup (if available)
    if(!event.isRealData())
      {
	edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
	event.getByType(puInfoH);
	if(puInfoH.isValid())
	  {
	    int npuOOT(0),npuIT(0);
	    for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	      {
		if(it->getBunchCrossing()==0) npuIT += it->getPU_NumInteractions();
		else npuOOT += it->getPU_NumInteractions();
	      }
	    getHist(istream+"_ngenpileup")->Fill(npuIT,weight);
	    getHist(istream+"_ngenpileupOOT")->Fill(npuOOT,weight);
	    ((TH2 *)getHist(istream+"_ngoodvertex_ngenpileup"))->Fill(npuIT,selVertices.size(),weight);
	  }
      }

    //
    // LEPTON KINEMATICS
    //

    //basic dilepton kinematics
    reco::CandidatePtr lepton1 = evhyp["leg1"];
    LorentzVector lepton1P(lepton1->p4());
    int l1id=dilepton::getLeptonId(lepton1);
    double lepton1pterr=dilepton::getPtErrorFor(lepton1);

    reco::CandidatePtr lepton2 = evhyp["leg2"];
    LorentzVector lepton2P(lepton2->p4());
    int l2id=dilepton::getLeptonId(lepton2);
    double lepton2pterr=dilepton::getPtErrorFor(lepton2);

    LorentzVector dileptonP=lepton1P+lepton2P;
    double dphill=deltaPhi(lepton1P.phi(),lepton2P.phi());
    getHist(istream+"_dilepton_sumpt")->Fill(lepton1P.pt()+lepton2P.pt(),weight);
    getHist(istream+"_dilepton_pt")->Fill(dileptonP.pt(),weight);
    getHist(istream+"_dilepton_mass")->Fill(dileptonP.mass(),weight);
    getHist(istream+"_dilepton_dphi")->Fill( fabs(dphill) ,weight );

    //select Z window
    if(fabs(dileptonP.mass()-91)>15) return;
    getHist(istream+"_cutflow")->Fill(3,weight);

    //veto-other leptons
    int lepMult(2);
    for (pat::eventhypothesis::Looper<pat::Electron> ele = evhyp.loopAs<pat::Electron>("electron"); ele; ++ele) 
      {
	lepMult++;
	LorentzVector lepP(ele->px(),ele->py(),ele->pz(), ele->energy());
	float dphi=deltaPhi(metP.phi(),lepP.phi());
	float mTlm=TMath::Sqrt(2*metP.pt()*lepP.pt()*(1-TMath::Cos(dphi)));
	getHist(istream+"_mT_otherlep")->Fill(mTlm,weight);
      }
    for (pat::eventhypothesis::Looper<pat::Muon> mu = evhyp.loopAs<pat::Muon>("muon"); mu; ++mu) 
      {
	lepMult++;
	LorentzVector lepP(mu->px(),mu->py(),mu->pz(), mu->energy());
	float dphi=deltaPhi(metP.phi(),lepP.phi());
	float mTlm=TMath::Sqrt(2*metP.pt()*lepP.pt()*(1-TMath::Cos(dphi)));
	getHist(istream+"_mT_otherlep")->Fill(mTlm,weight);
      }
    getHist(istream+"_nleptons")->Fill(lepMult,weight);
    if(lepMult>2) return;
    getHist(istream+"_cutflow")->Fill(4,weight);


    //
    // JET KINEMATICS
    //
    //count the jets in the event
    std::vector<reco::CandidatePtr> seljets= evhyp.all("jet");
    int njets(0), nbjets(0);
    std::vector<LorentzVector> jetmomenta;
    LorentzVector rejectedJetMomenta(0,0,0,0);
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("jet"); jet; ++jet) 
      {

	//associate to the same primary vertex
	float fassoc(jet::fAssoc(jet.get(),primVertex));
	if(fassoc>0.1) jetmomenta.push_back(jet->p4());
	else 
	  {
	    rejectedJetMomenta += jet->p4();
	    continue;
	  }
      
	//b-tag
	float btag=jet->bDiscriminator("trackCountingHighEffBJetTags");
	if(btag>1.74) nbjets+=1; //loose point

	getHist(istream+"_btags")->Fill(btag,weight);
	getHist(istream+"_jetpt")->Fill(jet->pt(),weight);
	getHist(istream+"_jeteta")->Fill(jet->eta(),weight);
	getHist(istream+"_jetfassoc")->Fill( fassoc ,weight);
    }
    njets=jetmomenta.size();
    getHist(istream+"_njets")->Fill(njets,weight);
    getHist(istream+"_bmult")->Fill(nbjets,weight);
    
    //update the selection stream
    int jetbin=njets;
    if(jetbin>2) jetbin=2;
    TString substream=istream; 
    if(jetbin==0)      substream = substream+"eq0jets";
    else if(jetbin==1) substream = substream+"eq1jets";
    else               substream = substream+"geq2jets";
    getHist(istream+"_cutflow")->Fill(5+jetbin,weight);
    
    
    //
    // MET kinematics
    //

    //MC truth on MET
    LorentzVector genMET(0,0,0,0);
    if(!event.isRealData())
      {     
	int igenpart(0);
	for (pat::eventhypothesis::Looper<reco::GenParticle> genpart = evhyp.loopAs<reco::GenParticle>("genparticle"); genpart; ++genpart) 
	  {
	    //higgs level (H)
	    //cout << "\t" << genpart->pdgId() << " -> " << flush;  
	    
	    int igenpartdau(0);
	    char buf[20];
	    sprintf(buf,"gendaughter_%d",igenpart);
	    for(pat::eventhypothesis::Looper<reco::GenParticle> genpartdau = evhyp.loopAs<reco::GenParticle>(buf); genpartdau; ++genpartdau)
	      {
		//higgs daughters level (ZZ/WW)
		//cout << genpartdau->pdgId() << " (" << flush;
		
		char buf[20];
		sprintf(buf,"gendaughter_%d_%d",igenpart,igenpartdau);
		for(pat::eventhypothesis::Looper<reco::GenParticle> genpartgdau = evhyp.loopAs<reco::GenParticle>(buf); genpartgdau; ++genpartgdau)
		  {
		    //final state products (leptons, jets, neutrinos)
		    //cout << genpartgdau->pdgId() << " " << flush;

		    int pdgid=fabs(genpartgdau->pdgId());
		    if(pdgid==12||pdgid==14||pdgid==16) genMET += genpartgdau->p4();
		  }
		
		//cout << ") " << flush;
		igenpartdau++;
	      }
	    igenpart++;
	  }
	//cout << endl;
      }
    
    //standard MET significance
    float metsig( hPfMET.isValid()? ((*hPfMET)[0]).significance() : -1);

    //rejected jet corrected met
    LorentzVector thecormet = themet->p4()+rejectedJetMomenta;
    
    //reduced met
    rmet_.compute(lepton1->p4(),lepton1pterr, lepton2->p4(),lepton2pterr, jetmomenta, themet->p4() );
    float reducedMET=rmet_.reducedMET();
    LorentzVector rmetP(rmet_.reducedMETComponents().first,rmet_.reducedMETComponents().second,0,reducedMET);
    std::pair<double, double> rmet_dil = rmet_.dileptonProjComponents();
    std::pair<double, double> rmet_delta = rmet_.dileptonPtCorrComponents();
    std::pair<double, double> rmet_rjet = rmet_.sumJetProjComponents();
    std::pair<double, double> rmet_met = rmet_.metProjComponents();
    std::pair<double, double> rmet_r = rmet_.recoilProjComponents();

    //projected MET
    float dphil2met[]={ fabs(deltaPhi(metP.phi(),lepton1P.phi())), fabs(deltaPhi(metP.phi(),lepton2P.phi())) };
    float dphimin=TMath::MinElement(sizeof(dphil2met)/sizeof(float),dphil2met);
    float relMET=metP.pt();
    if(fabs(dphimin)<TMath::Pi()/2) relMET = relMET*TMath::Sin(dphimin);

    //charged candidates based met 
    //(use only charged candidates associated to the primary vertex and neutral candidates with pT>5 GeV)
    LorentzVector tmet(0,0,0,0);
    if(pfCands.isValid())
      {
	LorentzVector neutralSum(0,0,0,0), chSum(0,0,0,0);
	for(std::vector<reco::PFCandidate>::const_iterator it = pfCands->begin(); it != pfCands->end(); it++)
	  {
	    LorentzVector candP(it->p4());
	    if(it->charge()==0 && candP.pt()>5)                            neutralSum -= candP;
	    else if(it->charge() && it->vertex()==primVertex->position() ) chSum -= candP;
	  }
	tmet = chSum + neutralSum;
      }

    //correlation with vertex
    LorentzVector vrtxP(primVertex->p4());
    float dphimet2vertex=deltaPhi(metP.phi(),vrtxP.phi());
    
    //correlation with dilepton
    float dphimet2zll = deltaPhi(metP.phi(),dileptonP.phi());
    
    //transverse masses
    float mTlmet[]={ TMath::Sqrt(2*metP.pt()*lepton1P.pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*metP.pt()*lepton2P.pt()*(1-TMath::Cos(dphil2met[1]))) };
    LorentzVector transvSum=dileptonP + metP;
    float transverseMass=TMath::Power(TMath::Sqrt(TMath::Power(dileptonP.pt(),2)+pow(dileptonP.mass(),2))+TMath::Sqrt(TMath::Power(metP.pt(),2)+pow(dileptonP.mass(),2)),2);
    transverseMass-=TMath::Power(transvSum.pt(),2);
    transverseMass=TMath::Sqrt(transverseMass);

    //final control histograms (inclusive and per jet bin)
    TString catsToFill[2]={istream,substream};
    for(size_t icat=0; icat<2; icat++)
      {
	getHist(catsToFill[icat]+"_dilepton_mass")->Fill(dileptonP.mass(),weight);
	getHist(catsToFill[icat]+"_dilepton_dphi")->Fill( fabs( deltaPhi(lepton1P.phi(),lepton2P.phi()) ) ,weight);
	getHist(catsToFill[icat]+"_dilepton_sumpt")->Fill(lepton1P.pt()+lepton2P.pt(),weight);
	getHist(catsToFill[icat]+"_dilepton_pt")->Fill(dileptonP.pt(),weight);
	
	getHist(catsToFill[icat]+"_met")->Fill(metP.pt(),weight);
	getHist(catsToFill[icat]+"_metresol")->Fill(metP.pt()-genMET.pt(),weight);
	getHist(catsToFill[icat]+"_metsig")->Fill(metsig,weight);
	getHist(catsToFill[icat]+"_cormet")->Fill( thecormet.pt() );
	getHist(catsToFill[icat]+"_rmet")->Fill(reducedMET,weight);
	getHist(catsToFill[icat]+"_relmet")->Fill(relMET,weight);
	getHist(catsToFill[icat]+"_tmet")->Fill(tmet.pt());
	getHist(catsToFill[icat]+"_met2vertex_dphi")->Fill(fabs(dphimet2vertex),weight);
	getHist(catsToFill[icat]+"_met2dilepton_dphi")->Fill(fabs(dphimet2zll),weight);

	getHist(catsToFill[icat]+"_mT_individualsum")->Fill(mTlmet[0]+mTlmet[1],weight);
	if(lepton1P.pt()>lepton2P.pt()) ((TH2 *)getHist(catsToFill[icat]+"_mT_individualcorr"))->Fill(mTlmet[0],mTlmet[1],weight);
	else ((TH2 *)getHist(catsToFill[icat]+"_mT_individualcorr"))->Fill(mTlmet[1],mTlmet[0],weight);
	
	getHist(catsToFill[icat]+"_mT")->Fill(transverseMass,weight);
      }


    //save summary
    ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();
    ev.run=event.id().run();
    ev.lumi=event.luminosityBlock();
    ev.event=event.id().event();
    ev.cat=selPath;
    ev.njets=njets;
    ev.nbtags=nbjets;
    ev.weight=weight;
    ev.pfmet_pt=metP.pt();                ev.pfmet_phi=metP.phi();           ev.pfmet_sig=metsig;
    ev.cormet_pt=thecormet.pt();          ev.cormet_phi=thecormet.phi();
    ev.redmet_pt=rmetP.pt();              ev.redmet_phi=rmetP.phi();
    ev.redmet_dilL = rmet_dil.first;      ev.redmet_dilT = rmet_dil.second;
    ev.redmet_deltaL = rmet_delta.first;  ev.redmet_deltaT = rmet_delta.second;
    ev.redmet_rjetL = rmet_rjet.first;    ev.redmet_rjetT = rmet_rjet.second;
    ev.redmet_metL = rmet_met.first;      ev.redmet_metT = rmet_met.second;
    ev.redmet_rL = rmet_r.first;          ev.redmet_rT = rmet_r.second;
    ev.projmet_pt=relMET;
    ev.tkmet_pt=tmet.pt();       ev.tkmet_phi=tmet.phi();
    ev.genmet_pt=genMET.pt();    ev.genmet_phi=genMET.phi();
    ev.l1id = l1id;              ev.l1px=lepton1P.px();       ev.l1py=lepton1P.py();  ev.l1pz=lepton1P.pz();  ev.l1e=lepton1P.energy();
    ev.l2id = l2id;              ev.l2px=lepton2P.px();       ev.l2py=lepton2P.py();  ev.l2pz=lepton2P.pz();  ev.l2e=lepton2P.energy();
    summaryHandler_.fillTree();

  }catch(std::exception &e){
    std::cout << "[CleanEventAnalysis][analyze] failed with " << e.what() << std::endl;
  }

}

//
void DileptonPlusMETEventAnalyzer::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  //  cout << "[DileptonPlusMETEventAnalyzer::endLuminosityBlock]" << endl;
  TString streams[]={"ee","mumu","emu"};
  edm::Handle<edm::MergeableCounter> ctrHandle;
  iLumi.getByLabel("startCounter", ctrHandle);
  for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(0.,ctrHandle->value);
      edm::Handle<edm::MergeableCounter> streamCtrHandle;
      std::string inpt= std::string(streams[istream])+"Counter";
      iLumi.getByLabel(inpt, streamCtrHandle);
      ((TH1F *)getHist(streams[istream]+"_cutflow"))->Fill(1.,streamCtrHandle->value);
    } 
}


DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);


 
