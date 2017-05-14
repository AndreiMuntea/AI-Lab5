#include <iostream>
#include <string>
#include "Tokenizer.h"
#include <stack>

using namespace std;

char GetSolution(const set<char>& target, const set<char>& knowledge);

string ForwardInference();

vector<string> ReverseInference();

bool ValidInformation(const Rule& rule, const set<char>& knowledge);

bool DoWork(char op, bool op1, bool op2);

bool CheckReverse(const char& target, set<char>& knowledge, const vector<Rule>& rules, map<char, bool>& visited);

int main()
{
   cout << ForwardInference() << "\n--------------------------------------------\n";
   auto rev = ReverseInference();
   for(auto& s : rev)
   {
      cout << s << "\n";
   }

   return 0;
}


string ForwardInference()
{
   Tokenizer tokenizer;

   auto facts = tokenizer.LoadFacts("facts.txt");
   auto rules = tokenizer.LoadRules("rules.txt");
   auto target = tokenizer.LoadTargets("targets.txt");
   auto knowledge = tokenizer.LoadKnowledge("knowledge.txt");
   auto goOn = true;

   while (goOn && GetSolution(target, knowledge) == NULL)
   {
      goOn = false;
      for (auto& rule : rules)
      {
         if (ValidInformation(rule, knowledge))
         {
            // we update the knowledge base so we can go on
            goOn = true;
            knowledge.insert(rule.GetResult());
         }
      }
   }
   
   auto sol = GetSolution(target, knowledge);
   if (NULL == sol)
      return "No solution found!";

   return facts[sol];
}


vector<string> ReverseInference()
{
   Tokenizer tokenizer;

   auto facts = tokenizer.LoadFacts("facts.txt");
   auto rules = tokenizer.LoadRules("rules.txt");
   auto target = tokenizer.LoadReverseTarget("revtarget.txt");
   auto knowledge = tokenizer.LoadKnowledge("knowledge.txt");
   map<char, bool> visited;
   vector<string> res;

  auto ok = CheckReverse(target, knowledge, rules, visited);

  if (!ok)
     res.push_back("Not enough information!");
  else
     for(auto& k : knowledge)
     {
        res.push_back(facts[k]);
     }

  return res;
}


char GetSolution(const set<char>& target, const set<char>& knowledge)
{
   for (auto c : target)
   {
      if (knowledge.find(c) != knowledge.end())
      {
         return c;
      }
   }
   return NULL;
}

bool ValidInformation(const Rule& rule, const set<char>& knowledge)
{
   // we already know this so we ignore it
   if (knowledge.find(rule.GetResult()) != knowledge.end())
      return false;

   // evaluate expression
   // we store a stack of bools (if an operand is in our knowledge base, we store true, else we store false
   stack<bool> stck;

   for(auto c : rule.GetExpression())
   {
      if (isupper(c) && isalpha(c))
         stck.push(knowledge.find(c) != knowledge.end());
      else
      {
         auto secondOperand = stck.top(); 
         stck.pop();
         auto firstOperand = stck.top();
         stck.pop();
         stck.push(DoWork(c, firstOperand, secondOperand));
      }
   }

   return stck.top();
}

bool DoWork(char op, bool op1, bool op2)
{
   switch (op)
   {
   case'^':
      return op1 && op2;
   default:
      return op1 || op2;
   }
}

bool CheckReverse(const char& target, set<char>& knowledge, const vector<Rule>& rules, map<char, bool>& visited)
{
   visited[target] = true;

   for(auto& rule : rules)
   {
      if(target == rule.GetResult())
      {
         if(ValidInformation(rule, knowledge))
         {
            knowledge.insert(target);
            return true;
         }
         stack<bool> stck;

         for (auto c : rule.GetExpression())
         {
            if (isupper(c) && isalpha(c))
            {
               if (visited[c]) stck.push(false);
               else if (knowledge.find(c) != knowledge.end()) stck.push(true);
               else stck.push(CheckReverse(c, knowledge, rules, visited));
            }
            else
            {
               auto secondOperand = stck.top();
               stck.pop();
               auto firstOperand = stck.top();
               stck.pop();
               stck.push(DoWork(c, firstOperand, secondOperand));
            }
         }

         if(stck.top())
         {
            knowledge.insert(target);
            return true;
         }
      }
   }

   return false;
}
