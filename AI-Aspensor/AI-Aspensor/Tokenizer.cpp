#include "Tokenizer.h"
#include <stack>
#include <fstream>

Tokenizer::Tokenizer()
{
   InitialiseOperators();
}

Tokenizer::~Tokenizer()
{
}

void Tokenizer::InitialiseOperators()
{
   operators['('] = 1;
   operators['v'] = 3;
   operators['^'] = 5;
}


string Tokenizer::InfixToPostfix(const string& infixExpression)
{
   string res = "";
   stack<char> stck;

   for (auto c : infixExpression)
   {
      if (c == ' ') continue; // ignore whitespaces
      if (c == '(')
         stck.push(c);
      else if (isupper(c) && isalpha(c)) // letter
         res += c;
      else if (c == ')')
      {
         auto topToken = stck.top();
         stck.pop();
         while (topToken != '(')
         {
            res += topToken;
            topToken = stck.top();
            stck.pop();
         }
      }
      else // operator
      {
         while (!stck.empty() && operators[stck.top()] >= operators[c])
         {
            auto topToken = stck.top();
            stck.pop();
            res += topToken;
         }
         stck.push(c);
      }
   }

   while (!stck.empty())
   {
      auto topToken = stck.top();
      stck.pop();
      res += topToken;
   }

   return res;
}


map<char, string> Tokenizer::LoadFacts(const string& filename)
{
   map<char, string> facts;
   ifstream f{filename};
   string line;
   char c;

   while (f >> c)
   {
      getline(f, line);
      facts[c] = line.substr(1, line.size() - 1);
   }

   f.close();

   return facts;
}

vector<Rule> Tokenizer::LoadRules(const string& filename)
{
   vector<Rule> rules;
   ifstream f{filename};
   string line;

   while (getline(f, line))
   {
      auto start = line.find("daca") + string("daca").length();
      auto end = line.find("atunci") - 1;
      auto str = InfixToPostfix(line.substr(start, end - start + 1));

      auto endP = line.find("atunci") + string("atunci").length() + 1;
      rules.push_back(Rule{str, line[endP]});
   }

   f.close();
   return rules;
}

set<char> Tokenizer::LoadKnowledge(const string& filename)
{
   set<char> knowledge;
   ifstream f{filename};
   string line;

   getline(f, line);
   for (auto c : line)
   {
      if (c != ' ') knowledge.insert(c);
   }

   f.close();
   return knowledge;
}

set<char> Tokenizer::LoadTargets(const string& filename)
{
   set<char> target;
   ifstream f{filename};
   string line;

   getline(f, line);
   for (auto c : line)
   {
      if (c != ' ') target.insert(c);
   }

   f.close();
   return target;
}

char Tokenizer::LoadReverseTarget(const string& filename)
{

   ifstream f{ filename };
   char c;

   f >> c;

   f.close();
   return c;
}
