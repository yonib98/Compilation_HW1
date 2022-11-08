#include "tokens.hpp"
#include <iostream>
#include <vector>

bool checkQu(){
    int last_char = yytext[yyleng-1];
    if(yytext[yyleng-2] == '\\'){
        return false;
    }
    return last_char == '"';
}

bool checkHexa(int i){
    if (i+2>=yyleng){
        return false;
    }
    char first = yytext[i+1];
    char second = yytext[i+2];
    if(!(first == '0' || first == '1' || first == '2' || first == '3' || first == '4' || first == '5' || first == '6' || first == '7')){
        return false;
    }
    //may need to add lower case
    if(!(second == '0' || second == '1' || second == '2' || second == '3' \
        || second == '4' || second == '5' || second == '6' || second == '7' \
        || second == '8' || second == '9' || second == 'A' || second == 'B' \
        || second == 'C' || second == 'D' || second == 'E' || second == 'F' )){
        return false;
    }
    return true;
}

int checkEscape(){
    int i = 0;
    while(yytext[i]){
        if (yytext[i] == '\\'){
            if (!(yytext[i+1] == '\\' || yytext[i+1] == '\"' || yytext[i+1] == 'n' || yytext[i+1] == 't' || yytext[i+1] == '0' )){
                return i+1;
            }
        }
        i++;
    }
    return 0;
}

void printHexError(int i){
    bool has_one = i + 1 < yyleng - 1;
    bool has_two = i + 1 < yyleng - 2;
    std::cout << "Error undefined escape sequence x";
    if (has_one) {
        std::cout << yytext[i + 1];
    }
    if (has_two) {
        std::cout << yytext[i + 2];
    }
    std::cout << "\n";
    exit(0);
}


std::string processLexema(){
    int i = 0;
    std::string processed = "";
    std::string hex_string = "";
    while(i<yyleng-1){
        if(yytext[i]=='\\'){
            switch (yytext[i+1]){
                case 'n':
                    processed += '\n';
                    break;
                case 't':
                    processed += '\t';
                    break;
                case 'r':
                    processed += '\r';
                    break;
                case 'x':
                    hex_string = "";
                    if(!checkHexa(i+1)) {
                        printHexError(i + 1);
                    }else{
                        hex_string += yytext[i + 2];
                        hex_string += yytext[i + 3];
                        processed += (char) std::stoi(hex_string, 0, 16);
                        i += 2;
                    }
                    break;
                case '\\':
                    processed += '\\';
                    break;
                case '0':
                    processed += '\0';
                    break;
                case '\"':
                    processed+= '\"';
            }
            i++;
        }else{
            processed += yytext[i];
        }
        i++;
    }
    return processed;
}



void printLex(std::string lex, int token,std::vector<std::string> a_very_sad_array){
    char space = ' ';
    std::string val = (token != 26) ? lex : "//"; //Do not print comment
    std::cout << yylineno << space  << a_very_sad_array[token] << space << val << "\n";
}
char findIllegalEscape(){
    for (int i=0;i<yyleng-1;i++){
        if(yytext[i]=='\\'){
            return yytext[i+1];
        }
    }
    throw std::exception();
}
int main()
{
    std::vector<std::string> a_very_sad_array = {"","VOID", "INT", "BYTE", "B", "BOOL", "AND", "OR", "NOT", "TRUE", "FALSE", "RETURN", "IF", "ELSE", "WHILE",
                                                 "BREAK", "CONTINUE", "SC", "COMMA", "LPAREN", "RPAREN",
                                                 "LBRACE", "RBRACE", "ASSIGN", "RELOP", "BINOP", "COMMENT", "ID", "NUM", "STRING"};
    int token;
    // std::cout << "YOU CHANGED TOKENS.HPP: YYLENG: INT -> UNSIGNED LONG " << std::endl;
    while ((token = yylex())) {
        if(token==30){
           char illegal_escp =  findIllegalEscape();
            std::cout << "Error undefined escape sequence " << illegal_escp << "\n";
            exit(0);
        }
        if (token == 31){
            std::cout << "Error unclosed string\n";
            exit(0);
        }
        if(token == -2) {
            std::cout << "Error " << yytext[0] << "\n";
            exit(0);
        }
        else if(token == STRING){
            std::string lex = processLexema();
            printLex(lex, token, a_very_sad_array);
        }else {
            printLex(yytext, token, a_very_sad_array);
        }


    }
    return 0;
}
