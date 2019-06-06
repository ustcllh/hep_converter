#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TTree.h"

class Event;
int main(int argc, char *argv[]);

class Event
{
public:
   double E[12];
   int ERand;
   double EWeight;
   string N[2];
   string U[2];
   double C[2];
   double H[13];
   double F[9];
   double V[9];
   int PCount;
   double P[12][10000];
public:
   Event() {}
   ~Event() {}
   void Clean()
   {
      for(int i = 0; i < 12; i++)   E[i] = 0;
      for(int i = 0; i < 2; i++)    N[i] = "";
      for(int i = 0; i < 2; i++)    U[i] = "";
      for(int i = 0; i < 2; i++)    C[i] = 0;
      for(int i = 0; i < 13; i++)   H[i] = 0;
      for(int i = 0; i < 9; i++)    F[i] = 0;
      for(int i = 0; i < 9; i++)    V[i] = 0;
      PCount = 0;
      for(int i = 0; i < 12; i++)
         for(int j = 0; j < 10000; j++)
            P[i][j] = 0;
   }
   void DoBranch(TTree &T)
   {
      T.Branch("E", &E, "E[12]/D");
      T.Branch("ERand", &ERand, "ERand/I");
      T.Branch("EWeight", &EWeight, "EWeight/D");
      T.Branch("C", &C, "C[2]/D");
      T.Branch("H", &H, "H[13]/D");
      T.Branch("F", &F, "F[9]/D");
      T.Branch("V", &V, "V[9]/D");
      T.Branch("NP", &PCount, "NP/I");
      T.Branch("PIndex", &P[0], "PIndex[NP]/I");
      T.Branch("PID", &P[1], "PID[NP]/D");
      T.Branch("PX", &P[2], "PX[NP]/D");
      T.Branch("PY", &P[3], "PY[NP]/D");
      T.Branch("PZ", &P[4], "PZ[NP]/D");
      T.Branch("PE", &P[5], "PE[NP]/D");
      T.Branch("PM", &P[6], "PM[NP]/D");
      T.Branch("PStatus", &P[7], "PStatus[NP]/D");
      T.Branch("PUnknown1", &P[8], "PUnknown1[NP]/D");
      T.Branch("PUnknown2", &P[9], "PUnknown2[NP]/D");
      T.Branch("PUnknown3", &P[10], "PUnknown3[NP]/D");
      T.Branch("PUnknown4", &P[11], "PUnknown4[NP]/D");
   }
};

int main(int argc, char *argv[])
{
   string InputFile = "example.hepmc";
   string OutputFile = "example.root";

   if(argc != 3 && argc != 2)
   {
      cerr << "Usage: " << argv[0] << " [InputFile.hepmc] OutputFile.root" << endl;
      return -1;
   }

   bool ReadFromSTD = false;

   if(argc == 3)
   {
      ReadFromSTD = false;
      InputFile = argv[1];
      OutputFile = argv[2];
   }
   else
   {
      ReadFromSTD = true;
      OutputFile = argv[1];
   }

   TFile File(OutputFile.c_str(), "RECREATE");

   TTree Tree("Tree", "Tree");

   ifstream filein(InputFile.c_str());

   istream &in = (ReadFromSTD ? cin : filein);
   
   bool BeforeFirstEvent = true;
   Event E;

   E.DoBranch(Tree);

   while(in)
   {
      char ch[1024];
      ch[0] = '\0';
      in.getline(ch, 1023, '\n');

      if(ch[0] == '\0')
         continue;

      stringstream str(ch);
      string Type;
      str >> Type;

      if(BeforeFirstEvent == true && Type != "E")
         continue;

      if(Type == "E")
      {
         if(BeforeFirstEvent == false)
            Tree.Fill();
         BeforeFirstEvent = false;
         E.Clean();

         for(int i = 0; i < 10; i++)
            str >> E.E[i];

         str >> E.E[10];
         if(E.E[10] != 0)
            for(int i = 0; i < E.E[10]; i++)
               str >> E.ERand;
         str >> E.E[11];
         if(E.E[11] != 0)
         {
            E.EWeight = 1;
            double Weight = 1;
            for(int i = 0; i < E.E[11]; i++)
            {
               str >> Weight;
               E.EWeight = E.EWeight * Weight;
            }
         }
         else
            E.EWeight = 1;
      }
      if(Type == "N")   for(int i = 0; i < 2; i++)    str >> E.N[i];
      if(Type == "U")   for(int i = 0; i < 2; i++)    str >> E.U[i];
      if(Type == "C")   for(int i = 0; i < 2; i++)    str >> E.C[i];
      if(Type == "H")   for(int i = 0; i < 13; i++)   str >> E.H[i];
      if(Type == "F")   for(int i = 0; i < 9; i++)    str >> E.F[i];
      if(Type == "V")   for(int i = 0; i < 9; i++)    str >> E.V[i];
      if(Type == "P")
      {
         for(int i = 0; i < 12; i++)
            str >> E.P[i][E.PCount];
         E.PCount = E.PCount + 1;
      }
   }

   filein.close();

   Tree.Write();

   File.Close();

   return 0;
}




