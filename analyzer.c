#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main(){
  FILE *fileLex,*file;
    file = fopen("code.psi", "r");
    fileLex = fopen("code.lex", "w");

    char keyword[18][10]={"break","goto","case","char","const","continue","do","else","enum","float","for","if","int","long","record","return","static","while"};
    
    
    int i,j,str_size,m;
    i=j=str_size=m=0;
    char word[20];
    char str_const[100];
    
    if(file!=NULL){

      char chr = getc(file);
      
      while(!feof(file)){
        
        if(isspace(chr)){ // Bosluk ve diger benzeri turleri yok sayilacaktir.
            chr = getc(file);
        }
        //identifier ve keyword kontrolu
        if((isalpha(chr))){ //  Okunan karakter alfabetik mi
          int word_len=0;
          bool is_keyword=true;
          while(isalpha(chr) || isdigit(chr) || chr == 95){ // identifer yazimina uygun mu
          // 20den uzun olan kabul edilmeyecektir.
            if(word_len > 20){
                printf("Identifier names can't be longer than 20 characters!");
                return 1;
            }
            if(isdigit(chr) || chr == 95){
              is_keyword=false;
            }
            //
            if(isupper(chr)) chr = tolower(chr);
            word[word_len++] = chr;
            chr = getc(file);
          }
        word[word_len] = '\0'; // kelimenin okunur olmasi icin sonuna null eklendi.

        if(is_keyword){ // kelimenin hepsi alfabetik karakterlerden olusuyorsa keyword kontrolu yapiliyor.
          for(j=0;j<18;j++){
            if(strcmp(word, keyword[j]) == 0){ // Eslesme varsa
                fputs("Keyword(", fileLex);
                is_keyword=true;
              for(j=0; j< word_len;j++){
                fputc(word[j], fileLex);
              }
              fputs(")\n", fileLex);
              break;
            }
            else{
              is_keyword=false;
            }
          }
        }
            if(!is_keyword){ // Keyword degilse identifer
              fputs("Identifier(", fileLex);
              for(j=0;j < word_len;j++){
                  fputc(word[j], fileLex);
              }
              fputs(")\n", fileLex);
            }
            
        }
        // operatorler
        if(chr == 43){ // + operatoru icin + veya ++ olma durumu
            chr = getc(file);
            if(chr == 43){
                fputs("Operator(++)\n", fileLex);
            }
            else{
                fputs("Operator(+)\n", fileLex);
            }
        }
        if(chr == 45){ // - operatoru icin - veya -- olma durumu
          chr = getc(file);
          if(chr == 45){
            fputs("Operator(--)\n", fileLex);
          }
          else{
            fputs("Operator(-)\n", fileLex);
          }
        }
        // yildiz karakteri '*' kontrol
        if(chr == 42){
          chr=getc(file);
          if(chr!=41){
            fputs("Operator(*)\n", fileLex);
          }
        }
        // := esitlik operatoru kontrol
        if(chr == 58){
            chr = getc(file);
            if(chr != 61){
              printf("Undefined input error...");
              return 1;
            }
            else{
              fputs("Operator(:=)\n",fileLex);
            }
        }
        // Bolme isareti kontrolu '/'
        if(chr == 47){ 
          chr = getc(file);
          fputs("Operator(/)\n", fileLex);
        }
        //string olma durumu
        if(chr == 34){ // cift tırnak kontrolu 
            chr = getc(file);
            str_const[str_size++]=chr;
            // string olarak tanimlanabilmesi icin cift tirnak gelene kadar okuyoruz.
            while(chr != 34 && chr != EOF){
              chr = getc(file);
              str_const[str_size++]=chr;
            }
            str_const[str_size]='\0';
            if(chr == 34){ //Tirnak kapandigi zaman
              fputs("StrConst(", fileLex);
              for(m=0;m < (strlen(str_const)-1);m++){
                  fputc(str_const[m], fileLex);
              }
              fputs(")\n", fileLex);
            }
            // tirnak kapanmadan dosyanin sonuna gelinirse:
            else if(chr == EOF){
                printf("Double quote error!");
                return 1;
            }
        }
        // integer kontrolu
        if(isdigit(chr)){ 
        int int_len=0;
          fputs("IntConst(", fileLex);
          while (isdigit(chr)){
            if(int_len > 10){
              printf("Integers can't be longer than 10 characters!");
              return 1;
            }              
            fputc(chr, fileLex);
            chr = getc(file);
            int_len++;
          }
            fputs(")\n", fileLex);
        }

        // ';' kontrolu 
        if(chr == 59){ 
            fputs("EndOfLine\n",fileLex);
        }
        
        // Parantez karakterleri bloklari
        if(chr == 40){
          chr = getc(file);
          // comment olup olmama durumu kontrol ediliyor
          
          if(chr == 42){ 
            // '(' sol parantezden sonra * yildiz geliyor mu
            while(!feof(file)){
              chr = getc(file);
              if(chr == 42){ // yildiz geldiyse sonraki karakter ')' ise yorum sonu olur.
                chr = getc(file);
                if(chr == 41) // ikinci yildizdan sonra ')' geldi mi
                chr = getc(file);
                break;
              }
              if(chr == EOF){
                printf("Comment error.");
                return 1;
              }
            }
          }
          // yorum blogu degilse duz sol parantez olarak okunacaktir.
          else{
            fputs("LeftParantheses\n", fileLex);
          }
        }
        // sag parantez
        if(chr == 41){
          fputs("RightParantheses\n", fileLex);
        }
        // sol koseli parantez
        if(chr == 91){
           fputs("LeftSquareBracket\n", fileLex);
        }
        // sag koseli parantez
        if(chr == 93){
           fputs("RightSquareBracket\n", fileLex);
        }
        // sol suslu parantez
        if(chr == 123){
           fputs("LeftCurlyBracket\n", fileLex);
        }
        // sag suslu parantez
        if(chr == 125){
           fputs("RightCurlyBracket\n", fileLex);
        }
        chr = getc(file);
      }
    }
    else{
        printf("File not found...");
        return 1;
    }
    
    fclose(file);
    fclose(fileLex);
    fflush(stdin);
    return 0;
}
