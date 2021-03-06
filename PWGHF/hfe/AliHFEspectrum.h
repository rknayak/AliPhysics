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
//
// Class for spectrum correction
// Subtraction of hadronic background, Unfolding of the data and
// Renormalization done here
// For more information see the implementation file
//
#ifndef ALIHFESPECTRUM_H
#define ALIHFESPECTRUM_H

#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

class TGraphErrors;
class TObject;
class TH1;
class TF1;
class TList;
class TObjArray;
class AliCFContainer;
class AliHFEcontainer;
class AliCFDataGrid;
class AliCFEffGrid;

class AliHFEspectrum : public TNamed{
  public:
    enum CFContainer_t{
      kDataContainer  = 0,
      kBackgroundData = 1,
      kMCContainerMC = 2,
      kMCContainerESD = 3,
      kMCContainerCharmMC = 4,
      kMCWeightedContainerNonHFEESD =5,
      kMCWeightedContainerConversionESD = 6,
      kDataContainerV0 = 7
    };

    enum{
      kElecBgSources = 6,
      kBgLevels = 3,
      kBgPtBins = 44,
      kCentrality = 12
	};

   enum Chargetype_t{
     kNegCharge = -1,
     kPosCharge = 1,
     kAllCharge = 0
   };
   
    AliHFEspectrum(const char* name);
    ~AliHFEspectrum();
    

    Bool_t Init(const AliHFEcontainer *datahfecontainer, const AliHFEcontainer *mchfecontainer, const AliHFEcontainer *v0hfecontainer=0x0, const AliHFEcontainer *bghfecontainer=0x0);
    Bool_t Correct(Bool_t subtractcontamination=kTRUE);
    Bool_t CorrectBeauty(Bool_t subtractcontamination=kTRUE);
   
    AliCFDataGrid *SubtractBackground(Bool_t setBackground = kFALSE);
    
    AliCFDataGrid *CorrectV0Efficiency(AliCFDataGrid* const bgsubpectrum = 0x0);
    AliCFDataGrid *CorrectParametrizedEfficiency(AliCFDataGrid* const bgsubpectrum = 0x0);
   
    TList *Unfold(AliCFDataGrid* const bgsubpectrum = 0x0);
    void UnfoldBG(AliCFDataGrid* const bgsubpectrum);
    AliCFDataGrid *CorrectForEfficiency(AliCFDataGrid* const bgsubpectrum = 0x0);
   
    TGraphErrors *Normalize(THnSparse * const spectrum,Int_t i = 0) const;
    TGraphErrors *Normalize(AliCFDataGrid * const spectrum,Int_t i = 0) const;
    TGraphErrors *NormalizeTH1N(TH1 *input,Int_t normalization) const;
    void CorrectFromTheWidth(TH1D *h1) const;
    void CorrectStatErr(AliCFDataGrid *backgroundGrid) const;
    
    void SetCorrelation(THnSparseF * const correlation) {fCorrelation = correlation; };
    void SetContainer(AliCFContainer *cont, AliHFEspectrum::CFContainer_t type);
    void SetEfficiencyFunction(TF1 *efficiencyFunction) { fEfficiencyFunction = efficiencyFunction; };
    void SetPbPbAnalysis(Bool_t isPbPb = kFALSE) { fBeamType=(Char_t) isPbPb; };
    void SetEtaSyst(Bool_t etaSyst = kTRUE) { fEtaSyst = etaSyst; };

    void SetParameterizedEff(AliCFContainer *container, AliCFContainer *containermb, AliCFContainer *containeresd, AliCFContainer *containeresdmb, Int_t *dimensions);
    
    void SetNumberOfEvents(Int_t nEvents,Int_t i = 0) { fNEvents[i] = nEvents; };
    void SetNumberOfMCEvents(Int_t nEvents) { fNMCEvents = nEvents; };
    void SetNumberOfMC2Events(Int_t nEvents,Int_t i = 0) { fNMCbgEvents[i] = nEvents; };
    void SetMCEffStep(Int_t step) { fStepMC = step; };
    void SetMCTruthStep(Int_t step) { fStepTrue = step; };
    void SetStepToCorrect(Int_t step) { fStepData = step; };
    void SetStepBeforeCutsV0(Int_t step) { fStepBeforeCutsV0 = step; };
    void SetStepAfterCutsV0(Int_t step) { fStepAfterCutsV0 = step; };
    void SetNbDimensions(Int_t nbDimensions) { fNbDimensions = nbDimensions; };
    void SetChargeChoosen(Chargetype_t chargechoosen) {fChargeChoosen = chargechoosen; };
    void SetEtaRange(Double_t etamin, Double_t etamax) { fEtaRange[0] = etamin; fEtaRange[1] = etamax; fEtaSelected = kTRUE; }
    void SetUnSetCorrelatedErrors(Bool_t unsetcorrelatederrors) {fUnSetCorrelatedErrors = unsetcorrelatederrors;};
    void SetSmoothing(Bool_t setSmoothing) {fSetSmoothing = setSmoothing;};
    void SetTestOneBinCentrality(Double_t centralitymin, Double_t centralitymax) { fTestCentralityLow = centralitymin; fTestCentralityHigh = centralitymax;}
    void SetFillMoreCorrelationMatrix(Bool_t fillMoreCorrelationMatrix) { fFillMoreCorrelationMatrix = fillMoreCorrelationMatrix;}

    void SetNCentralityBinAtTheEnd(Int_t nCentralityBinAtTheEnd) {fNCentralityBinAtTheEnd = nCentralityBinAtTheEnd; };
    void SetLowHighBoundaryCentralityBinAtTheEnd(Int_t low, Int_t high, Int_t i) { fLowBoundaryCentralityBinAtTheEnd[i] = low; fHighBoundaryCentralityBinAtTheEnd[i] = high;};

    void SetBeautyAnalysis() { fInclusiveSpectrum = kFALSE; };
    void CallInputFileForBeauty2ndMethod();
    void SetInputFileForBeauty2ndMethod(const char *filenameb = "BSpectrum2ndmethod.root"){fkBeauty2ndMethodfilename = filenameb; };
    void SetBeautyAnalysis2ndMethod(Bool_t beauty2ndmethod) { fBeauty2ndMethod = beauty2ndmethod; }
    void SetIPEffCombinedSamples(Bool_t ipEffCombinedSamples) { fIPEffCombinedSamples = ipEffCombinedSamples; }
    void SetHadronEffbyIPcut(THnSparseF* hsHadronEffbyIPcut) { fHadronEffbyIPcut = hsHadronEffbyIPcut;};
    void SetNonHFEsyst(Bool_t syst){ fNonHFEsyst = syst; };

    void SetStepGuessedUnfolding(Int_t stepGuessedUnfolding) { fStepGuessedUnfolding = stepGuessedUnfolding; };
    void SetNumberOfIteration(Int_t numberOfIteration) { fNumberOfIterations = numberOfIteration; };
    void SetUnfoldingRandomIterations(Int_t niter) { fNRandomIter = niter; }
    
    void SetDumpToFile(Bool_t dumpToFile) { fDumpToFile=dumpToFile; }; 
  
    void SetDebugLevel(Int_t debugLevel, Bool_t writeToFile = kFALSE) { fDebugLevel = debugLevel; fWriteToFile = writeToFile; };
    void SetUnfoldBG() { fUnfoldBG = kTRUE; };


    AliCFDataGrid* GetRawBspectra2ndMethod();
    AliCFDataGrid* GetCharmBackground();
    AliCFDataGrid* GetConversionBackground();
    AliCFDataGrid* GetNonHFEBackground();
    THnSparse* GetCharmWeights();
    THnSparse* GetBeautyIPEff(Bool_t isMCpt);
    THnSparse* GetPIDxIPEff(Int_t source);
    void CalculateNonHFEsyst(Int_t centrality = 0);

    void EnableIPanaHadronBgSubtract() { fIPanaHadronBgSubtract = kTRUE; };
    void EnableIPanaCharmBgSubtract() { fIPanaCharmBgSubtract = kTRUE; };
    void EnableIPanaConversionBgSubtract() { fIPanaConversionBgSubtract = kTRUE; };
    void EnableIPanaNonHFEBgSubtract() { fIPanaNonHFEBgSubtract = kTRUE; };
    void EnableIPParameterizedEff() { fIPParameterizedEff = kTRUE; };
    TObject* GetSpectrum(const AliCFContainer * const c, Int_t step);
    AliCFContainer *GetContainer(AliHFEspectrum::CFContainer_t contt);

  protected:
       
    
    AliCFContainer *GetSlicedContainer(AliCFContainer *cont, Int_t ndim, Int_t *dimensions,Int_t source=-1,Chargetype_t charge=kAllCharge,Int_t centralitylow=-1, Int_t centralityhigh=-1);
    THnSparseF *GetSlicedCorrelation(THnSparseF *correlationmatrix,Int_t nDim, Int_t *dimensions,Int_t centralitylow=-1, Int_t centralityhigh=-1) const;
    TObject* GetEfficiency(const AliCFContainer * const c, Int_t step, Int_t step0);
 
    void AddTemporaryObject(TObject *cont);
    void ClearObject(TObject *o);
    
    TGraphErrors *NormalizeTH1(TH1 *input,Int_t i = 0) const;


  private:
    AliHFEspectrum(const AliHFEspectrum &);
    AliHFEspectrum &operator=(const AliHFEspectrum &);
 
    TObjArray *fCFContainers;     // List of Correction Framework Containers
    TList *fTemporaryObjects;     // Emulate garbage collection
    THnSparseF *fCorrelation;     // Correlation Matrices
    AliCFDataGrid *fBackground;   // Background Grid
    TF1 *fEfficiencyFunction;     // Efficiency Function
    TF1 *fEfficiencyTOFPIDD[kCentrality];       // TOF PID efficiency parameterized
    TF1 *fEfficiencyesdTOFPIDD[kCentrality];    // TOF PID efficiency parameterized
    TF1 *fEfficiencyIPCharmD[kCentrality];      // IP efficiency parameterized for charm
    TF1 *fEfficiencyIPBeautyD[kCentrality];     // IP efficiency parameterized for beauty 
    TF1 *fEfficiencyIPBeautyesdD[kCentrality];  // IP efficiency parameterized for beauty for esd
    TF1 *fEfficiencyIPConversionD[kCentrality]; // IP efficiency parameterized for conversion
    TF1 *fEfficiencyIPNonhfeD[kCentrality];     // IP efficiency parameterized for nonhfe 

    THnSparseF *fWeightCharm;     // Weight for charm bg

    AliCFContainer *fConvSourceContainer[kElecBgSources][kBgLevels][kCentrality]; //container for conversion electrons, divided into different photon sources
    AliCFContainer *fNonHFESourceContainer[kElecBgSources][kBgLevels][kCentrality]; //container for non-HF electrons, divided into different sources

    Bool_t fInclusiveSpectrum;     // Inclusive Spectrum
    Bool_t fDumpToFile;           // Write Result in a file

    Bool_t fEtaSelected;              // Switch for eta selection
    Bool_t fUnSetCorrelatedErrors;    // Unset correlated errors
    Bool_t fSetSmoothing;             // Set smoothing

    Bool_t fIPanaHadronBgSubtract;     // Hadron background subtraction
    Bool_t fIPanaCharmBgSubtract;      // Charm background subtraction 
    Bool_t fIPanaConversionBgSubtract; // Conversion background subtraction
    Bool_t fIPanaNonHFEBgSubtract;     // nonHFE except for conversion background subtraction
    Bool_t fIPParameterizedEff;        // switch to use parameterized efficiency for ip analysis
    Bool_t fNonHFEsyst;            // choose NonHFE background level (upper, lower, central)
    Bool_t fBeauty2ndMethod;      // 2nd method to get beauty spectrum
    Bool_t fIPEffCombinedSamples; // flag to combine two different samples

    Int_t fNbDimensions;          // Number of dimensions for the correction
    Int_t fNEvents[20];           // Number of Events
    Int_t fNMCEvents;             // Number of MC Events
    Int_t fNMCbgEvents[20];       // Number of BG MC Events
    Int_t fStepMC;                // MC step (for unfolding)
    Int_t fStepTrue;              // MC step of the final spectrum
    Int_t fStepData;              // Data Step (various applications)
    Int_t fStepBeforeCutsV0;      // Before cuts V0
    Int_t fStepAfterCutsV0;       // After cuts V0
    Int_t fStepGuessedUnfolding;  // Step for first guessed unfolding
    Int_t fNumberOfIterations;    // Number of iterations
    Int_t fNRandomIter;           // Number of random iterations
    Chargetype_t fChargeChoosen;         // Select positive or negative electrons

    Double_t fEtaRange[2];        // Eta range 
    Double_t fEtaRangeNorm[2];    // Eta range used in the normalization

    Int_t fNCentralityBinAtTheEnd;// Number of centrality class at the end
    Int_t fLowBoundaryCentralityBinAtTheEnd[20];  // Boundary of the bins
    Int_t fHighBoundaryCentralityBinAtTheEnd[20];  // Boundary of the bins
    Int_t fTestCentralityLow;                      // To test one bin in centrality only
    Int_t fTestCentralityHigh;                     // To test one bin in centrality only
    Bool_t fFillMoreCorrelationMatrix;             // For low stats to have reasonable errors

    THnSparseF *fHadronEffbyIPcut;// container for hadron efficiency by IP cut
    TH1D *fEfficiencyCharmSigD[kCentrality]; // charm IP cut eff from signal enhanced MC
    TH1D *fEfficiencyBeautySigD[kCentrality]; // beauty IP cut eff from signal enhanced MC
    TH1D *fEfficiencyBeautySigesdD[kCentrality]; // beauty IP cut eff from signal enhanced MC for esd
    TH1D *fConversionEff[kCentrality];     // conversion IP cut eff
    TH1D *fNonHFEEff[kCentrality];         // nonhfe IP cut eff
    TH1D *fCharmEff[kCentrality];          // charm IP cut eff
    TH1D *fBeautyEff[kCentrality];         // beauty IP cut eff
    TH1D *fConversionEffbgc;      // conversion IP cut eff
    TH1D *fNonHFEEffbgc;          // nonhfe IP cut eff
    TH1D *fBSpectrum2ndMethod;             // beauty spectrum for 2nd method
    const char *fkBeauty2ndMethodfilename;      // name of file, which contains beauty spectrum for 2ndmethod
    Char_t fBeamType;             // beamtype; default -1; pp =0; PbPb=1
    Bool_t fEtaSyst;              // pp 2.76 TeV (= kTRUE) or 7 TeV (= kFALSE)


    Int_t fDebugLevel;            // Debug Level
    Bool_t fWriteToFile;           // Write plots to eps files
    Bool_t fUnfoldBG;             // flag to unfold backgroud

    ClassDef(AliHFEspectrum, 1) 
};
#endif

