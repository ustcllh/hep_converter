#include <fstream>
#include <iostream>
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

   Tree->SetBranchAddress("EWeight", &Weight);
   Tree->SetBranchAddress("NP", &NP);
   Tree->SetBranchAddress("PID", &PID);
   Tree->SetBranchAddress("PX", &PX);
   Tree->SetBranchAddress("PY", &PY);
   Tree->SetBranchAddress("PZ", &PZ);
   Tree->SetBranchAddress("PM", &PM);
   Tree->SetBranchAddress("PStatus", &PStatus);

   int EntryCount = Tree->GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      Tree->GetEntry(iE);

      out << "# event " << iE << endl;
      out << "weight " << Weight << endl;
      for(int i = 0; i < NP; i++)
      {
         if(PStatus[i] < 1.1 && PStatus[i] > 0.9)   // 1
            out << PX[i] << " " << PY[i] << " " << PZ[i] << " " << PM[i] << " " << PID[i] << " " << 0 << endl;
         if(PStatus[i] < 4.1 && PStatus[i] > 3.9)   // 4
            out << PX[i] << " " << PY[i] << " " << PZ[i] << " " << PM[i] << " " << PID[i] << " " << 0 << endl;
         if(PStatus[i] > 2.9 && PStatus[i] < 3.1)   // 3
            out << PX[i] << " " << PY[i] << " " << PZ[i] << " " << PM[i] << " " << PID[i] << " " << -1 << endl;
      }
      out << "end" << endl;
   }

   File.Close();
   out.close();

   return 0;
}




