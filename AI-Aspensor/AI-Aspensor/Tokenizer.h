#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include "Rule.h"
using namespace std;

class Tokenizer
{
private:
   map<char, short> operators;

   void InitialiseOperators();

public:
   Tokenizer();

   ~Tokenizer();

   string InfixToPostfix(const string& infixExpression);

   map<char, string> LoadFacts(const string& filename);

   vector<Rule> LoadRules(const string& filename);

   set<char> LoadKnowledge(const string& filename);

   set<char> LoadTargets(const string& filename);

   char LoadReverseTarget(const string& filename);

};


#endif //TOKENIZER_H