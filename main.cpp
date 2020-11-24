#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

using namespace std;

// Global counters
int varAssignCount = 0;
int varReferenceCount = 0;
int currentPosition = 0;
int totalSize = 0;

// Token to check in Expr and Val
string exprToCheck;
string vartoCheck;

// Vector of input
vector<string> tokens;
vector<string> exprVector;

// Functions of the Grammar
bool Program();
bool Block();
bool StmtList();
bool MoreStmts();
bool Stmt();
bool Assign();
bool AssignEnd();
bool IfStmt();
bool ElseStmt();
bool WhileStmt();
bool TestExpr(string testExprStr);
bool Expr(string exprToCheck);
bool Var(string vartoCheck);
bool Digit(string digitToCheck);

void errorMessage(int currentLine, string errorType);
void ProgramEnd();
void PrintCurrentLine();

// Regex List to test
regex program("^\\s*(\\bprogram\\b)\\s*$");
regex blockStart("^\\s*(\\bbegin\\b)\\s*$");
regex blockEnd("^\\s*(\\bend\\b)\\s*$");
regex blockEndFinal("^\\s*(\\bend\\b)\\s*.$");
regex whilestmt("^\\s*\\b(\\while)\\b\\s*(.*)\\b(do)\\b$");
regex ifStmt("\\s*(\\bif\\b)\\s*[a|b|c]\\s*(<=)\\s*([0|1|2]|[a|b|c])\\s*(\\bthen\\b)\\s*");
regex elseStmt("^\\s*\\belse\\b\\s*$");
regex assign("\\s*[a|b|c]\\s*=\\s*(([\\+|\\*])\\s*(([0|1|2]|([a|b|c]))\\s*(([0|1|2])|([a|b|c])))\\s*|([a|b|c])|\\s*([0|1|2])\\s*);\\s*");
regex assignEnd("\\s*[a|b|c]\\s*=\\s*(([\\+|\\*])\\s*(([0|1|2]|([a|b|c]))\\s*(([0|1|2])|([a|b|c])))\\s*|([a|b|c])|\\s*([0|1|2])\\s*)");
regex testExpr("\\s*[a|b|c]\\s*<=\\s*(([\\+|\\*])\\s*(([0|1|2]|([a|b|c]))\\s*(([0|1|2])|([a|b|c])))\\s*|([a|b|c])|\\s*([0|1|2])\\s*)");
regex expr("(\\s*[\\+|\\*]\\s*((\\s*[0|1|2]\\s*)|(\\s*[a|b|c]\\s*))\\s*((\\s*[0|1|2]\\s*)|(\\s*[a|b|c]\\s*))\\s*|(\\s*[a|b|c]\\s*)|(\\s*[0|1|2]\\s*)\\s*)\\s*;?\\s*");
regex var("\\s*(\\b[a|b|c])\\b\\s*");
regex digit("\\s*[0|1|2]+\\s*");

int main(int argc, char *argv[])
{
  ifstream fp(argv[1]);
  string line;
  // Read input line by line and add to vector of tokens
  while(getline(fp, line))
  {
      tokens.push_back(line);
  }

//   for (string line; getline(cin, line);)
//   {
//     tokens.push_back(line);
//   }

  if (regex_match(tokens[6], expr))
  {
    cout << "Working" << endl;
  }

  totalSize = tokens.size();

  Program();

  return 0;
}

// Recursive check to see if program syntax is valid
bool Program()
{
  if (regex_match(tokens[0], program))
  {
    PrintCurrentLine();
    currentPosition++;
    Block();
  }
  else
  {
    errorMessage(1, "Program");
  }

  return true;
}

// Recursive check to see if all parts of the block are valid
bool Block()
{
  if (regex_match(tokens[currentPosition], blockStart))
  {
    PrintCurrentLine();
    currentPosition++;
    StmtList();
  }
  else if (regex_match(tokens[currentPosition], blockEnd))
  {
    PrintCurrentLine();
    currentPosition++;
    StmtList();
  }
  else if (currentPosition + 1 == totalSize)
  {
    PrintCurrentLine();
    cout << "=====================================================" << endl;
    cout << "Program is valid" << endl;
    cout << "=====================================================" << endl;
    ProgramEnd();
  }
  else if (regex_match(tokens[currentPosition], blockEndFinal))
  {
    if (regex_match(tokens[currentPosition + 1], program))
    {
      currentPosition++;
      Program();
    }
    else
    {
      errorMessage(currentPosition, "Invalid Termination");
      return false;
    }
  }
  else if (regex_match(tokens[currentPosition], program))
  {
    Program();
  }
  else
  {
    errorMessage(currentPosition, "Block");
  }
  return true;
}

// Recursive check to see if all parts of all Statements are valid
bool StmtList()
{
  Stmt();
  MoreStmts();
  return true;
}

// Recursive check to see if all parts of all Statements are valid
bool MoreStmts()
{
  StmtList();
  return true;
}

// Recursive check to see if all parts of the Statement is valid
bool Stmt()
{
  string currentToken = tokens[currentPosition];
  if (regex_match(currentToken, assign))
  {
    Assign();
  }
  else if (regex_match(currentToken, assignEnd))
  {
    AssignEnd();
  }
  else if (regex_match(currentToken, ifStmt))
  {
    IfStmt();
  }
  else if (regex_match(currentToken, elseStmt))
  {
    ElseStmt();
  }
  else if (regex_match(currentToken, whilestmt))
  {
    WhileStmt();
  }
  else if (regex_match(currentToken, blockStart))
  {
    Block();
  }
  else if (regex_match(currentToken, blockEnd))
  {
    Block();
  }
  else if (regex_match(currentToken, blockEndFinal))
  {
    Block();
  }
  else
  {
    errorMessage(currentPosition, "Stmt");
  }
  return true;
}

// Recursive check to see if all parts of the assignment statement ending are valid
bool AssignEnd()
{
  if (regex_match(tokens[currentPosition], assignEnd))
  {
    PrintCurrentLine();
    if (regex_match(tokens[currentPosition + 1], assign) || regex_match(tokens[currentPosition + 1], assignEnd))
    {
      errorMessage(currentPosition, "End of Assignments");
    }
    else
    {
      char *tempString = strdup(tokens[currentPosition].c_str());
      vartoCheck = strtok(tempString, "=");
      free(tempString);
      Var(vartoCheck);

      size_t pos = tokens[currentPosition].find("=");
      exprToCheck = tokens[currentPosition].substr(pos + 1);
      Expr(exprToCheck);

      varAssignCount++;
      currentPosition++;
      return true;
    }
  }
  else
  {
    errorMessage(currentPosition, "End of Assignments");
  }
  return true;
}

// Recursive check to see if all parts of the assignment statement are valid
bool Assign()
{
  if (regex_match(tokens[currentPosition], assign))
  {
    PrintCurrentLine();
    char *tempString = strdup(tokens[currentPosition].c_str());
    vartoCheck = strtok(tempString, "=");
    free(tempString);
    Var(vartoCheck);

    size_t pos = tokens[currentPosition].find("=");
    exprToCheck = tokens[currentPosition].substr(pos + 1);
    Expr(exprToCheck);

    varAssignCount++;
    currentPosition++;
    return true;
  }
  else
  {
    errorMessage(currentPosition, "Assign");
  }
  return true;
}

// Recursive check to see if all parts of the if conditional are valid
bool IfStmt()
{
  if (regex_match(tokens[currentPosition], ifStmt))
  {
    PrintCurrentLine();
    size_t pos1 = tokens[currentPosition].find("if");
    size_t pos2 = tokens[currentPosition].find("then");
    string testExprStr = tokens[currentPosition].substr((pos1 + 2), (pos2 - (pos1 + 2)));
    TestExpr(testExprStr);

    currentPosition++;
    Stmt();
    return true;
  }
  else
  {
    errorMessage(currentPosition, "IfStmt");
  }
  return false;
}

// Regex check to see if a string matches the syntax for an else statement
bool ElseStmt()
{
  if (regex_match(tokens[currentPosition], elseStmt))
  {
    PrintCurrentLine();
    currentPosition++;
    return true;
  }
  else
  {
    errorMessage(currentPosition, "ElseStmt");
  }
  return false;
}

// Regex check to see if a string matches the syntax for a while statement
bool WhileStmt()
{
  if (regex_match(tokens[currentPosition], whilestmt))
  {
    PrintCurrentLine();
    size_t pos1 = tokens[currentPosition].find("while");
    size_t pos2 = tokens[currentPosition].find("do");
    string testExprStr = tokens[currentPosition].substr(pos1 + 5, ((pos2 - 1) - (pos1 + 5)));
    TestExpr(testExprStr);

    currentPosition++;
    Stmt();
    return true;
  }
  else
  {
    errorMessage(currentPosition, "WhileStmt");
  }
  return true;
}

// Regex check to see if a string matches the syntax for a TestExpr
bool TestExpr(string testExprStr)
{
  if (regex_match(testExprStr, testExpr))
  {
    size_t pos1 = testExprStr.find("<=");
    size_t pos2 = testExprStr.find("=");
    string vartoCheck = testExprStr.substr(0, pos1);
    Var(vartoCheck);

    size_t pos = testExprStr.find("=");
    exprToCheck = testExprStr.substr(pos + 1);
    Expr(exprToCheck);
    return true;
  }
  else
  {
    errorMessage(currentPosition, "TestExpr");
  }
  return true;
}

// Regex check to see if a string matches the syntax for an expression
bool Expr(string exprToCheck)
{
  if (regex_match(exprToCheck, expr))
  {
    string str = exprToCheck;
    str = str.substr(0, str.length());
    regex reg("\\s*");
    sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    sregex_token_iterator end;

    vector<string> exprVector(iter, end);
    for (int i = 0; i < exprVector.size(); i++)
    {
      if (regex_match(exprVector[i], var))
      {
        Var(exprVector[i]);
      }
      else if (regex_match(exprVector[i], digit))
      {
        Digit(exprVector[i]);
      }
      else
      {
        // Do Nothing
      }
    }
    return true;
  }
  else
  {
    errorMessage(currentPosition, "Expr");
  }
  return true;
}

// Regex check to see if a string matches the var syntax
bool Var(string varToCheck)
{
  if (regex_match(varToCheck, var))
  {
    varReferenceCount++;
    return true;
  }
  else
  {
    errorMessage(currentPosition, "Var is invalid");
  }
  return true;
}

// Regex check to see if a string matches a digit
bool Digit(string digitToCheck)
{
  if (regex_match(digitToCheck, digit))
  {
    return true;
  }
  else
  {
    errorMessage(currentPosition, "Digit is invalid");
  }
  return true;
}

void PrintCurrentLine()
{
  cout << tokens[currentPosition] << endl;
}

void errorMessage(int currentLine, string errorType)
{
  cout << "Invalid " << errorType << " on line " << currentLine + 1 << endl;
  exit(0);
}

void ProgramEnd()
{
  cout << "=====================================================" << endl;
  cout << "There were " << varAssignCount << " assignments" << endl;
  cout << "There were " << varReferenceCount << " references" << endl;
  cout << "=====================================================" << endl;
  exit(0);
}