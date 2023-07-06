/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
#include "Riostream.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TList.h"
#include "AliAnalysisTaskSE.h"
#include "AliAnalysisManager.h"
#include "AliStack.h"
#include "AliESDtrackCuts.h"
#include "AliESDEvent.h"
#include "AliVEvent.h"
#include "AliESDInputHandler.h"
#include "AliAODEvent.h"
#include "AliMCEvent.h"
#include "AliESDPmdTrack.h"
#include "AliPmdAnalysisTask.h"
#include "AliESDVertex.h"
#include "AliESDVZERO.h"
#include "TProfile.h"
#include "AliPMDUtility.h"

ClassImp(AliPmdAnalysisTask)

//________________________________________________________________________
AliPmdAnalysisTask::AliPmdAnalysisTask() : AliAnalysisTaskSE(),
  fOutput(0),
// fSmn(10), 
  fHistTotEvent(0),
  fHistVtxZ(0),
  fHistVtxZAfterCut(0),
  fHistAdcPre(0), 
  fHistAdcCpv(0),
  fHistNcellCpv(0),
  fHistNcellPre(0),
  fHistClusterXYPre(0),           
  fHistNcellModulePre(0),
  fTreeModule(0),
  fTDet(0),fTSmn(0),fTRow(0),
  fTCol(0),fTAdc(0),
  fHistClusterXYCpv(0){
}
//________________________________________________________________________
AliPmdAnalysisTask::AliPmdAnalysisTask(const char *name) // All data members should be initialised here
  :AliAnalysisTaskSE(name),
   fOutput(0), 
   fHistTotEvent(0),
   fHistVtxZ(0),
   fHistVtxZAfterCut(0),
   fHistAdcPre(0), 
   fHistAdcCpv(0),
   fHistNcellCpv(0),
   fHistNcellPre(0),
   fHistClusterXYPre(0),
   fHistNcellModulePre(0),
   fTreeModule(0),
   fTDet(0),fTSmn(0),fTRow(0),
   fTCol(0),fTAdc(0),
   fHistClusterXYCpv(0) {
  DefineOutput(1, TList::Class());// for output list
}

//________________________________________________________________________
AliPmdAnalysisTask::~AliPmdAnalysisTask() {
  if (fOutput && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutput;
}
//________________________________________________________________________
void AliPmdAnalysisTask::UserCreateOutputObjects() {
  fOutput = new TList();
  fOutput->SetOwner(kTRUE);  

  //Total Events
  fHistTotEvent = new TH1F("fHistTotEvent","fHistTotEvent",6,0,6);
  fHistTotEvent->GetXaxis()->SetBinLabel(1, "Total events");
  fHistTotEvent->GetXaxis()->SetBinLabel(2, "Events after trigger");
  fHistTotEvent->GetXaxis()->SetBinLabel(3, "Events after Pileup-SPD");
  fHistTotEvent->GetXaxis()->SetBinLabel(4, "Events after NContributors > 0");
  fHistTotEvent->GetXaxis()->SetBinLabel(5, "Events after GetZRes != 0");
  fHistTotEvent->GetXaxis()->SetBinLabel(6, "Selected events |VtxZ|<10");
  fOutput->Add(fHistTotEvent);   
  
  fHistVtxZ = new TH1F("fHistVtxZ","VtxZ",100,-50,50);
  fOutput->Add(fHistVtxZ);
  
  fHistVtxZAfterCut = new TH1F("fHistVtxZAfterCut","VtxZ After Cut",100,-20,20);
  fOutput->Add(fHistVtxZAfterCut);
  
  fHistAdcPre = new TH1F("fHistAdcPre", "ADC Distribution for PRE module", 2000, 0, 2000);
  fHistAdcCpv = new TH1F("fHistAdcCpv", "ADC Distribution for CPV module", 2000, 0, 2000);
  
  fHistNcellModulePre = new TH2F("fHistNcellModulePre", "Ncell vs Module PRE module", 
			       30,0,30,4000,0,4000);
  fTreeModule = new TTree("fTreeModule","Module data");
  fTDet = new Int_t(0);
  fTSmn = new Int_t(0);
  fTRow = new Int_t(0);
  fTCol = new Int_t(0);
  fTAdc = new Float_t(0);
  fTreeModule->Branch("Detector",fTDet);
  fTreeModule->Branch("Module",fTSmn);
  fTreeModule->Branch("Row",fTRow);
  fTreeModule->Branch("Column",fTCol);
  fTreeModule->Branch("ADC",fTAdc);

  fHistClusterXYPre = new TH2F("fHistClusterXYPre", "ClusterX-Y Plot for PRE module", 
			       200,-100,100,200,-100,100);
  fHistClusterXYCpv = new TH2F("fHistClusterXYCpv", "ClusterX-Y Plot for CPV module", 
			       200,-100,100,200,-100,100);
  fHistNcellCpv = new TH1F("NcellCpv","NcellCpv",5,0,5);
  fHistNcellPre = new TH1F("NcellPre","NcellPre",5,0,5);
  
  fOutput->Add(fHistAdcPre);
  fOutput->Add(fHistAdcCpv);
  fOutput->Add(fHistClusterXYPre);
  fOutput->Add(fHistClusterXYCpv);
  fOutput->Add(fHistNcellCpv);
  fOutput->Add(fHistNcellPre);
  fOutput->Add(fHistNcellModulePre);
  fOutput->Add(fTreeModule);
 PostData(1, fOutput);   
}

//________________________________________________________________________
void AliPmdAnalysisTask::UserExec(Option_t *) {
  
  Double_t etacls, theta, rdist,phi;
  Int_t xpos, ypos;
  Float_t Globalx, Globaly;
  AliPMDUtility cc;
  AliVEvent *event = InputEvent();
  if (!event) { Printf("ERROR: Could not retrieve event"); return; }
  AliESDEvent* esd = dynamic_cast<AliESDEvent*>(event);
  if (!esd) {
    AliError("Cannot get the ESD event");
    return;
  }  
  fHistTotEvent->Fill(0.5);
  printf("All events \n");

  //event selection
  Bool_t isSelected = (((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & AliVEvent::kINT7);
  if (!isSelected) return;
  
  fHistTotEvent->Fill(1.5);
  printf("Trigger events \n");
  Bool_t pileUp = esd->IsPileupFromSPD(3, 0.8, 3., 2., 5.);
  if(pileUp) return;
  printf("After pileup events \n");
  fHistTotEvent->Fill(2.5);
  
  const AliESDVertex *vertex = esd->GetPrimaryVertex();
  Float_t Vz = vertex->GetZ();
  fHistVtxZ->Fill(Vz);
  Bool_t zVerStatus = vertex->GetStatus();
  printf("After vtx events \n");
  if(zVerStatus){
    if(vertex->GetNContributors() > 0){
      fHistTotEvent->Fill(3.5);
      if(vertex->GetZRes() != 0){
	fHistTotEvent->Fill(4.5);
	if(TMath::Abs(Vz) < 10){
	  //events accpeted for analysis
	  fHistTotEvent->Fill(5.5);
	  fHistVtxZAfterCut->Fill(Vz);

	  //*********** Comments by Sidharth Kumar Prasad (Contact: sprasad@cern.ch) ************//
	  // For PbPb system the code is modified and hits information
	  // are stored in the ESD files.
	  // However the names of the fuctions of the classes
	  // were not changed. Please do not get confused with
	  // the function names used.
	  // Please read the commented lines carefully against
	  // each function used.
	  //Follow the slides below for more details:
	  //https://indico.cern.ch/event/855732/contributions/3668479/attachments/1969444/3275647/2020-01-15-Pmd.pdf
	  //*********************************************************************************//
	  Int_t pmdhits = esd->GetNumberOfPmdTracks();//Total PMD hits
	  printf ("Number of PMD Hits: %8d \n", pmdhits) ;
	  while (pmdhits--) {//hits loop
	    AliESDPmdTrack *pmdtr = esd->GetPmdTrack(pmdhits);//retreiving PMD hit
	    Int_t det     = pmdtr->GetDetector();//detector; 0=PRE; 1=CPV
	    Int_t smn     = pmdtr->GetSmn();//module number; 0 to 23 for CPV and 0 to 23 for PRE
	    Int_t Row  = pmdtr->GetClusterX();//cell row; 0 to 47
	    Int_t Col  = pmdtr->GetClusterY();//cell coloumn; 0 to 95
	    Float_t clsZ  = pmdtr->GetClusterZ();//cell z-position
	    Float_t ncell = pmdtr->GetClusterCells();//no. of cells in cluster (in this case it is 1)
	    Float_t adc   = pmdtr->GetClusterADC();//cell adc
	    Float_t sigX = pmdtr->GetClusterSigmaX(); //Track number
	    Float_t sigY = pmdtr->GetClusterSigmaY(); //Track PID
	    //Converting Row to Col and Col to Row depending on the module type (short or long)
	    if(smn < 12){
	      xpos = Col;
	      ypos = Row;
	    }//if
	    else if(smn >= 12 && smn < 24){
	      xpos = Row;
	      ypos = Col;
	    }//else if
	    if(det == 0) {//PMD-PRE
	      fHistAdcPre->Fill(adc);
	      fHistNcellPre->Fill(ncell);
	      cc.RectGeomCellPos(smn,xpos,ypos,Globalx,Globaly);
	      fHistClusterXYPre->Fill(Globalx,Globaly);
	      fHistNcellModulePre->Fill(smn,ncell);
	    }//if det
	    else if(det == 1) {//PMD-CPV
	      fHistAdcCpv->Fill(adc);
	      fHistNcellCpv->Fill(ncell);
	      cc.RectGeomCellPos(smn,xpos,ypos,Globalx,Globaly);
	      fHistClusterXYCpv->Fill(Globalx,Globaly);
	    }//if det
	    *fTDet = det;
	    *fTSmn = smn;
	    *fTRow = Row;
	    *fTCol = Col;
	    *fTAdc = adc;
	    fTreeModule->Fill();
	  }//hits loop
	}//vtxZ < 10
      }//if (GetzRes)
    }//if vertexContributors
  }//ifVerTstatus
  PostData(1, fOutput);
}//UserExec
//________________________________________________________________________
void AliPmdAnalysisTask::Terminate(Option_t *) {
  fOutput = dynamic_cast<TList*> (GetOutputData(1));
  if(!fOutput) { Printf("ERROR: could not retrieve TList fOutput"); return; }
}
