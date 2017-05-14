#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;

bool DoubleEquals(double a, double b, double epsilon = 1e-3) {
   return fabs(a - b) < epsilon;
}

map<string, vector<double>> LoadFuzzySet(const string& fileName);

map<string, vector<pair<pair<string, string>, string>>> LoadRules(const string& fileName);

double Fuzzy(const double& val, const vector <double>& set);

map<string, double> MamdaniInference(
   map<string, vector<double>>& inputA,
   map<string, vector<double>>& inputB,
   const map<string, vector<pair<pair<string, string>, string>>>& rules,
   const map<string, vector<double>>& output,
   const double& inputValueA,
   const double& inputValueB);

double Deffuzy(map<string, vector<double>>& output, map<string, double>& fuzzies);

void Tests();

int main()
{
   auto inA = 0.0;  // umiditate
   auto inB = 35.0; // temperatura

   auto inputA = LoadFuzzySet("a.txt");
   auto inputB = LoadFuzzySet("b.txt");
   auto output = LoadFuzzySet("out.txt");
   auto rules = LoadRules("rules.txt");
   auto fuzzies = MamdaniInference(inputA, inputB, rules, output, inA, inB);

   cout << Deffuzy(output, fuzzies) << "\n";

   Tests();

   return 0;
}

void Tests()
{

   auto inputA = LoadFuzzySet("a.txt");
   auto inputB = LoadFuzzySet("b.txt");
   auto output = LoadFuzzySet("out.txt");
   auto rules = LoadRules("rules.txt");
   
   auto inA = 0.0;  // umiditate
   auto inB = 35.0; // temperatura

   auto fuzzies = MamdaniInference(inputA, inputB, rules, output, inA, inB);
   assert(DoubleEquals(Deffuzy(output, fuzzies),83.6667));


   inA = 65.0;  // umiditate
   inB = 18.0; // temperatura

   fuzzies = MamdaniInference(inputA, inputB, rules, output, inA, inB);
   assert(DoubleEquals(Deffuzy(output, fuzzies),47.46860));

   inA = 30.0;  // umiditate
   inB = 0.0; // temperatura

   fuzzies = MamdaniInference(inputA, inputB, rules, output, inA, inB);
   assert(DoubleEquals(Deffuzy(output, fuzzies), 54.0702));

   inA = 100.0;  // umiditate
   inB = 0.0; // temperatura

   fuzzies = MamdaniInference(inputA, inputB, rules, output, inA, inB);
   assert(DoubleEquals(Deffuzy(output, fuzzies), 16.333));

   inA = 0.0;  // umiditate
   inB = 100.0; // temperatura

   fuzzies = MamdaniInference(inputA, inputB, rules, output, inA, inB);
   assert(DoubleEquals(Deffuzy(output, fuzzies), 50));

}

map<string, vector<double>> LoadFuzzySet(const string& fileName)
{
   ifstream f{fileName};
   string str;
   map<string, vector<double>> db;
   string current;

   while (f >> str)
   {
      if (isalpha(str[0]))
      {
         current = str;
      }
      else
      {
         db[current].push_back(stod(str));
      }
   }

   f.close();
   return db;
}

map<string, vector<pair<pair<string, string>, string>>>LoadRules(const string& fileName)
{
   ifstream f{ fileName };
   map<string, vector<pair<pair<string, string>, string>>>rules;
   string condA, condB, rule, type;

   while(f>>condA>>type>>condB>>rule)
   {
      rules[rule].push_back(make_pair(make_pair(condA, condB), type));
   }

   f.close();
   return rules;
}

double Fuzzy(const double& val, const vector <double>& set)
{

   double f1, f2;

   f1 = (val - set[0]) / (set[1] - set[0]);

   // triangle
   if (set.size() == 3)
   {
      f2 = (set[2] - val) / (set[2] - set[1]);
   }
   // trapeze
   else 
   {
      f2 = (set[3] - val) / (set[3] - set[2]);
   }

   return max(0.0, min(min(f1, f2), 1.0));
}

map<string, double> MamdaniInference(
   map<string, vector<double>>& inputA,
   map<string, vector<double>>& inputB,
   const map<string, vector<pair<pair<string, string>, string>>>& rules,
   const map<string, vector<double>>& output,
   const double& inputValueA,
   const double& inputValueB)
{
   map<string, double> fuzzies;

   for (auto & out : output)
   {
      fuzzies[out.first] = numeric_limits<double>::min();
   }

   for(auto& rule : rules)
   {
      for(auto& inputAInputB: rule.second)
      {
         auto type = inputAInputB.second;
         auto fKey = inputAInputB.first.first;
         auto sKey = inputAInputB.first.second;
         auto res = 0.0;

         if(type == "AND")
            res = min(Fuzzy(inputValueA, inputA[fKey]), Fuzzy(inputValueB, inputB[sKey]));
         else 
            res = max(Fuzzy(inputValueA, inputA[fKey]), Fuzzy(inputValueB, inputB[sKey]));

         fuzzies[rule.first] = max(fuzzies[rule.first], res);
      }
   }

   return fuzzies;

}

double Deffuzy( map<string, vector<double>>& output, map<string, double>& fuzzies)
{

   auto leftMostPoint = numeric_limits<double>::max();
   auto rightMostPoint = numeric_limits<double>::min();
   auto step = 0.5;
   auto sum = 0.0;
   auto fuzzySum = 0.0;

   for (auto& out : output)
   {
      leftMostPoint = min(leftMostPoint, out.second[0]);
      rightMostPoint = max(rightMostPoint, out.second[out.second.size() - 1]);
   }

   for(auto i = leftMostPoint; i <= rightMostPoint; i+=step)
   {
      auto bestFuzzy = numeric_limits<double>::min();
      for(auto& keyValuePair : output)
      {
         auto localFuzzy = min(Fuzzy(i, output[keyValuePair.first]), fuzzies[keyValuePair.first]);
         bestFuzzy = max(localFuzzy, bestFuzzy);
      }
      sum += (i * bestFuzzy);
      fuzzySum += bestFuzzy;
   }

   return sum / fuzzySum;

}