//
// Created by Nathan Houwaart on 2019-02-19.
//
#include <iostream>
#include <string>

// Voorbeeld voor het aanroepen van de code:
// ls | ./opdracht0 7
// De output van ls wordt de input voor deze code
// 7 geeft aan dat de letters 7 plaatsen worden geshift


std::string cecarCode(std::string & stringtotranslate, const char & shifts){
    for(int i=0;i< stringtotranslate.size();i++){
        if(isalpha(stringtotranslate[i]) && stringtotranslate != " "){
            for(int x=0; x<shifts;x++){
                if(stringtotranslate[i]=='z'){
                    stringtotranslate[i] = 'a';
                } else if(stringtotranslate[i] == 'Z'){
                    stringtotranslate[i] = 'A';
                } else{
                    stringtotranslate[i] ++;
                }
            }
        }
    }
    return stringtotranslate;
}

int main(int argc, char *argv[]){
    std::string s;
    if (argc !=2){
        std::cerr << "Je moet wel een argument meegeven" << std::endl;
        return -1;
    }
    int shifts = std::atoi(argv[1]);
    while(true){
        std::getline(std::cin, s);
        if (std::cin.eof()){
            return 0;
        }
        std::cout << s << "\t\t" << cecarCode(s, shifts) << std::endl;
    }
}