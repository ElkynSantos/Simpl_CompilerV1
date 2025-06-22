#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include "SimplLexer.hpp"
#include "tokens.hpp"
#include "SimplParse.hpp"

int main(int argc, char* argv[]) {

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
      std::cout << "Error al abrir el archivo: " << argv[1] << std::endl;
      return 0;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string input = buffer.str();
  
  SimplLexer lexer(input);
  SimplParser parser(lexer);

  if(parser.parse()){
    std::cout<<"===============================SUCCESSFULL====================================" << std::endl;
  }
  return 0;
}
