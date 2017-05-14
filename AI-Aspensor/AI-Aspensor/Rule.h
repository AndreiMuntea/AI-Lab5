#ifndef RULE_H
#define RULE_H
#include <string>
using namespace std;


class Rule
{
private:
   string expression;
   char result;


public:


   Rule(const string& expression, char result)
      : expression(expression),
        result(result)
   {
   }

   string GetExpression() const
   {
      return expression;
   }

   char GetResult() const
   {
      return result;
   }


   void SetExpression(const string& expression)
   {
      this->expression = expression;
   }

   void SetResult(char result)
   {
      this->result = result;
   }

};

#endif //RULE_H