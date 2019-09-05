#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

#include "TFile.h"
#include "TTree.h"

#define MAX 20000

int main(int argc, char *argv[])
{
   if(argc != 3)
   {
      cerr << "Usage: " << argv[0] << " InputRootFile OutputFileName" << endl;
      return -1;
   }

   ofstream out(argv[2]);
   TFile File(argv[1]);

   TTree *Tree = (TTree *)File.Get("Tree");

   int NP;
   double Weight;
   double PID[MAX], PX[MAX], PY[MAX], PZ[MAX], PM[MAX], PStatus[MAX];
   double H[13];

   Tree->SetBranchAddress("EWeight", &Weight);
   Tree->SetBranchAddress("NP", &NP);
   Tree->SetBranchAddress("PID", &PID);
   Tree->SetBranchAddress("PX", &PX);
   Tree->SetBranchAddress("PY", &PY);
   Tree->SetBranchAddress("PZ", &PZ);
   Tree->SetBranchAddress("PM", &PM);
   Tree->SetBranchAddress("PStatus", &PStatus);
   Tree->SetBranchAddress("H", &H);

   int EntryCount = Tree->GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree->GetEntry(iE);

      out << "# event " << iE << endl;
      out << "weight " << Weight << endl;

      // H line in private jewel for partons
      // double px1 = H[0]/100. * cos(H[2]/100.);
      // double py1 = H[0]/100. * sin(H[2]/100.);
      // double pz1 = H[0]/100. * sinh(H[8]/100.);
      // double pm1 = H[1]/100.;
      // int pid1 = H[6]==1? 21 : 0;	// is gluon
      // int vtx1 = 1;
      //
      // double px2 = H[3]/100. * cos(H[5]/100.);
      // double py2 = H[3]/100. * sin(H[5]/100.);
      // double pz2 = H[3]/100. * sinh(H[12]/100.);
      // double pm2 = H[4]/100.;
      // int pid2 = H[7]==1? 21 : 0;	// is gluon
      // int vtx2 = 1;
      //
      //
      // out << px1 << " " << py1 << " " << pz1 << " " << pm1 << " " << pid1 << " " << vtx1 << endl;
      // out << px2 << " " << py2 << " " << pz2 << " " << pm2 << " " << pid2 << " " << vtx2 << endl;

      for(int i = 0; i < NP; i++)
      {

         if(PStatus[i] < 23.1 && PStatus[i] > 22.9)   // 23: parton
            out << PX[i] << " " << PY[i] << " " << PZ[i] << " " << PM[i] << " " << PID[i] << " " << -1 << endl;
         if(PStatus[i] < 1.1 && PStatus[i] > 0.9)   // 1: final state particle
            out << PX[i] << " " << PY[i] << " " << PZ[i] << " " << PM[i] << " " << PID[i] << " " << 0 << endl;
         if(PStatus[i] < 4.1 && PStatus[i] > 3.9)   // 4: incoming beam particle
            out << PX[i] << " " << PY[i] << " " << PZ[i] << " " << PM[i] << " " << PID[i] << " " << 0 << endl;
         if(PStatus[i] > 2.9 && PStatus[i] < 3.1)   // 3: documentation entry
            out << PX[i] << " " << PY[i] << " " << PZ[i] << " " << PM[i] << " " << PID[i] << " " << -1 << endl;

      }
      out << "end" << endl;
   }

   File.Close();
   out.close();

   return 0;
}
