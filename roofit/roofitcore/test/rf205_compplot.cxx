//////////////////////////////////////////////////////////////////////////
//
// 'ADDITION AND CONVOLUTION' RooFit tutorial macro #205
//
// Options for plotting components of composite p.d.f.s.
//
//
//
// 07/2008 - Wouter Verkerke
//
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
using namespace RooFit ;


class TestBasic205 : public RooFitTestUnit
{
public:
  TestBasic205(TFile* refFile, bool writeRef, Int_t verbose) : RooFitTestUnit("Component plotting variations",refFile,writeRef,verbose) {} ;
  bool testCode() {

    // S e t u p   c o m p o s i t e    p d f
    // --------------------------------------

    // Declare observable x
    RooRealVar x("x","x",0,10) ;

    // Create two Gaussian PDFs g1(x,mean1,sigma) anf g2(x,mean2,sigma) and their parameters
    RooRealVar mean("mean","mean of gaussians",5) ;
    RooRealVar sigma1("sigma1","width of gaussians",0.5) ;
    RooRealVar sigma2("sigma2","width of gaussians",1) ;
    RooGaussian sig1("sig1","Signal component 1",x,mean,sigma1) ;
    RooGaussian sig2("sig2","Signal component 2",x,mean,sigma2) ;

    // Sum the signal components into a composite signal p.d.f.
    RooRealVar sig1frac("sig1frac","fraction of component 1 in signal",0.8,0.,1.) ;
    RooAddPdf sig("sig","Signal",RooArgList(sig1,sig2),sig1frac) ;

    // Build Chebychev polynomial p.d.f.
    RooRealVar a0("a0","a0",0.5,0.,1.) ;
    RooRealVar a1("a1","a1",-0.2,0.,1.) ;
    RooChebychev bkg1("bkg1","Background 1",x,RooArgSet(a0,a1)) ;

    // Build expontential pdf
    RooRealVar alpha("alpha","alpha",-1) ;
    RooExponential bkg2("bkg2","Background 2",x,alpha) ;

    // Sum the background components into a composite background p.d.f.
    RooRealVar bkg1frac("sig1frac","fraction of component 1 in background",0.2,0.,1.) ;
    RooAddPdf bkg("bkg","Signal",RooArgList(bkg1,bkg2),sig1frac) ;

    // Sum the composite signal and background
    RooRealVar bkgfrac("bkgfrac","fraction of background",0.5,0.,1.) ;
    RooAddPdf  model("model","g1+g2+a",RooArgList(bkg,sig),bkgfrac) ;



    // S e t u p   b a s i c   p l o t   w i t h   d a t a   a n d   f u l l   p d f
    // ------------------------------------------------------------------------------

    // Generate a data sample of 1000 events in x from model
    RooDataSet *data = model.generate(x,1000) ;

    // Plot data and complete PDF overlaid
    RooPlot* xframe  = x.frame(Title("Component plotting of pdf=(sig1+sig2)+(bkg1+bkg2)")) ;
    data->plotOn(xframe) ;
    model.plotOn(xframe) ;

    // Clone xframe for use below
    RooPlot* xframe2 = x.frame(Title("Component plotting of pdf=(sig1+sig2)+(bkg1+bkg2)"),Name("xframe2")) ;


    // M a k e   c o m p o n e n t   b y   o b j e c t   r e f e r e n c e
    // --------------------------------------------------------------------

    // Plot single background component specified by object reference
    model.plotOn(xframe,Components(bkg),LineColor(kRed)) ;

    // Plot single background component specified by object reference
    model.plotOn(xframe,Components(bkg2),LineStyle(kDashed),LineColor(kRed)) ;

    // Plot multiple background components specified by object reference
    // Note that specified components may occur at any level in object tree
    // (e.g bkg is component of 'model' and 'sig2' is component 'sig')
    model.plotOn(xframe,Components(RooArgSet(bkg,sig2)),LineStyle(kDotted)) ;



    // M a k e   c o m p o n e n t   b y   n a m e  /   r e g e x p
    // ------------------------------------------------------------

    // Plot single background component specified by name
    model.plotOn(xframe2,Components("bkg"),LineColor(kCyan)) ;

    // Plot multiple background components specified by name
    model.plotOn(xframe2,Components("bkg1,sig2"),LineStyle(kDotted),LineColor(kCyan)) ;

    // Plot multiple background components specified by regular expression on name
    model.plotOn(xframe2,Components("sig*"),LineStyle(kDashed),LineColor(kCyan)) ;

    // Plot multiple background components specified by multiple regular expressions on name
    model.plotOn(xframe2,Components("bkg1,sig*"),LineStyle(kDashed),LineColor(kYellow),Invisible()) ;


    regPlot(xframe,"rf205_plot1") ;
    regPlot(xframe2,"rf205_plot2") ;

    delete data ;
    return true ;

  }

} ;
