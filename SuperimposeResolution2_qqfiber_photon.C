//RPD resolution plot maker
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TRandom3.h"
#include "TMultiGraph.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TLine.h"

using namespace std;
vector < string > fileNames;
vector < string > legend;
vector < int > color;
vector < int > style;
vector < TFile* > file;
vector < TGraphErrors* > graph;
vector < int > neutron_bins;

TStyle* AtlasStyle();

void LoadNeutronBins(){
  neutron_bins.push_back(20); neutron_bins.push_back(25); neutron_bins.push_back(30);
  neutron_bins.push_back(35); neutron_bins.push_back(41);
}

void SuperimposeResolution(){

  LoadNeutronBins();


  fileNames.push_back("../Data/qq_fiber/ResoFiles/ResoEvents10_20.root");  legend.push_back("p_{T}^{spec} = 10-20 MeV");  color.push_back(kBlue + 1); style.push_back(23);//33
  fileNames.push_back("../Data/qq_fiber/ResoFiles/ResoEvents20_30.root");  legend.push_back("p_{T}^{spec} = 20-30 MeV");  color.push_back(kGreen + 3); style.push_back(20);//21
  fileNames.push_back("../Data/qq_fiber/ResoFiles/ResoEvents30_40.root");  legend.push_back("p_{T}^{spec} = 30-40 MeV");  color.push_back(kOrange+ 2); style.push_back(21);//22
  fileNames.push_back("../Data/qq_fiber/ResoFiles/ResoEvents40_50.root");  legend.push_back("p_{T}^{spec} = 40-50 MeV");  color.push_back(kRed+1); style.push_back(22);//20

  // fileNames.push_back("ResoEvents10_20.root");  legend.push_back("p_{T}^{spec} = 10-20 MeV");  color.push_back(kBlue + 1); style.push_back(23);//33
  // fileNames.push_back("ResoEvents20_30.root");  legend.push_back("p_{T}^{spec} = 20-30 MeV");  color.push_back(kGreen + 3); style.push_back(20);//21
  // fileNames.push_back("ResoEvents30_40.root");  legend.push_back("p_{T}^{spec} = 30-40 MeV");  color.push_back(kOrange+ 2); style.push_back(21);//22
  // fileNames.push_back("ResoEvents40_50.root");  legend.push_back("p_{T}^{spec} = 40-50 MeV");  color.push_back(kRed+1); style.push_back(22);//20
  //


  // fileNames.push_back("../Data/qq_fiber/ResoFiles/ResoEvents5_15.root");  legend.push_back("p_{T}^{spec} = 5-15 MeV");  color.push_back(kGreen + 3); style.push_back(33);
  // fileNames.push_back("../Data/qq_fiber/ResoFiles/ResoEvents15_25.root");  legend.push_back("p_{T}^{spec} = 15-25 MeV");  color.push_back(kBlue + 1); style.push_back(21);
  // fileNames.push_back("../Data/qq_fiber/ResoFiles/ResoEvents25_35.root");  legend.push_back("p_{T}^{spec} = 25-35 MeV");  color.push_back(kViolet+1); style.push_back(22);
  // fileNames.push_back("../Data/qq_fiber/ResoFiles/ResoEvents35_50.root");  legend.push_back("p_{T}^{spec} = 35-50 MeV");  color.push_back(kRed+1); style.push_back(20);
  //

  for(int i = 0; i < fileNames.size(); i++){
    file.push_back(TFile::Open(fileNames.at(i).c_str()));
    graph.push_back((TGraphErrors*)file.back()->Get("Resolution_vs_n"));
    graph.back()->SetMarkerColor(color.at(i));
    graph.back()->SetLineColor(color.at(i));
    graph.back()->SetMarkerStyle(style.at(i));
  }

  //Fitting and plotting
  TStyle* atlasStyle = AtlasStyle();
  gROOT->ForceStyle();
  atlasStyle->cd();
  double ll = 0.0;
  double ul = 1.0;
  TCanvas* c1 = new TCanvas ("c1","c1",600,600);
  TLegend* leg = new TLegend(0.2,0.7,0.95,0.875);
  leg->SetNColumns(2);
  leg->SetTextSize(0.035);
  c1->cd();
  gPad->SetTopMargin(0.125);
  gPad->SetLeftMargin(0.2);
  gPad->SetRightMargin(0.05);
    for(int i = 0; i < (int)graph.size(); i++){
      if( i == 0){
        graph.at(i)->GetYaxis()->SetTitle("#sqrt{<cos (#psi_{1}^{A} - #psi_{1}^{C})>}");
        graph.at(i)->GetYaxis()->SetRangeUser(ll,ul);
        graph.at(i)->GetYaxis()->SetLimits(ll,ul);
        graph.at(i)->GetYaxis()->SetTitleSize(0.055);
        graph.at(i)->GetYaxis()->SetTitleOffset(1.45);
        graph.at(i)->GetXaxis()->SetTitleOffset(1.);
        graph.at(i)->GetXaxis()->SetRangeUser(19.5,40.5);
        graph.at(i)->GetXaxis()->SetLimits(19.5,40.5);
        gPad->SetLeftMargin(0.2);
        graph.at(i)->GetXaxis()->SetTitle("N_{neutrons}");
        graph.at(i)->Draw("AP");
      }
      else graph.at(i)->Draw("PSAME");
      leg->AddEntry(graph.at(i),legend.at(i).c_str(), "p");
    }//Loop on graphs
    TLatex l;
    l.SetTextSize(0.04);
    l.SetTextFont(42);

    l.DrawLatexNDC(0.2,0.94,"JZCaP HL-ZDC");
    l.DrawLatexNDC(0.2,0.9,"Simulation");

    TLine *l1 = new TLine();
    l1->SetLineStyle(kDashed);
    l1->SetLineColor(kBlack);
    for(int j = 1; j < neutron_bins.size()-1; j++)
      l1->DrawLine(neutron_bins.at(j),ll, neutron_bins.at(j), ul );
    leg->Draw();
}


TStyle* AtlasStyle()
{
  // TStyle *atlasStyle = new TStyle("ATLAS","Atlas style");
  TStyle *atlasStyle = gROOT->GetStyle("Plain");

  // use plain black on white colors
  Int_t icol=0; // WHITE
  atlasStyle->SetFrameBorderMode(icol);
  atlasStyle->SetFrameFillColor(icol);
  atlasStyle->SetCanvasBorderMode(icol);
  atlasStyle->SetCanvasColor(icol);
  atlasStyle->SetPadBorderMode(icol);
  atlasStyle->SetPadColor(icol);
  atlasStyle->SetStatColor(icol);
  //atlasStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  atlasStyle->SetPaperSize(20,26);

  // set margin sizes
  atlasStyle->SetPadTopMargin(0.05);
  atlasStyle->SetPadRightMargin(0.1);
  atlasStyle->SetPadBottomMargin(0.16);
  atlasStyle->SetPadLeftMargin(0.2);

  // set title offsets (for axis label)
  atlasStyle->SetTitleXOffset(1.4);
  atlasStyle->SetTitleYOffset(1.);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t tsize=0.05;
  atlasStyle->SetTextFont(font);

  atlasStyle->SetTextSize(tsize);
  atlasStyle->SetLabelFont(font,"x");
  atlasStyle->SetTitleFont(font,"x");
  atlasStyle->SetLabelFont(font,"y");
  atlasStyle->SetTitleFont(font,"y");
  atlasStyle->SetLabelFont(font,"z");
  atlasStyle->SetTitleFont(font,"z");

  atlasStyle->SetLabelSize(tsize,"x");
  atlasStyle->SetTitleSize(tsize,"x");
  atlasStyle->SetLabelSize(tsize,"y");
  atlasStyle->SetTitleSize(tsize,"y");
  atlasStyle->SetLabelSize(tsize,"z");
  atlasStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  atlasStyle->SetMarkerStyle(10);
  atlasStyle->SetMarkerSize(3);
  atlasStyle->SetHistLineWidth(2.);
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars
  //atlasStyle->SetErrorX(0.001);
  // get rid of error bar caps
  atlasStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  atlasStyle->SetOptTitle(0);
  atlasStyle->SetOptStat("rme");
  atlasStyle->SetOptFit(1);

  // put tick marks on top and RHS of plots
  atlasStyle->SetPadTickX(1);
  atlasStyle->SetPadTickY(1);

  return atlasStyle;

}
