#include "tokens.hpp"
#include <iostream>
int main()
{
    int token;
    std::cout << "YOU CHANGED TOKENS.HPP: YYLENG: INT -> UNSIGNED LONG ";
   while ((token = yylex())) {
        std::cout << "token: " << token <<" lexema: " << yytext <<std::endl;
    }
    return 0;
}
