////////////////////////////////////////////////////////////////////////
// Class:       ProtoDUNEelectronWireAna
// Plugin Type: analyzer (Unknown Unknown)
// File:        ProtoDUNEelectronWireAna_module.cc
//
// Generated at Fri Oct 21 18:51:17 2022 by Ewerton Batista das Chagas using cetskelgen
// from  version .
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art_root_io/TFileService.h"

#include "lardataobj/RecoBase/Wire.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/raw.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "larcore/Geometry/Geometry.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "larreco/Calorimetry/CalorimetryAlg.h" 
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "canvas/Persistency/Common/FindManyP.h"

#include "duneprototypes/Protodune/singlephase/DataUtils/ProtoDUNEDataUtils.h"
#include "protoduneana/Utilities/ProtoDUNEPFParticleUtils.h"
#include "protoduneana/Utilities/ProtoDUNEShowerUtils.h"
#include "protoduneana/Utilities/ProtoDUNETruthUtils.h"
#include "protoduneana/Utilities/ProtoDUNETrackUtils.h"
#include "larsim/MCCheater/BackTrackerService.h"
#include "larsim/MCCheater/ParticleInventoryService.h"
#include "dunecalib/Calib/LifetimeCalib.h" 
#include "dunecalib/CalibServices/LifetimeCalibService.h" 
#include "protoduneana/Utilities/ProtoDUNEBeamlineUtils.h"
#include "larsim/MCCheater/BackTrackerService.h"
#include "larsim/MCCheater/ParticleInventoryService.h"
//#include "nusimdata/SimulationBase/MCParticle.h"
#include "lardataobj/RecoBase/TrackHitMeta.h"
#include "larevt/CalibrationDBI/Interface/ChannelStatusService.h"
#include "larevt/CalibrationDBI/Interface/ChannelStatusProvider.h"

#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TTree.h"
#include "TH2F.h"
#include "TFile.h"
#include <stdio.h>
#include <stdlib.h> 
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

#define PI 3.14159265

const int kMaxShowerHits = 30000;
const int kMaxWires = 30000;
const int kMaxTracks = 10000;
//const int kMaxTrackHits = 5000;

const int MinChannel=2080;
const int MaxChannel=4000;//2560;

namespace test {
  class ProtoDUNEelectronWireAna;
}


class test::ProtoDUNEelectronWireAna : public art::EDAnalyzer {
public:
  explicit ProtoDUNEelectronWireAna(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  ProtoDUNEelectronWireAna(ProtoDUNEelectronWireAna const&) = delete;
  ProtoDUNEelectronWireAna(ProtoDUNEelectronWireAna&&) = delete;
  ProtoDUNEelectronWireAna& operator=(ProtoDUNEelectronWireAna const&) = delete;
  ProtoDUNEelectronWireAna& operator=(ProtoDUNEelectronWireAna&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;
  void beginJob() override;
  void beginRun(const art::Run&);
  void endJob() override;
  
private:

  //analyzer;
  // Declare member data here.
  
  void Initialize();  
  typedef int channel_t;
  typedef size_t tick_t;
  
  void   FillShowerCharge(art::Event const & evt, 
                  std::map<int,std::vector<double>> & hit_w_and_t1, 
                  std::map<int,std::vector<double>> & hit_w_and_t2, 
                  std::map<int,std::vector<double>> & hit_w_and_x, 
                  std::map<int,std::vector<double>> & hit_w_and_y,
                  std::map<channel_t,std::map<tick_t,double>> & map_reco,
                  double & ymin,
                  double & ymax );  
  
  void   FillShowerCharge_fullSignal(art::Event const & evt//, 
                  //std::map<int,std::vector<double>> & hit_w_and_t1, 
                  //std::map<int,std::vector<double>> & hit_w_and_t2, 
                  //std::map<int,std::vector<double>> & hit_w_and_x, 
                  //std::map<int,std::vector<double>> & hit_w_and_y,
                  //std::map<channel_t,std::map<tick_t,double>> & map_reco,
                  //double & ymin,
                  //double & ymax 
                                  );
  
  void GetTrueElectronEnergy(art::Event const & evt, const simb::MCParticle* mcparticle, const recob::PFParticle* particle);  
  void GetTrueShowerEnergyNew(art::Event const & evt);
      
//  void getDecayTrackIds(const simb::MCParticle* mcparticle, std::vector<int> & trkids, art::ServiceHandle<cheat::ParticleInventoryService> pi_serv);
  
  void GetShowerParticlesXYZE(art::Event const & evt, const simb::MCParticle* particle, const simb::MCParticle* geantGoodParticle);
  void GetShowerParticlesTrueEnergyOutsideTPC(art::Event const & evt, const simb::MCParticle* particle, const simb::MCParticle* geantGoodParticle);
  
  double BuildTDCEnergySimMap(art::Event const & evt, std::map<channel_t,std::map<tick_t,double>> map_reco) const;  
  
  void FillPrimaryPFParticle(art::Event const & evt, const recob::PFParticle* particle);

  // Declare member data here.
  protoana::ProtoDUNEDataUtils dataUtil;
  protoana::ProtoDUNEPFParticleUtils pfpUtil;
  protoana::ProtoDUNEShowerUtils showerUtil;
  protoana::ProtoDUNETruthUtils truthUtil;
  protoana::ProtoDUNEBeamlineUtils beamlineUtil;
  protoana::ProtoDUNETrackUtils trackUtil;
 
  geo::GeometryCore const * fGeometry = &*(art::ServiceHandle<geo::Geometry>());
  const lariov::ChannelStatusProvider* m_pChannelStatusProvider;

  const art::InputTag fBeamModuleLabel;
  std::string fTrackerTag;
  std::string fShowerTag;
  std::string fShowerCaloTag;
  std::string fCaloTag;  
  std::string fPFParticleTag;
  std::string fGeneratorTag;
  std::string fWireTag;
  std::string fSimChannelTag;
  std::string fRawDigitTag;
  std::string fHitTag;
  calo::CalorimetryAlg fCalorimetryAlg;
  int fVerbose;

  TTree *fTree;

  // Tree variables
  int fRun;
//  int fSubRun;
  int fevent;
  
  TH1D *missenergy_per_tick;
  
  
  // Beam track
  float fbeamtrackMomentum; 
  int fcerenkovStatus[2];
  float fbeamtrackDir[3];

  // Reconstructed tracks/showers
  int fprimaryIstrack;
  int fprimaryIsshower;
  int fprimaryNHits;
  float fprimaryStartDirection[3];
  int   fprimaryShower_nHits; //collection only
//  int   fprimaryShower_hit_w[kMaxShowerHits];
  float fprimaryShower_hit_q[kMaxShowerHits];
  float fprimaryShower_hit_t[kMaxShowerHits]; 
  float fprimaryShower_hit_X[kMaxShowerHits];
  float fprimaryShower_hit_Y[kMaxShowerHits];
  float fprimaryShower_hit_Z[kMaxShowerHits];  
  int   fprimaryShower_hit_ch[kMaxShowerHits];


  float fLifetime; //us

  Int_t fTrack_index= 0;
  Int_t fShower_index= 0;

  float missenergy_evt; // missing energy in the event

  float fmcparticle_E; // MC particle E 
  float fmcparticle_P; // MC particle P   
  float fmcparticle_EndE; // MC particle EndE
  float fmcparticle_EndP; // MC particle EndP  
  
  float ftotal_energy_photons;
  float ftotal_energy_electrons;  
  float ftotal_energy_photons_outside_TPC_annihil;
  float fftotal_energy_photons_outside_TPC_eBrem;  
  float ftotal_energy_electrons_outside_TPC;
  
  int fprimary_Shower_nWires; //collection only
  int fprimary_Shower_nWires_full; //collection only  
 
  float fprimary_Shower_sim_energy_new_tpc1;  
  float fprimary_Shower_sim_energy_new1_tpc1;
  float fprimary_Shower_sim_energy_new2_tpc1;
  float fprimary_Shower_sim_energy_new3_tpc1;  
  float fprimary_Shower_sim_energy_new_othertpc;  
  float fprimary_Shower_sim_energy_new1_othertpc;
  float fprimary_Shower_sim_energy_new2_othertpc;
  float fprimary_Shower_sim_energy_new3_othertpc;  
  
  float fprimary_Shower_sim_electrons_true0;  
  float fprimary_Shower_sim_electrons_true1;  
  int fprimary_Shower_wire_w[kMaxWires];
  float fprimary_Shower_wire_ch[kMaxWires]; //charge corrected by lifetime
  float fprimary_Shower_wire_X[kMaxWires];  
  float fprimary_Shower_wire_Y[kMaxWires];  
  float fprimary_Shower_wire_Z[kMaxWires];  
  float fprimary_Shower_wire_energy;    
  
  int fprimary_Shower_wire_w_full[kMaxWires];
  float fprimary_Shower_wire_ch_full[kMaxWires]; //charge corrected by lifetime
  float fprimary_Shower_wire_X_full[kMaxWires];  
  float fprimary_Shower_wire_Y_full[kMaxWires];  
  float fprimary_Shower_wire_Z_full[kMaxWires];  
  float fprimary_Shower_wire_energy_full;    
  
  float fcosmic_bkg_energy;
  float fcosmic_bkg_energy_1sigma;
  float fcosmic_bkg_energy_3sigma;
  float fcosmic_bkg_energy_5sigma;
  float fcosmic_bkg_energy_outsideYregion;
  float fcosmic_bkg_energy_1sigma_outsideYregion;
  float fcosmic_bkg_energy_3sigma_outsideYregion;
  float fcosmic_bkg_energy_5sigma_outsideYregion;

//  std::vector<double>  fprimary_Shower_MCwire_E;  //energy in MeV
//  std::vector<int>  fprimary_Shower_MCwire_w;
  float fprimaryStartPosition[3];
  
//  std::vector<std::pair<double,double>> fdqdx_resrange;

  int fcosmicNTracks;  
  int fcosmicNTracks_PF;
  int fcosmicNTracks_PF_length;
  int fcosmicNTracks_PF_length_crossTPC;
  int fcosmicNTracks_PF_length_crossTPC_diverters;
  int fcosmicNTracks_PF_length_crossTPC_diverters_notbroken;
  int fcosmicNTracks_PF_length_crossTPC_diverters_notbroken_angle;
  int fcosmicNTracks_PF_length_crossTPC_diverters_notbroken_angle_earlyTrack;  
//  int fcosmicTrack_nHits[kMaxTracks];
//  int fcosmicTrack_nPFs[kMaxTracks];
//  int fcosmicTrack_nT0s[kMaxTracks];
  float fcosmicTrack_theta_xz[kMaxTracks];    
  float fcosmicTrack_theta_yz[kMaxTracks];    
  float fcosmicTrack_startx[kMaxTracks];
  float fcosmicTrack_starty[kMaxTracks];
  float fcosmicTrack_startz[kMaxTracks];   
  float fcosmicTrack_endx[kMaxTracks];    
  float fcosmicTrack_endy[kMaxTracks]; 
  float fcosmicTrack_endz[kMaxTracks];
  float fcosmicTrack_length[kMaxTracks];
//  float fcosmicTrack_minHitPeakTime[kMaxTracks];
//  float fcosmicTrack_maxHitPeakTime[kMaxTracks];
    
  TH1D *fdqdx;
  TH1D *fdqdx_after;  
  TH2D *fdqdx_resrange;   
  TH2D *fdqdx_resrange_after;  
  TH2D *fposX_dqdx; 
  TH2D *fnegX_dqdx;  
  
  TH1D *h_photon_x;  
  TH1D *h_photon_endx;
  TH1D *h_photon_y;  
  TH1D *h_photon_endy;
  TH1D *h_photon_z;  
  TH1D *h_photon_endz;  
  TH1D *h_electron_x;  
  TH1D *h_electron_endx;
  TH1D *h_electron_y;  
  TH1D *h_electron_endy;
  TH1D *h_electron_z;  
  TH1D *h_electron_endz;   
  
//  TH1D *deconwf[MaxChannel];
  
//  double Rho = 1.383;//g/cm^3 (liquid argon density at a pressure 18.0 psia) 
//  double betap = 0.212;//(kV/cm)(g/cm^2)/MeV
//  double alpha = 0.93;//parameter from ArgoNeuT experiment at 0.481kV/cm 
  double Wion = 23.6e-6;//parameter from ArgoNeuT experiment at 0.481kV/cm
  double normalisation_factor=0;
  double calib_factor = 0;
  double recombination =0.6417;
//  TFile *fyz_corr=NULL;
//  TFile *fx_corr=NULL;
//  TH2F *YZ_neg=NULL;
//  TH2F *YZ_pos=NULL; 
//  TH2F *X_correction=NULL;  

};

void ProtoDUNEelectronWireAna::beginJob()
{

  art::ServiceHandle<art::TFileService> tfs;
  fTree = tfs->make<TTree>("WireAna","Recob::Wire");
  fTree->Branch("run", &fRun,"fRun/I");
//  fTree->Branch("subrun", &fSubRun,"subrun/I");
  fTree->Branch("event", &fevent,"fevent/I");

  fTree->Branch("cerenkovStatus", &fcerenkovStatus, "fcerenkovStatus[2]/I");
  fTree->Branch("beamtrackMomentum", &fbeamtrackMomentum, "fbeamtrackMomentum/F");
  fTree->Branch("beamtrackDir", &fbeamtrackDir, "fbeamtrackDir[3]/F");

  fTree->Branch("primaryIstrack", &fprimaryIstrack, "fprimaryIstrack/I");
  fTree->Branch("primaryIsshower", &fprimaryIsshower, "fprimaryIsshower/I");
  fTree->Branch("primaryNHits", &fprimaryNHits, "fprimaryNHits/I");
//  fTree->Branch("primaryStartPosition",  &fprimaryStartPosition);
  fTree->Branch("primaryStartDirection", &fprimaryStartDirection, "fprimaryStartDirection[3]/F");
/*
  fTree->Branch("primaryShower_nHits", &fprimaryShower_nHits, "fprimaryShower_nHits/I");
  fTree->Branch("primaryShower_hit_q", &fprimaryShower_hit_q, "fprimaryShower_hit_q[fprimaryShower_nHits]/F");
//  fTree->Branch("primaryShower_hit_w", &fprimaryShower_hit_w);
  fTree->Branch("primaryShower_hit_t", &fprimaryShower_hit_t, "fprimaryShower_hit_t[fprimaryShower_nHits]/F");
  fTree->Branch("primaryShower_hit_X", &fprimaryShower_hit_X, "fprimaryShower_hit_X[fprimaryShower_nHits]/F");
  fTree->Branch("primaryShower_hit_Y", &fprimaryShower_hit_Y, "fprimaryShower_hit_Y[fprimaryShower_nHits]/F");
  fTree->Branch("primaryShower_hit_Z", &fprimaryShower_hit_Z, "fprimaryShower_hit_Z[fprimaryShower_nHits]/F");
  fTree->Branch("primaryShower_hit_ch", &fprimaryShower_hit_ch, "fprimaryShower_hit_ch[fprimaryShower_nHits]/I");
*/
  fTree->Branch("lifetime", &fLifetime, "fLifetime/F");
  
  fTree->Branch("Track_index",  &fTrack_index);
  fTree->Branch("Shower_index", &fShower_index);  
  
  
  fTree->Branch("mcparticle_E", &fmcparticle_E, "fmcparticle_E/F");  
  fTree->Branch("mcparticle_P", &fmcparticle_P, "fmcparticle_P/F");  
  fTree->Branch("mcparticle_EndE", &fmcparticle_EndE, "fmcparticle_EndE/F");
  fTree->Branch("mcparticle_EndP", &fmcparticle_EndP, "fmcparticle_EndP/F");  
  fTree->Branch("total_energy_photons", &ftotal_energy_photons, "ftotal_energy_photons/F");  
  fTree->Branch("total_energy_electrons", &ftotal_energy_electrons, "ftotal_energy_electrons/F");   
  fTree->Branch("total_energy_photons_outside_TPC_annihil", &ftotal_energy_photons_outside_TPC_annihil, "ftotal_energy_photons_outside_TPC_annihil/F");  
  fTree->Branch("ftotal_energy_photons_outside_TPC_eBrem", &fftotal_energy_photons_outside_TPC_eBrem, "fftotal_energy_photons_outside_TPC_eBrem/F");   
  fTree->Branch("total_energy_electrons_outside_TPC", &ftotal_energy_electrons_outside_TPC, "ftotal_energy_electrons_outside_TPC/F");  
  
  fTree->Branch("primary_Shower_nWires", &fprimary_Shower_nWires, "fprimary_Shower_nWires/I");  
  fTree->Branch("primary_Shower_nWires_full", &fprimary_Shower_nWires_full, "fprimary_Shower_nWires_full/I");   
  fTree->Branch("primary_Shower_sim_energy_new_tpc1", &fprimary_Shower_sim_energy_new_tpc1, "fprimary_Shower_sim_energy_new_tpc1/F"); 
  fTree->Branch("primary_Shower_sim_energy_new1_tpc1", &fprimary_Shower_sim_energy_new1_tpc1, "fprimary_Shower_sim_energy_new1_tpc1/F");   
  fTree->Branch("primary_Shower_sim_energy_new2_tpc1", &fprimary_Shower_sim_energy_new2_tpc1, "fprimary_Shower_sim_energy_new2_tpc1/F");
  fTree->Branch("primary_Shower_sim_energy_new3_tpc1", &fprimary_Shower_sim_energy_new3_tpc1, "fprimary_Shower_sim_energy_new3_tpc1/F");  
  fTree->Branch("primary_Shower_sim_energy_new_othertpc", &fprimary_Shower_sim_energy_new_othertpc, "fprimary_Shower_sim_energy_new_othertpc/F"); 
  fTree->Branch("primary_Shower_sim_energy_new1_othertpc", &fprimary_Shower_sim_energy_new1_othertpc, "fprimary_Shower_sim_energy_new1_othertpc/F");   
  fTree->Branch("primary_Shower_sim_energy_new2_othertpc", &fprimary_Shower_sim_energy_new2_othertpc, "fprimary_Shower_sim_energy_new2_othertpc/F");
  fTree->Branch("primary_Shower_sim_energy_new3_othertpc", &fprimary_Shower_sim_energy_new3_othertpc, "fprimary_Shower_sim_energy_new3_othertpc/F");    
  
  fTree->Branch("primary_Shower_sim_electrons_true0", &fprimary_Shower_sim_electrons_true0, "fprimary_Shower_sim_electrons_true0/F");  
  fTree->Branch("primary_Shower_sim_electrons_true1", &fprimary_Shower_sim_electrons_true1, "fprimary_Shower_sim_electrons_true1/F"); 
  
  fTree->Branch("primary_Shower_wire_w", &fprimary_Shower_wire_w, "fprimary_Shower_wire_w[fprimary_Shower_nWires]/I");
  fTree->Branch("primary_Shower_wire_ch", &fprimary_Shower_wire_ch, "fprimary_Shower_wire_ch[fprimary_Shower_nWires]/F");
  fTree->Branch("primary_Shower_wire_X", &fprimary_Shower_wire_X, "fprimary_Shower_wire_X[fprimary_Shower_nWires]/F");
  fTree->Branch("primary_Shower_wire_Y", &fprimary_Shower_wire_Y, "fprimary_Shower_wire_Y[fprimary_Shower_nWires]/F");
  fTree->Branch("primary_Shower_wire_Z", &fprimary_Shower_wire_Z, "fprimary_Shower_wire_Z[fprimary_Shower_nWires]/F");  
  
  fTree->Branch("primary_Shower_wire_w_full", &fprimary_Shower_wire_w_full, "fprimary_Shower_wire_w_full[fprimary_Shower_nWires_full]/I");
  fTree->Branch("primary_Shower_wire_ch_full", &fprimary_Shower_wire_ch_full, "fprimary_Shower_wire_ch_full[fprimary_Shower_nWires_full]/F");
  fTree->Branch("primary_Shower_wire_X_full", &fprimary_Shower_wire_X_full, "fprimary_Shower_wire_X_full[fprimary_Shower_nWires_full]/F");
  fTree->Branch("primary_Shower_wire_Y_full", &fprimary_Shower_wire_Y_full, "fprimary_Shower_wire_Y_full[fprimary_Shower_nWires_full]/F");
  fTree->Branch("primary_Shower_wire_Z_full", &fprimary_Shower_wire_Z_full, "fprimary_Shower_wire_Z_full[fprimary_Shower_nWires_full]/F");    

  fTree->Branch("primary_Shower_wire_energy", &fprimary_Shower_wire_energy, "fprimary_Shower_wire_energy/F");   
  fTree->Branch("primary_Shower_wire_energy_full", &fprimary_Shower_wire_energy_full, "fprimary_Shower_wire_energy_full/F");
  
  fTree->Branch("missenergy_evt", &missenergy_evt, "missenergy_evt/F");  
/*  
  fTree->Branch("cosmic_bkg_energy", &fcosmic_bkg_energy);
  fTree->Branch("cosmic_bkg_energy_1sigma", &fcosmic_bkg_energy_1sigma);
  fTree->Branch("cosmic_bkg_energy_3sigma", &fcosmic_bkg_energy_3sigma);
  fTree->Branch("cosmic_bkg_energy_5sigma", &fcosmic_bkg_energy_5sigma);
  fTree->Branch("cosmic_bkg_energy_outsideYregion", &fcosmic_bkg_energy_outsideYregion);
  fTree->Branch("cosmic_bkg_energy_1sigma_outsideYregion", &fcosmic_bkg_energy_1sigma_outsideYregion);
  fTree->Branch("cosmic_bkg_energy_3sigma_outsideYregion", &fcosmic_bkg_energy_3sigma_outsideYregion);
  fTree->Branch("cosmic_bkg_energy_5sigma_outsideYregion", &fcosmic_bkg_energy_5sigma_outsideYregion);
  fTree->Branch("cosmicNTracks", &fcosmicNTracks, "fcosmicNTracks/I");
  fTree->Branch("cosmicTrack_theta_xz",  &fcosmicTrack_theta_xz, "fcosmicTrack_theta_xz[fcosmicNTracks]/F"); 
  fTree->Branch("cosmicTrack_theta_yz",  &fcosmicTrack_theta_yz, "fcosmicTrack_theta_yz[fcosmicNTracks]/F");
  fTree->Branch("cosmicTrack_startx",  &fcosmicTrack_startx, "fcosmicTrack_startx[fcosmicNTracks]/F");
  fTree->Branch("cosmicTrack_starty",  &fcosmicTrack_starty, "fcosmicTrack_starty[fcosmicNTracks]/F");
  fTree->Branch("cosmicTrack_startz",  &fcosmicTrack_startz, "fcosmicTrack_startz[fcosmicNTracks]/F");
  fTree->Branch("cosmicTrack_endx",  &fcosmicTrack_endx, "fcosmicTrack_endx[fcosmicNTracks]/F");
  fTree->Branch("cosmicTrack_endy",  &fcosmicTrack_endy, "fcosmicTrack_endy[fcosmicNTracks]/F");
  fTree->Branch("cosmicTrack_endz",  &fcosmicTrack_endz, "fcosmicTrack_endz[fcosmicNTracks]/F");
  fTree->Branch("cosmicTrack_length",  &fcosmicTrack_length, "fcosmicTrack_length[fcosmicNTracks]/F"); 
*/  
  missenergy_per_tick = tfs->make<TH1D>("missenergy_per_tick","",1000,0,10); 
  
  fdqdx = tfs->make<TH1D>("fdqdx","dQ/dx;Entries;dQ/dx (ADC/cm)",100,0,1000); 
  fdqdx_after = tfs->make<TH1D>("fdqdx_after","dQ/dx;Entries;dQ/dx (ADC/cm)",100,0,1000); 
  fdqdx_resrange = tfs->make<TH2D>("fdqdx_resrange","dQ/dx vs residual range;residual range (cm);dQ/dx (ADC/cm)",200,0,200,1000,0,1000); 
  fdqdx_resrange_after = tfs->make<TH2D>("fdqdx_resrange_after","dQ/dx vs residual range;residual range (cm);dQ/dx (ADC/cm)",200,0,200,1000,0,1000);   
  fposX_dqdx = tfs->make<TH2D>("fposX_dqdx","dQ/dx as a function of theta_xz vs theta_yz (x>0);theta xz(deg);theta yz(deg)",80,-200,200,80,-200,200); 
  fnegX_dqdx = tfs->make<TH2D>("fnegX_dqdx","dQ/dx as a function of theta_xz vs theta_yz (x<0);theta xz(deg);theta yz(deg)",80,-200,200,80,-200,200);
  
  h_photon_x = tfs->make<TH1D>("h_photon_x","All photons;Vx [cm];Entries",800,-400,400); 
  h_photon_endx = tfs->make<TH1D>("h_photon_endx","All photons;End x [cm];Entries",800,-400,400);
  h_photon_y = tfs->make<TH1D>("h_photon_y","All photons;Vy [cm];Entries",800,0,800);
  h_photon_endy = tfs->make<TH1D>("h_photon_endy","All photons;End y [cm];Entries",800,0,800);
  h_photon_z = tfs->make<TH1D>("h_photon_z","All photons;Vz [cm];Entries",800,0,800);
  h_photon_endz = tfs->make<TH1D>("h_photon_endz","All photons;End z [cm];Entries",800,0,800); 
  
  h_electron_x = tfs->make<TH1D>("h_electron_x","All electrons;Vx [cm];Entries",800,-400,400); 
  h_electron_endx = tfs->make<TH1D>("h_electron_endx","All electrons;End x [cm];Entries",800,-400,400);
  h_electron_y = tfs->make<TH1D>("h_electron_y","All electrons;Vy [cm];Entries",800,0,800);
  h_electron_endy = tfs->make<TH1D>("h_electron_endy","All electrons;End y [cm];Entries",800,0,800);
  h_electron_z = tfs->make<TH1D>("h_electron_z","All electrons;Vz [cm];Entries",800,0,800);
  h_electron_endz = tfs->make<TH1D>("h_electron_endz","All electrons;End z [cm];Entries",800,0,800);  


//  fTree->Branch("primary_Shower_MCwire_w",                        &fprimary_Shower_MCwire_w);
//  fTree->Branch("primary_Shower_MCwire_E",                        &fprimary_Shower_MCwire_E); 
  
  
/*  
  for (int j = MinChannel; j<=MaxChannel; ++j){
    deconwf[j] = tfs->make<TH1D>(Form("deconwf_%d_%d",2,j),Form("Plane %d Channel %d",2,j),6000,0,6000);
  }
*/
}


void ProtoDUNEelectronWireAna::beginRun(const art::Run& run)
{

}


void ProtoDUNEelectronWireAna::endJob()
{

}

test::ProtoDUNEelectronWireAna::ProtoDUNEelectronWireAna(fhicl::ParameterSet const& p)
  : EDAnalyzer{p}  // ,
  // More initializers here.
  dataUtil(p.get<fhicl::ParameterSet>("DataUtils")),
  beamlineUtil(p.get<fhicl::ParameterSet>("BeamLineUtils")),
  fBeamModuleLabel(p.get< art::InputTag >("BeamModuleLabel")),
  fTrackerTag(p.get<std::string>("TrackerTag")),
  fShowerTag(p.get<std::string>("ShowerTag")),
  fShowerCaloTag(p.get<std::string>("ShowerCalorimetryTag")),
  fCaloTag(p.get<std::string>("CalorimetryTag")),
  fPFParticleTag(p.get<std::string>("PFParticleTag")),
  fGeneratorTag(p.get<std::string>("GeneratorTag")),
  fWireTag(p.get<std::string>("WireTag")),
  fSimChannelTag(p.get<std::string>("SimChannelTag")),
  fRawDigitTag(p.get<std::string>("RawDigitTag")),
  fHitTag(p.get<std::string>("HitTag")),
  fCalorimetryAlg(p.get<fhicl::ParameterSet>("CalorimetryAlg")),
  fVerbose(p.get<int>("Verbose"))  
{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}

void test::ProtoDUNEelectronWireAna::analyze(art::Event const& e)
{
  // Implementation of required member function here.

  Initialize(); 
  fRun = evt.run();
//  fSubRun = evt.subRun();
  fevent  = evt.id().event(); 
     
  //auto const clockData = art::ServiceHandle<detinfo::DetectorClocksService>()->DataFor(evt); // Prod4 software only   
  //auto const detProp = art::ServiceHandle<detinfo::DetectorPropertiesService>()->DataFor(evt, clockData); // Prod4 software only  
  auto const detProp = lar::providerFrom<detinfo::DetectorPropertiesService>(); // Prod2 software only
  
  //======================================
  // Begin ProtoDUNEelectronAnaTree_module
  //======================================
  bool beamTriggerEvent = false;
  if(!evt.isRealData()){
      
    // Firstly we need to get the list of MCTruth objects from the generator. The standard protoDUNE
    auto mcTruths = evt.getValidHandle<std::vector<simb::MCTruth>>(fGeneratorTag);
//std::cerr << "\n fGeneratorTag=" << fGeneratorTag << "\n\n";   
//std::cerr << "\n mcTruths->size()=" << mcTruths->size() << "\n\n"; 
    // Also get the reconstructed beam information in the MC - TO DO
    const simb::MCParticle* geantGoodParticle = truthUtil.GetGeantGoodParticle((*mcTruths)[0],evt);

   
    if(geantGoodParticle != 0x0){
      beamTriggerEvent = true;
      fbeamtrackMomentum = geantGoodParticle->P();

      if( abs(geantGoodParticle->PdgCode()) != 11 ) return;  
    }
    
  } // MC
  else{ //data
    // For data we can see if this event comes from a beam trigger
    beamTriggerEvent = dataUtil.IsBeamTrigger(evt);
 
    if( !beamTriggerEvent ) return;
    art::Handle< std::vector<beam::ProtoDUNEBeamEvent> > pdbeamHandle;
    std::vector< art::Ptr<beam::ProtoDUNEBeamEvent> > beaminfo;
    if(evt.getByLabel(fBeamModuleLabel, pdbeamHandle))
      art::fill_ptr_vector(beaminfo, pdbeamHandle);
    else{
       std::cout<<"No beam information from "<<fBeamModuleLabel<<std::endl;
    } 
    if(beaminfo.size()){
      if( beamlineUtil.IsGoodBeamlineTrigger( evt ) ){  
        // Get Cerenkov
        fcerenkovStatus[0] = beaminfo[0]->GetCKov0Status();
        fcerenkovStatus[1] = beaminfo[0]->GetCKov1Status();
        auto & tracks = beaminfo[0]->GetBeamTracks();
        if(!tracks.empty()){
          fbeamtrackDir[0] = tracks[0].StartDirection().X();
          fbeamtrackDir[1] = tracks[0].StartDirection().Y();
          fbeamtrackDir[2] = tracks[0].StartDirection().Z();
        }
        auto & beammom = beaminfo[0]->GetRecoBeamMomenta();
        if(!beammom.empty()) fbeamtrackMomentum = beammom[0];
      } //good beam trigger
    }
  }//for data
 
  //check for reco pandora stuff
  art::Handle<std::vector<recob::PFParticle>> recoParticleHandle;
  if( !evt.getByLabel(fPFParticleTag,recoParticleHandle) ) return;

  // Get all of the PFParticles, by default from the "pandora" product
  auto recoParticles = evt.getValidHandle<std::vector<recob::PFParticle>>(fPFParticleTag);

  // We'd like to find the beam particle. Pandora tries to do this for us, so let's use the PFParticle utility 
  // to look for it. Pandora reconstructs slices containing one (or sometimes more) primary PFParticles. These
  // are tagged as either beam or cosmic for ProtoDUNE. This function automatically considers only those
  // PFParticles considered as primary
  std::vector<const recob::PFParticle*> pfParticles = pfpUtil.GetPFParticlesFromBeamSlice(evt,fPFParticleTag);
  for(const recob::PFParticle* particle : pfParticles){

    FillPrimaryPFParticle(evt, particle);
    
    // For now only consider the first primary track. Need a proper treatment if more than one primary particles are found.
    break;
  }

///*
  // Electron lifetime from database calibration service provider
  art::ServiceHandle<calib::LifetimeCalibService> lifetimecalibHandler;
  calib::LifetimeCalibService & lifetimecalibService = *lifetimecalibHandler; 
  calib::LifetimeCalib *lifetimecalib = lifetimecalibService.provider();

  if (evt.isRealData()) {
    fLifetime = lifetimecalib->GetLifetime()*1000.0; // [ms]*1000.0 -> [us]
//    std::cout << "\nuse lifetime from database(run " << fRun << ")   " << fLifetime << "\n\n";
  } 
  else {
    //fLifetime = detProp.ElectronLifetime(); // [us] prod4 
    fLifetime = detProp->ElectronLifetime(); // [us] prod2    
//    std::cout << "\nuse lifetime from DetectorProperties(run " << fRun << ")   " << fLifetime << "\n\n";
  }

  //------------------------------------------------------------
  // select track-like beam particles
  //------------------------------------------------------------
  if( recoParticles->size() > 0 ){
    const art::FindManyP<recob::Track> findTracks(recoParticles,evt,fTrackerTag);
    const std::vector<art::Ptr<recob::Track>> pfpTracks = findTracks.at(recoParticles->at(0).Self());
    if(pfpTracks.size() != 0){fTrack_index=1;}
    const art::FindManyP<recob::Shower> findShowers(recoParticles,evt,fShowerTag);
    const std::vector<art::Ptr<recob::Shower>> pfpShowers = findShowers.at(recoParticles->at(0).Self());
    if(pfpShowers.size() != 0){fShower_index=1;}
  }
//  if(fTrack_index==1 && fShower_index==0){std::cout << "Track-like   \n\n";}
//  if(fTrack_index==0 && fShower_index==1){std::cout << "Shower-like  \n\n";}
//  if(fTrack_index==1 && fShower_index==1){std::cout << "Ambiguity    \n\n";}
//  if(fTrack_index==0 && fShower_index==0){std::cout << "Unclassified \n\n";}
  

  //std::vector<int> hit_w;
  std::map<int,std::vector<double>> hit_w_and_t1;
  std::map<int,std::vector<double>> hit_w_and_t2;
  std::map<int,std::vector<double>> hit_w_and_y;
  std::map<int,std::vector<double>> hit_w_and_x;
//  std::map<int,std::vector<recob::Hit>> hits_shower;
  
//  std::vector<const recob::PFParticle*> pfParticles = pfpUtil.GetPFParticlesFromBeamSlice(evt,fPFParticleTag);
//std::cout << "\n event " << fevent << ": [PFParticlesFromBeamSlice] pfParticles.size()= " << pfParticles.size() << "\n\n";
  const simb::MCParticle* mcparticle = NULL;
  bool doAna = false;
  //art::ServiceHandle<cheat::BackTrackerService> bt_serv;

  // to check channels status
  m_pChannelStatusProvider = &art::ServiceHandle<lariov::ChannelStatusService>()->GetProvider();
  if ( m_pChannelStatusProvider == nullptr ) {
    std::cout << "\n WARNING: Channel status provider not found." << std::endl;
  }  
  
  for(const recob::PFParticle* particle : pfParticles){
     const recob::Shower* thisShower = pfpUtil.GetPFParticleShower(*particle,evt,fPFParticleTag,fShowerTag);
     if( thisShower == 0x0 ) continue;
     if( !evt.isRealData() ){
       mcparticle = truthUtil.GetMCParticleFromRecoShower(*thisShower, evt, fShowerTag); // Prod2
       //mcparticle = truthUtil.GetMCParticleFromRecoShower(clockData, *thisShower, evt, fShowerTag); // Prod4  
       if( abs(mcparticle->PdgCode()) != 11 ) return;
//std::cout << "\n mcpartice->E() [beam shower]=" << mcparticle->E();    
     }
     fprimaryStartPosition[0] = thisShower->ShowerStart().X();
     fprimaryStartPosition[1] = thisShower->ShowerStart().Y();
     fprimaryStartPosition[2] = thisShower->ShowerStart().Z();
     doAna = true; 
     const std::vector<const recob::Hit*> sh_hits = showerUtil.GetRecoShowerHits(*thisShower, evt, fShowerTag);     
     art::FindManyP<recob::Wire> wFromHits(sh_hits,evt,fHitTag);
     art::FindManyP<recob::SpacePoint> spFromShowerHits(sh_hits,evt,fPFParticleTag);

     for( size_t j=0; j<sh_hits.size() && j<kMaxShowerHits; ++j){
       if( sh_hits[j]->WireID().Plane != 2 ) continue;
       if( sh_hits[j]->WireID().TPC != 1 ) continue; // 2021-10-27
       std::vector<art::Ptr<recob::Wire>> wires = wFromHits.at(j);
       //hit_w.push_back(wires[0]->Channel());
       
       if ( m_pChannelStatusProvider != nullptr ) {
         if ( m_pChannelStatusProvider->IsNoisy(wires[0]->Channel()) ) {continue;} // 2021-10-28
         if ( m_pChannelStatusProvider->IsBad(wires[0]->Channel())   ) {continue;}   // 2021-10-28
       }
       
       hit_w_and_t1[wires[0]->Channel()].push_back(sh_hits[j]->PeakTimeMinusRMS(5.0));
       hit_w_and_t2[wires[0]->Channel()].push_back(sh_hits[j]->PeakTimePlusRMS(5.0));
       //hit_w_and_x[wires[0]->Channel()].push_back(detProp.ConvertTicksToX(sh_hits[j]->PeakTime(),sh_hits[j]->WireID().Plane,sh_hits[j]->WireID().TPC,0)); // prod 4
       hit_w_and_x[wires[0]->Channel()].push_back(detProp->ConvertTicksToX(sh_hits[j]->PeakTime(),sh_hits[j]->WireID().Plane,sh_hits[j]->WireID().TPC,0)); // prod 2
       //       hits_shower[wires[0]->Channel()].push_back(*sh_hits[j]);
       std::vector<art::Ptr<recob::SpacePoint>> sp = spFromShowerHits.at(j); 
       if(!sp.empty()){
          hit_w_and_y[wires[0]->Channel()].push_back(sp[0]->XYZ()[1]);
       }
       else hit_w_and_y[wires[0]->Channel()].push_back(fprimaryStartPosition[1]); //use vtx if no sp 

     }   
     
     break;
  }
 
  if( doAna ){  

    // Fill charge from shower
    std::map<channel_t,std::map<tick_t,double>> map_reco;
    double ymin;
    double ymax;
    FillShowerCharge(evt, hit_w_and_t1, hit_w_and_t2, hit_w_and_x, hit_w_and_y, map_reco, ymin, ymax);
    FillShowerCharge_fullSignal(evt);//, hit_w_and_t1, hit_w_and_t2, hit_w_and_x, hit_w_and_y, map_reco, ymin, ymax);  
    std::vector<int> trkids;
    art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
    //getDecayTrackIds(mcparticle, trkids, pi_serv);
    
    // Fill cosmic charge
    FillCosmicCharge(evt, hit_w_and_t1, hit_w_and_t2, hit_w_and_x, hit_w_and_y, ymin, ymax);

    
    // missing energy
    if(!evt.isRealData()) {
      auto mcTruths = evt.getValidHandle<std::vector<simb::MCTruth>>(fGeneratorTag);
      const simb::MCParticle* geantGoodParticle = truthUtil.GetGeantGoodParticle((*mcTruths)[0],evt);
      if( abs(geantGoodParticle->PdgCode()) != 11 ) {std::cerr << "\n geant good particle not an electron!\n";}
      if(geantGoodParticle->TrackId()!=1) {std::cerr << "\n geantGoodParticle is not a primary particle!! Fix me.\n";} 
      GetShowerParticlesTrueEnergyOutsideTPC(evt, geantGoodParticle, geantGoodParticle);
      GetShowerParticlesXYZE(evt, geantGoodParticle, geantGoodParticle);
      std::cerr << "\n total_energy_photons_outside_TPC_annihil=" << ftotal_energy_photons_outside_TPC_annihil << " GeV";
      std::cerr << "\n ftotal_energy_photons_outside_TPC_eBrem=" << fftotal_energy_photons_outside_TPC_eBrem << " GeV"; 
      std::cerr << "\n total_energy_electrons_outside_TPC=" << ftotal_energy_electrons_outside_TPC << " GeV"; 
      std::cerr << "\n total_energy_photons=" << ftotal_energy_photons << " GeV"; 
      std::cerr << "\n total_energy_electrons=" << ftotal_energy_electrons << " GeV";      
      GetTrueElectronEnergy(evt,mcparticle,pfParticles[0]);   
      GetTrueShowerEnergyNew(evt);         
      missenergy_evt = BuildTDCEnergySimMap(evt, map_reco);
    }
    
    
  }//shower
  
  
  if(beamTriggerEvent) fTree->Fill();  
}

// -----------------------------------------------------------------------------  
void ProtoDUNEelectronWireAna::FillShowerCharge(art::Event const & evt, 
                                          std::map<int,std::vector<double>> & hit_w_and_t1, 
                                          std::map<int,std::vector<double>> & hit_w_and_t2,
                                          std::map<int,std::vector<double>> & hit_w_and_x,
                                          std::map<int,std::vector<double>> & hit_w_and_y,
                                          std::map<channel_t,std::map<tick_t,double>> & map_reco,
                                          double & ymin,
                                          double & ymax )
{  
    

  // ----------- get charge from shower --------------   
  ymin=999; // minimum of hits y-position
  ymax=-999;// maximum of hits y-position
  
  auto const& wires = evt.getValidHandle<std::vector<recob::Wire> >(fWireTag);
  std::cerr << "\n (FillShowerCharge) event/run=" << fevent << "/" << fRun;

  if(!wires.isValid()) exit(-1);

  auto w1 = hit_w_and_t1.begin();
  auto w2 = hit_w_and_t2.begin(); 
  auto x  = hit_w_and_x.begin();
  auto y  = hit_w_and_y.begin();
    
  fprimary_Shower_nWires=-1;
  
  while( w1 != hit_w_and_t1.end()){

    int it_w = w1->first;
    int n_hits = w1->second.size();
    double t1 =  w1->second[0]; //first hit
    double t2 =  w2->second[n_hits-1]; //last hit 
    double x_w, y_w;
    if( x->second.size() < 1 ){
//std::cerr << "Im here 1 (xmin, xmax, etc...)\n";        
      x_w = (x->second[0]-x->second[n_hits-1])/2.0;
      y_w = (y->second[0]-y->second[n_hits-1])/2.0;
    }
    else {
      x_w = x->second[0];
      y_w = y->second[0];
    }
     
    for(size_t i=0; i<x->second.size(); i++) {
      if(y->second[i]<ymin) ymin=y->second[i];      
      if(y->second[i]>ymax) ymax=y->second[i];      
    }

//    for(auto & wire : * wires){  
    for(size_t iwire=0; iwire<wires->size() && iwire<kMaxWires; iwire++){
      //std::cerr<<"\n iwire(ROI)/channel=" << iwire << "/" << channel_no;  
      auto wire = (*wires)[iwire];  
      int channel_no = wire.Channel();
      int plane = fGeometry->View(wire.Channel()); 
      if( plane != 2 ) continue;
      
      if ( m_pChannelStatusProvider != nullptr ) {
        if ( m_pChannelStatusProvider->IsNoisy(channel_no) ) {continue;} // 2021-10-28
        if ( m_pChannelStatusProvider->IsBad(channel_no) ) {continue;}   // 2021-10-28
      }      
      
      std::vector< geo::WireID > wireID= fGeometry->ChannelToWire(channel_no);
      const geo::WireGeo* pwire = fGeometry->WirePtr(wireID.at(0)); //for collection plane there is one wire per channel (each recob::Wire is a ROI prod4)
      TVector3 xyzWire = pwire->GetCenter<TVector3>();
      
      int tpc = wireID[0].TPC;
      if( tpc != 1 ) continue; // 2021-10-27      
      
      if(it_w ==channel_no) {
              
        double charge_corr =0.0;
            
        for(size_t i = 0; i < wire.Signal().size(); ++i){
          double tick_in_us = i*500; // tick in ns
          tick_in_us /= 1000; // tick in us
          if( i > t1 && i < t2 ) {
            charge_corr += wire.Signal()[i]*exp(tick_in_us/fLifetime);
            //if(!map_reco[channel_no][i]) map_reco[channel_no][i] = charge_corr; // <=== we could have many recob::Wire for a given channel (prod4)
            map_reco[channel_no][i] = charge_corr; // prod2
          }
        }
        
        fprimary_Shower_nWires++;
        fprimary_Shower_wire_X[fprimary_Shower_nWires] = x_w;
        fprimary_Shower_wire_Y[fprimary_Shower_nWires] = y_w;        
        fprimary_Shower_wire_Z[fprimary_Shower_nWires] = xyzWire.Z();         
        fprimary_Shower_wire_ch[fprimary_Shower_nWires] = charge_corr;
        fprimary_Shower_wire_w[fprimary_Shower_nWires] = channel_no;

        break;
      }
    
    }

    w1 ++; w2 ++;
    x ++; y ++;
  }
  
  double calib_factor_MC=1.0205e-3; //SCE ON MC prod4, see https://indico.fnal.gov/event/50211/contributions/220626/attachments/145608/185442/dQdx_dEdx_new_calibrations_prod4a.pdf
  double calib_factor_data_5387=5.23e-3;//1 GeV
  double calib_factor= (evt.isRealData()) ? calib_factor_data_5387 : calib_factor_MC;
  double scale=(1/calib_factor)*((23.6/recombination)/1e9); // scale charge (ADC) to energy (GeV)  
  // ----------- end get charge from shower --------------     
  
  double shower_ch=0;
  for(int j=0; j<fprimary_Shower_nWires; ++j) {
    shower_ch += fprimary_Shower_wire_ch[j];
  } 
  fprimary_Shower_wire_energy = shower_ch*scale; // ADC    
  
  
  
  // ---- debug only ----
  std::cerr << "\n\n Shower reco energy from wires [TPC1] = " << fprimary_Shower_wire_energy << " [GeV]";
  // ---- end debug only ----

}



// -----------------------------------------------------------------------------  
void ProtoDUNEelectronWireAna::FillShowerCharge_fullSignal(art::Event const & evt//, 
                                                                      //std::map<int,std::vector<double>> & hit_w_and_t1, 
                                                                      //std::map<int,std::vector<double>> & hit_w_and_t2,
                                                                      //std::map<int,std::vector<double>> & hit_w_and_x,
                                                                      //std::map<int,std::vector<double>> & hit_w_and_y,
                                                                      //std::map<channel_t,std::map<tick_t,double>> & map_reco,
                                                                      //double & ymin,
                                                                      //double & ymax 
                                                          )
{  
    

  // ----------- get charge from shower --------------   
  //ymin=999; // minimum of hits y-position
  //ymax=-999;// maximum of hits y-position    
  
  auto const& wires = evt.getValidHandle<std::vector<recob::Wire> >(fWireTag);
  
  if(!wires.isValid()) exit(-1);

  //auto w1 = hit_w_and_t1.begin();
  //auto w2 = hit_w_and_t2.begin(); 
  //auto x  = hit_w_and_x.begin();
  //auto y  = hit_w_and_y.begin();
    
  fprimary_Shower_nWires_full=-1;
  
  //while( w1 != hit_w_and_t1.end()){

    //int it_w = w1->first;
    //int n_hits = w1->second.size();
    //double t1 =  w1->second[0]; //first hit
    //double t2 =  w2->second[n_hits-1]; //last hit 
    //double x_w, y_w;
    //if( x->second.size() < 1 ){           
//std::cerr << "Im here 1 (xmin, xmax, etc...)\n";        
      //x_w = (x->second[0]-x->second[n_hits-1])/2.0;
      //y_w = (y->second[0]-y->second[n_hits-1])/2.0;
    //}
    //else {
      //x_w = x->second[0];
      //y_w = y->second[0];
    //}
     
    //for(size_t i=0; i<x->second.size(); i++) {
      //if(y->second[i]<ymin) ymin=y->second[i];      
      //if(y->second[i]>ymax) ymax=y->second[i];      
    //}

//    for(auto & wire : * wires){  
//    for(size_t iwire=0; iwire<wires->size() && iwire<kMaxWires; iwire++){
    for(size_t iwire=0; iwire<wires->size() && iwire<4847; iwire++){               
      auto wire = (*wires)[iwire];  
      int channel_no = wire.Channel();
      int plane = fGeometry->View(wire.Channel()); 
      if( plane != 2 ) continue;

      if ( m_pChannelStatusProvider != nullptr ) {
        if ( m_pChannelStatusProvider->IsNoisy(channel_no) ) {continue;} // 2021-10-28
        if ( m_pChannelStatusProvider->IsBad(channel_no) ) {continue;}   // 2021-10-28
      }      
      
      std::vector< geo::WireID > wireID= fGeometry->ChannelToWire(channel_no);
      const geo::WireGeo* pwire = fGeometry->WirePtr(wireID.at(0)); //for collection plane there is one wire per channel
      TVector3 xyzWire = pwire->GetCenter<TVector3>();
      
      int tpc = wireID[0].TPC;
      if( tpc != 1 ) continue; // 2021-10-27
      
//      if(it_w ==channel_no) {
      //if(iwire<=4847) {          
           
        double charge=0;
        for(size_t i = 0; i < wire.Signal().size(); ++i) charge += wire.Signal()[i];
        if(charge==0) continue;
        
        double charge_corr =0.0;        
        
        for(size_t i = 0; i < wire.Signal().size(); ++i){
          double tick_in_us = i*500; // tick in ns
          tick_in_us /= 1000; // tick in us
          charge_corr += wire.Signal()[i]*exp(tick_in_us/fLifetime);
        }
//std::cerr << "\n iwire=" << iwire<< " / charge_corr=" << charge_corr;              
        fprimary_Shower_nWires_full++;
        //fprimary_Shower_wire_X_full[fprimary_Shower_nWires_full] = x_w;
        //fprimary_Shower_wire_Y_full[fprimary_Shower_nWires_full] = y_w;        
        fprimary_Shower_wire_Z_full[fprimary_Shower_nWires_full] = xyzWire.Z();         
        fprimary_Shower_wire_ch_full[fprimary_Shower_nWires_full] = charge_corr;
        //fprimary_Shower_wire_w_full[fprimary_Shower_nWires_full] = channel_no;

        //break;
      //}
    
    }
//std::cerr << "\n Im here 3";  
    //w1 ++; w2 ++;
    //x ++; y ++;
  //}
  
  double calib_factor_MC=1.0205e-3; //SCE ON MC prod4, see https://indico.fnal.gov/event/50211/contributions/220626/attachments/145608/185442/dQdx_dEdx_new_calibrations_prod4a.pdf
  double calib_factor_data_5387=5.23e-3;//1 GeV
  double calib_factor= (evt.isRealData()) ? calib_factor_data_5387 : calib_factor_MC;
  double scale=(1/calib_factor)*((23.6/recombination)/1e9); // scale charge (ADC) to energy (GeV)  
  // ----------- end get charge from shower --------------     
  
  double shower_ch=0;
  for(int j=0; j<fprimary_Shower_nWires_full; ++j) {
    shower_ch += fprimary_Shower_wire_ch_full[j];
  } 
  fprimary_Shower_wire_energy_full = shower_ch*scale; // ADC    
  
  
  
  // ---- debug only ----
  std::cerr << "\n\n Shower reco energy from wires [full signal/TPC1] = " << fprimary_Shower_wire_energy_full << " [GeV]";
  // ---- end debug only ----
//std::cerr << "\n Im here 3.2";   
}



// -----------------------------------------------------------------------------
void ProtoDUNEelectronWireAna::GetTrueShowerEnergyNew(art::Event const & evt)
{
    
  // get simchannels  
  art::Handle< std::vector<sim::SimChannel> > simchannelHandle;
  std::vector < art::Ptr < sim::SimChannel > > simchannel_v;
  if(evt.getByLabel(fSimChannelTag, simchannelHandle)){
    art::fill_ptr_vector(simchannel_v, simchannelHandle);
  }
  
  
  auto const* geo = lar::providerFrom<geo::Geometry>();
  
  
  art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
  for (unsigned int isim = 0; isim < simchannel_v.size(); ++isim) 
  {
    channel_t chanid = simchannel_v[isim]->Channel();//1
    const auto & wireid =  geo->ChannelToWire(chanid);//2
    if(wireid[0].Plane!=2) continue;//3
    int tpc = wireid[0].TPC;
    
    if ( m_pChannelStatusProvider != nullptr ) {
      if ( m_pChannelStatusProvider->IsNoisy(chanid) ) {continue;} // 2021-10-28
      if ( m_pChannelStatusProvider->IsBad(chanid) ) {continue;}   // 2021-10-28
    }
    
    const auto & tdcidemap = simchannel_v[isim]->TDCIDEMap();    
    
    for(auto mapitr = tdcidemap.begin(); mapitr != tdcidemap.end(); mapitr++)
    {
      auto const& tdc = (*mapitr).first;

      //if(!(tdc>=0 && tdc<=6000)) continue;//commented 2021/10/20
      
      double tick_in_us = tdc*500; // tick in ns
      tick_in_us /= 1000; // tick in us
     
      // Loop over energy deposits
      const std::vector<sim::IDE> idevec = (*mapitr).second;
      for(size_t iv = 0; iv < idevec.size(); ++iv){ 
        int trkid = idevec[iv].trackID;
        if( pi_serv->TrackIdToMotherParticle_P(trkid)->TrackId()!=1) continue;
         if(tpc==1) {
           fprimary_Shower_sim_energy_new_tpc1 += idevec[iv].energy;
           fprimary_Shower_sim_energy_new1_tpc1 += idevec[iv].energy*exp(tick_in_us/fLifetime);
           fprimary_Shower_sim_electrons_true0 +=  idevec[iv].numElectrons;
           if(tdc>=0 && tdc<=6000) {
             fprimary_Shower_sim_energy_new2_tpc1 += idevec[iv].energy;
             fprimary_Shower_sim_energy_new3_tpc1 += idevec[iv].energy*exp(tick_in_us/fLifetime); 
           }
         }
         else {
           fprimary_Shower_sim_energy_new_othertpc += idevec[iv].energy;
           fprimary_Shower_sim_energy_new1_othertpc += idevec[iv].energy*exp(tick_in_us/fLifetime);  
           fprimary_Shower_sim_electrons_true1 +=  idevec[iv].numElectrons;           
           if(tdc>=0 && tdc<=6000) {
             fprimary_Shower_sim_energy_new2_othertpc += idevec[iv].energy;
             fprimary_Shower_sim_energy_new3_othertpc += idevec[iv].energy*exp(tick_in_us/fLifetime);  
           }
         }
      }

    }    
    
  }

  std::cerr << "\n fprimary_Shower_sim_energy_new_tpc1=" << (fprimary_Shower_sim_energy_new_tpc1)/1000 << " GeV";
  std::cerr << "\n fprimary_Shower_sim_energy_new1_tpc1=" << (fprimary_Shower_sim_energy_new1_tpc1)/1000 << " GeV"; 
  std::cerr << "\n fprimary_Shower_sim_energy_new2_tpc1=" << (fprimary_Shower_sim_energy_new2_tpc1)/1000 << " GeV"; 
  std::cerr << "\n fprimary_Shower_sim_energy_new3_tpc1=" << (fprimary_Shower_sim_energy_new3_tpc1)/1000 << " GeV";   
    
}


// -----------------------------------------------------------------------------
void ProtoDUNEelectronWireAna::GetTrueElectronEnergy(art::Event const & evt, const simb::MCParticle* mcparticle, const recob::PFParticle* particle)
{
  auto mcTruths = evt.getValidHandle<std::vector<simb::MCTruth>>(fGeneratorTag);
  const simb::MCParticle* geantGoodParticle = truthUtil.GetGeantGoodParticle((*mcTruths)[0],evt);

  if( mcparticle->TrackId() == geantGoodParticle->TrackId() ){

    art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;

    fmcparticle_E = pi_serv->TrackIdToParticle_P(mcparticle->TrackId())->E();
    fmcparticle_P = pi_serv->TrackIdToParticle_P(mcparticle->TrackId())->P();    
    fmcparticle_EndE = pi_serv->TrackIdToParticle_P(mcparticle->TrackId())->EndE();  
    fmcparticle_EndP = pi_serv->TrackIdToParticle_P(mcparticle->TrackId())->EndMomentum().P();
     
    std::cerr << "\n (good particle) status=" << pi_serv->TrackIdToParticle_P(mcparticle->TrackId())->StatusCode();   
    std::cerr << "\n (good particle) pdg=" << pi_serv->TrackIdToParticle_P(mcparticle->TrackId())->PdgCode();  
    std::cerr << "\n (good particle) E=" << fmcparticle_E;   
    std::cerr << "\n (good particle) P=" << fmcparticle_P;     
    std::cerr << "\n (good particle) EndE=" << fmcparticle_EndE;   
    std::cerr << "\n (good particle) EndP=" << fmcparticle_EndP;    
      
  }//is MC same as beam?   

  
}

/*
// -----------------------------------------------------------------------------
void ProtoDUNEelectronWireAna::getDecayTrackIds(const simb::MCParticle* mcparticle, 
                                              std::vector<int> & trkids, 
                                              art::ServiceHandle<cheat::ParticleInventoryService> pi_serv)
{

  std::cerr << "\n mcparticle->TrackId()=" << mcparticle->TrackId();    
  if(mcparticle->NumberDaughters()) {
    for(int i=0; i<mcparticle->NumberDaughters(); i++) {
  std::cerr << "\n daughter " << i;         
      if(pi_serv->TrackIdToParticle_P(mcparticle->Daughter(i))->StatusCode()!=1) {  
      std::cerr << "\n pdg[i=" << i << "]=" << pi_serv->TrackIdToParticle_P(mcparticle->Daughter(i))->PdgCode();        
      std::cerr << "\n status[i=" << i << "]=" << pi_serv->TrackIdToParticle_P(mcparticle->Daughter(i))->StatusCode();    
      }
      //else {std::cerr << "\n daughter " << i; }
      trkids.push_back(mcparticle->Daughter(i));
      getDecayTrackIds(pi_serv->TrackIdToParticle_P(mcparticle->Daughter(i)), trkids, pi_serv);      
    }
  }

}
*/



///*
// -----------------------------------------------------------------------------
void ProtoDUNEelectronWireAna::GetShowerParticlesXYZE(art::Event const & evt, 
                                                    const simb::MCParticle* particle, 
                                                    const simb::MCParticle* geantGoodParticle)
{
  art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
  
  if(particle != 0x0){
      
    if(particle->PdgCode()==22) {
      h_photon_x->Fill(particle->Vx());  
      h_photon_endx->Fill(particle->EndX());
      h_photon_y->Fill(particle->Vy());  
      h_photon_endy->Fill(particle->EndY());
      h_photon_z->Fill(particle->Vz());  
      h_photon_endz->Fill(particle->EndZ());   
      ftotal_energy_photons += particle->E();
    }
    
    if(fabs(particle->PdgCode())==11 && (particle->TrackId() != geantGoodParticle->TrackId()) ) {
      h_electron_x->Fill(particle->Vx());  
      h_electron_endx->Fill(particle->EndX());
      h_electron_y->Fill(particle->Vy());  
      h_electron_endy->Fill(particle->EndY());
      h_electron_z->Fill(particle->Vz());  
      h_electron_endz->Fill(particle->EndZ());   
      ftotal_energy_electrons += particle->E();
    }
    
    if(fabs(particle->PdgCode())==11 && (particle->TrackId() == geantGoodParticle->TrackId()) ) {std::cerr << "\n (GetShowerParticlesXYZE) primary electron! Skip..";}

    for(int i=0; i<particle->NumberDaughters(); i++) {
      int daughter_trkid = particle->Daughter(i);   
      GetShowerParticlesXYZE(evt, pi_serv->TrackIdToParticle_P(daughter_trkid), geantGoodParticle);
    }
 
  }
  else{std::cerr << "\n No pointer to mcparticle!! Fix me\n"; exit (0);}

}
//*/




///*
// -----------------------------------------------------------------------------
void ProtoDUNEelectronWireAna::GetShowerParticlesTrueEnergyOutsideTPC(art::Event const & evt, 
                                                    const simb::MCParticle* particle, 
                                                    const simb::MCParticle* geantGoodParticle)
{
  art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
    
  //TPC boundaries
  //const int xneg = -360;
  const int xpos = 360;
  const int yneg = 0;
  const int ypos = 610;
  const int zneg = 0;
  const int zpos = 700;
    
  //const int negAPA1 = 226-10;
  //const int posAPA1 = 236+10;
  //const int negAPA2 = 456-10;    
  //const int posAPA2 = 472+10;  
  
  bool stop=false;
  
  if(particle != 0x0){
    
    if(particle->PdgCode()==22 &&
       ( fabs(particle->EndX())>xpos ||
         particle->EndY()>ypos || particle->EndY()<yneg || 
         particle->EndZ()>zpos || particle->EndZ()<zneg
       )
      )
    {
      if(particle->Process()=="annihil") {ftotal_energy_photons_outside_TPC_annihil += particle->E();} 
      else if(particle->Process()=="eBrem") {fftotal_energy_photons_outside_TPC_eBrem += particle->E();} 
      else {std::cerr << "\n photon process=" << particle->Process();}
      stop=true;
    }
    
    if(fabs(particle->PdgCode())==11 && (particle->TrackId() != geantGoodParticle->TrackId()) &&
       ( fabs(particle->EndX())>xpos ||
         particle->EndY()>ypos || particle->EndY()<yneg || 
         particle->EndZ()>zpos || particle->EndZ()<zneg
       )
      )
    {
      std::cerr << "\n electron process=" << particle->Process();  
      ftotal_energy_electrons_outside_TPC += particle->E();
      stop=true;
    }

    if(!stop) {
      for(int i=0; i<particle->NumberDaughters(); i++) {
        int daughter_trkid = particle->Daughter(i);    
        GetShowerParticlesTrueEnergyOutsideTPC(evt, pi_serv->TrackIdToParticle_P(daughter_trkid), geantGoodParticle); 
      }
    }
 
  }
  else{std::cerr << "\n No pointer to mcparticle!! Fix me\n"; exit (0);}

}
//*/



// -----------------------------------------------------------------------------
double ProtoDUNEelectronWireAna::BuildTDCEnergySimMap(
    art::Event const & evt,
    std::map<ProtoDUNEelectronWireAna::channel_t,std::map<ProtoDUNEelectronWireAna::tick_t,double>> map_reco) const // See ProtoDUNEelectronWireAnaCalib_module.cc
{
    
  // sim channel info
  art::Handle<std::vector<sim::SimChannel>> simChannelHandle;
  if( !evt.isRealData() ) {if( !evt.getByLabel(fSimChannelTag,simChannelHandle) ) {std::cerr << "\n Simchannels not found!!! check module labels**\n\n "; return -999;}}    
    
  // first collect the channels to look at
  std::vector<channel_t> channels;
  for (std::map<channel_t,std::map<tick_t,double>>::iterator it = map_reco.begin(); 
                                                             it != map_reco.end(); it++)
  { 
    channel_t channel=it->first; 
    channels.push_back(channel);
  }  
   
  // remove duplicates
  sort( channels.begin(), channels.end() );
  channels.erase( unique( channels.begin(), channels.end() ), channels.end() );
  
  // get simchannels  
  art::Handle< std::vector<sim::SimChannel> > simchannelHandle;
  std::vector < art::Ptr < sim::SimChannel > > simchannel_v;
  if(evt.getByLabel(fSimChannelTag, simchannelHandle)){
    art::fill_ptr_vector(simchannel_v, simchannelHandle);
  }
  
  auto const* geo = lar::providerFrom<geo::Geometry>();
  
  // loop over simchannels to fill sim map
  std::map<channel_t,std::map<tick_t,double>> map_sim;  
  for (unsigned int isim = 0; isim < simchannel_v.size(); ++isim)
  {
   channel_t channel=simchannel_v[isim]->Channel();
   if(!(std::find(channels.begin(), channels.end(), channel) != channels.end())) continue; // skip other sim channels without reco signals
   
   
   
   if ( m_pChannelStatusProvider != nullptr ) {
     if ( m_pChannelStatusProvider->IsNoisy(channel) ) {continue;} // 2021-10-28
     if ( m_pChannelStatusProvider->IsBad(channel) ) {continue;}   // 2021-10-28
   }   
   
   
   const auto & wireid = geo->ChannelToWire(channel);
   if (wireid[0].Plane != 2) continue; //collection plane only
   
   int tpc = wireid[0].TPC;
   
   if(tpc != 1) continue; // 2021-10-28
   
   const auto & tdcidemap = simchannel_v[isim]->TDCIDEMap(); 
   
   std::map<tick_t,double> map; // tick and energy in a given channel 
   
   for(auto mapitr = tdcidemap.begin(); mapitr != tdcidemap.end(); mapitr++)
   {
     auto const& tdc = (*mapitr).first;

     if(!(tdc>=0 && tdc<=6000)) continue;

     double tdc_energy = 0;
     
     // Loop over energy deposits
     const std::vector<sim::IDE> idevec = (*mapitr).second;
     for(size_t iv = 0; iv < idevec.size(); ++iv){ 
       tdc_energy += idevec[iv].energy;
       //numEle += idevec[iv].numElectrons;
     }
   
     // tick and energy in a given channel 
     map[tdc] = tdc_energy; 

   }
   
   map_sim[channel] = map;

  }
  

  // compare two maps to get missing energy (Tingjun => invert the loop sim before reco)
  double  missenergy=0;
  
  
  
  for (std::map<channel_t,std::map<tick_t,double>>::iterator itsim = map_sim.begin(); 
                                                             itsim != map_sim.end(); itsim++)
  {
    channel_t channel_sim=itsim->first;
    std::map<tick_t,double> simmap=itsim->second;
    for (std::map<tick_t,double>::iterator itsim2 = simmap.begin(); 
                                           itsim2 != simmap.end(); itsim2++)
    {
      tick_t tick_sim=itsim2->first;
      double energy_sim=itsim2->second;   
  
      for (std::map<channel_t,std::map<tick_t,double>>::iterator itreco = map_reco.begin(); 
                                                                 itreco != map_reco.end(); itreco++)
      {
        channel_t channel_reco=itreco->first;
        std::map<tick_t,double> recomap=itreco->second;
        for (std::map<tick_t,double>::iterator itreco2 = recomap.begin(); 
                                               itreco2 != recomap.end(); itreco2++)
        {
          tick_t tick_reco=itreco2->first;
          double signal_reco=itreco2->second;  
           
          if(channel_sim==channel_reco && tick_sim==tick_reco)
          {    
            if(energy_sim && !signal_reco)
            {
              missenergy++;
              missenergy_per_tick->Fill(energy_sim);
            }  
          }
        }
      }
    }
  }
  
  return  missenergy;
}


// -----------------------------------------------------------------------------
void ProtoDUNEelectronWireAna::FillPrimaryPFParticle(art::Event const & evt, const recob::PFParticle* particle){
    
  //auto const clockData = art::ServiceHandle<detinfo::DetectorClocksService>()->DataFor(evt); // Prod4
  //auto const detProp = art::ServiceHandle<detinfo::DetectorPropertiesService>()->DataFor(evt, clockData); // Prod4
  auto const detProp = lar::providerFrom<detinfo::DetectorPropertiesService>(); // Prod2
  
  // NHits associated with this pfParticle
  fprimaryNHits = (pfpUtil.GetPFParticleHits(*particle,evt,fPFParticleTag)).size();

  // "particle" is the pointer to our beam particle. The recob::Track or recob::Shower object
  // of this particle might be more helpful. These return null pointers if not track-like / shower-like
  const recob::Track* thisTrack   = pfpUtil.GetPFParticleTrack(*particle, evt,fPFParticleTag,fTrackerTag);
  const recob::Shower* thisShower = pfpUtil.GetPFParticleShower(*particle,evt,fPFParticleTag,fShowerTag);

//  const simb::MCParticle* mcparticle = NULL; 
  if(thisTrack != 0x0){     
    fprimaryIstrack                    = 1;
    fprimaryIsshower                   = 0;
    fprimaryStartDirection[0] = thisTrack->Trajectory().StartDirection().X();
    fprimaryStartDirection[1] = thisTrack->Trajectory().StartDirection().Y();
    fprimaryStartDirection[2] = thisTrack->Trajectory().StartDirection().Z();

  } // end is track
  else if(thisShower != 0x0){
    fprimaryIstrack                     = 0;
    fprimaryIsshower                    = 1;
    fprimaryStartDirection[0] = thisShower->Direction().X();
    fprimaryStartDirection[1] = thisShower->Direction().Y();
    fprimaryStartDirection[2] = thisShower->Direction().Z();
   
    const std::vector<const recob::Hit*> sh_hits = showerUtil.GetRecoShowerHits(*thisShower, evt, fShowerTag);
    art::FindManyP<recob::SpacePoint> spFromShowerHits(sh_hits,evt,fPFParticleTag);
    fprimaryShower_nHits=-1;

    for( size_t j=0; j<sh_hits.size() && j<kMaxShowerHits; ++j){
       if( sh_hits[j]->WireID().Plane != 2 ) continue;
       fprimaryShower_nHits++;
       art::FindManyP<recob::Wire> wFromHits(sh_hits,evt,fHitTag);
       std::vector<art::Ptr<recob::Wire>> wires = wFromHits.at(j);
       const geo::WireGeo* pwire = fGeometry->WirePtr(sh_hits[j]->WireID());
       TVector3 xyzWire = pwire->GetCenter<TVector3>();
//       double p_trk_or_sh = cnn_out[ hitResults.getIndex("track") ]+ cnn_out[ hitResults.getIndex("em") ]; 
//       fprimaryShowerCharge += sh_hits[j]->Integral();
//       fprimaryShower_hit_w.push_back(sh_hits[j]->WireID().Wire);
       fprimaryShower_hit_t[fprimaryShower_nHits] = sh_hits[j]->PeakTime();
       fprimaryShower_hit_q[fprimaryShower_nHits] = sh_hits[j]->Integral();        
       //fprimaryShower_hit_X[fprimaryShower_nHits] = detProp.ConvertTicksToX(sh_hits[j]->PeakTime(),sh_hits[j]->WireID().Plane,sh_hits[j]->WireID().TPC,0); // prod 4   
       fprimaryShower_hit_X[fprimaryShower_nHits] = detProp->ConvertTicksToX(sh_hits[j]->PeakTime(),sh_hits[j]->WireID().Plane,sh_hits[j]->WireID().TPC,0); // prod 2          
       fprimaryShower_hit_Z[fprimaryShower_nHits] = xyzWire.Z();  
       fprimaryShower_hit_ch[fprimaryShower_nHits] = wires[0]->Channel(); //only one channel per wire at collection plane
       std::vector<art::Ptr<recob::SpacePoint>> sp = spFromShowerHits.at(j); 
       
       if(!sp.empty() ){
         //fprimaryShower_hit_X[idx]= sp[0]->XYZ()[0];
         fprimaryShower_hit_Y[fprimaryShower_nHits] = sp[0]->XYZ()[1];
         //fprimaryShower_hit_Z[idx]= sp[0]->XYZ()[2];
       }
    
    }

    
    // Calorimetry only colleciton plane
    std::vector<anab::Calorimetry> calovector = showerUtil.GetRecoShowerCalorimetry(*thisShower, evt, fShowerTag, fShowerCaloTag);
    if(calovector.size() != 3 && fVerbose > 0)
      std::cerr << "WARNING::Calorimetry vector size for primary is = " << calovector.size() << std::endl;

  } // end is shower

}

// -----------------------------------------------------------------------------

void ProtoDUNEelectronWireAna::Initialize(){
    
  fRun =-999;
//  fSubRun =-999;
  fevent =-999;
  
  fbeamtrackMomentum = -999;
  fcerenkovStatus[0]= -999;
  fcerenkovStatus[1]= -999;   
  fbeamtrackDir[0] = -999;
  fbeamtrackDir[1] = -999;
  fbeamtrackDir[2] = -999;   
  
  fprimaryIstrack = -999;
  fprimaryIsshower = -999;  
  fprimaryNHits = -999;
  fprimaryStartDirection[0] = -999;
  fprimaryStartDirection[1] = -999;
  fprimaryStartDirection[2] = -999;  
  fprimaryShower_nHits = -999;

  for(size_t i=0; i<kMaxShowerHits; i++) {
//    fprimaryShower_hit_w[i] = -999;
    fprimaryShower_hit_q[i] = -999;
    fprimaryShower_hit_t[i] = -999;
    fprimaryShower_hit_X[i] = -999;
    fprimaryShower_hit_Y[i] = -999;
    fprimaryShower_hit_Z[i] = -999;
    fprimaryShower_hit_ch[i] = -999;
  }
  
  fLifetime = -999;
  
  fTrack_index = -999;
  fShower_index = -999; 

  missenergy_evt = -999;

  fmcparticle_E = 0;  
  fmcparticle_P = 0;   
  fmcparticle_EndE = 0;
  fmcparticle_EndP = 0;  
  ftotal_energy_photons = 0;
  ftotal_energy_electrons = 0;  
  ftotal_energy_photons_outside_TPC_annihil = 0;
  fftotal_energy_photons_outside_TPC_eBrem = 0;
  ftotal_energy_electrons_outside_TPC = 0;
  
  fprimary_Shower_nWires = 0;
  fprimary_Shower_nWires_full = 0;  
 
  fprimary_Shower_sim_energy_new_tpc1 = 0;
  fprimary_Shower_sim_energy_new1_tpc1 = 0;
  fprimary_Shower_sim_energy_new2_tpc1 = 0;
  fprimary_Shower_sim_energy_new3_tpc1 = 0;  
  fprimary_Shower_sim_energy_new_othertpc = 0;
  fprimary_Shower_sim_energy_new1_othertpc = 0;
  fprimary_Shower_sim_energy_new2_othertpc = 0;
  fprimary_Shower_sim_energy_new3_othertpc = 0;    


  fprimary_Shower_sim_electrons_true0 = 0; 
  fprimary_Shower_sim_electrons_true1 = 0;   
  
  for(size_t i=0; i<kMaxWires; i++) {  
    fprimary_Shower_wire_w[i] = -999;
    fprimary_Shower_wire_Y[i] = -999;
    fprimary_Shower_wire_X[i] = -999;
    fprimary_Shower_wire_Z[i] = -999;
    fprimary_Shower_wire_ch[i] = -999;
    fprimary_Shower_wire_w_full[i] = -999;
    fprimary_Shower_wire_Y_full[i] = -999;
    fprimary_Shower_wire_X_full[i] = -999;
    fprimary_Shower_wire_Z_full[i] = -999;
    fprimary_Shower_wire_ch_full[i] = -999;    
  }
  
  fprimary_Shower_wire_energy = -999;   

  fcosmic_bkg_energy = -999;
  fcosmic_bkg_energy_1sigma = -999;
  fcosmic_bkg_energy_3sigma = -999;
  fcosmic_bkg_energy_5sigma = -999;
  fcosmic_bkg_energy_outsideYregion = -999;
  fcosmic_bkg_energy_1sigma_outsideYregion = -999;
  fcosmic_bkg_energy_3sigma_outsideYregion = -999;
  fcosmic_bkg_energy_5sigma_outsideYregion = -999;  
  
  fprimaryStartPosition[0] = -999;
  fprimaryStartPosition[1] = -999;
  fprimaryStartPosition[2] = -999;    
  
  fcosmicNTracks = -999;
  fcosmicNTracks_PF = -999;   
  fcosmicNTracks_PF_length = -999;   
  fcosmicNTracks_PF_length_crossTPC = -999;   
  fcosmicNTracks_PF_length_crossTPC_diverters = -999;   
  fcosmicNTracks_PF_length_crossTPC_diverters_notbroken = -999;   
  fcosmicNTracks_PF_length_crossTPC_diverters_notbroken_angle = -999;   
  fcosmicNTracks_PF_length_crossTPC_diverters_notbroken_angle_earlyTrack = -999; 

  for(size_t i=0; i<kMaxTracks; i++) {
//    fcosmicTrack_nHits[i] = -999;
//    fcosmicTrack_nPFs[i] = -999;
//    fcosmicTrack_nT0s[i] = -999;
    fcosmicTrack_theta_xz[i] = -999; 
    fcosmicTrack_theta_yz[i] = -999;
    fcosmicTrack_startx[i] = -999;
    fcosmicTrack_starty[i] = -999;
    fcosmicTrack_startz[i] = -999;
    fcosmicTrack_endx[i] = -999;
    fcosmicTrack_endy[i] = -999;
    fcosmicTrack_endz[i] = -999;
    fcosmicTrack_length[i] = -999;
//    fcosmicTrack_minHitPeakTime[i] = -999;
//    fcosmicTrack_maxHitPeakTime[i] = -999;   
  }

}

DEFINE_ART_MODULE(test::ProtoDUNEelectronWireAna)
