
#include <iostream>
#include <cmath>

char *encode (char *plaintext, unsigned long key);
void ascii_armour (int count, unsigned char Sr[], char print[]);
char *decode (char *ciphertext, unsigned long key);

char *encode (char *plaintext, unsigned long key){
    
    int i=0;
    int j=0;
    unsigned char S[256];
    
    
    for(int v=0; v<256; v++){
        S[v] = v;
    }
    
    for(int b=0; b<256; b++){
           
        int k= i % 64;
        j= (j + S[i] + ((key>>k)&1))%256;
            
        int Temp;
        Temp=S[i];
        S[i]=S[j];
        S[j]=Temp;
        i=(i+1)%256;
        }
      
  /*
    for(int i=0; i<256; i++){
        std::cout<< (int) S[i]<<std::endl;
    }
  */
    
   //XOR plaintext
    int count=0;
    while (plaintext[count] != '\0'){
        count++;
    }
    //std::cout << "plain count " << count << std::endl;

    int nulls = count % 4 ? 4-(count%4) : 0;
    unsigned int size=(5 * (unsigned int)ceil(count/4.0))+1;
    char* print = new char [size];
    unsigned char *intermediate = new unsigned char [count + nulls+1];
    
  //  std::cout<<"The size is"<<size;

   if(count % 4 != 0) {
    
    if(count % 4 == 1){
        intermediate[count + 0] = '\0';
        intermediate[count + 1] = '\0';
        intermediate[count + 2] = '\0';

    }
    else if(count % 4 == 2){
        intermediate[count+0]= '\0';
        intermediate[count+1]= '\0';
    }
    else if(count % 4 ==3){
        intermediate[count]='\0';
        }
    }
    
    for(int a=0; a<count; a++){
        intermediate[a]= plaintext[a];
    }
    
    //std::cout<<count+x;
    for(int a=0; a<count+nulls; a++){
        
        i=(i+1)%256;
        j=(j+S[i])%256;
        
        int temp1;
        temp1=S[i];
        S[i]=S[j];
        S[j]=temp1;
        
        int r = (S[i]+S[j])%256;
        unsigned char R=S[r];
        
        intermediate[a] = static_cast <unsigned char> (intermediate[a])^R;
        
        }
       intermediate[count+nulls]= '\0';
    ascii_armour(count+nulls, intermediate, print);
    delete [] intermediate;
    print[size-1] = '\0';
    return print;
}

    //start of ASCII Armour
void ascii_armour (int count, unsigned char S[],  char print[]){

    unsigned int isfour[4] {};
    unsigned int bin{0};
    for(int x=0; x<(count/4); x++){
        isfour[0] = S[x*4] ;
        isfour[1] = S[x*4+1] ;
        isfour[2] = S[x*4+2] ;
        isfour[3] = S[x*4+3] ;
        
        bin = (isfour[0]<<24) + (isfour[1]<<16) + (isfour[2]<<8) + (isfour[3]);
       // std::cout<<bin;
        int ct = 0;
        while(ct< 5){
            print[5*x + 4 - ct] = ((bin % 85)+33);
            bin/=85;
            
            ct++;
        }
    }
    }

//start of decoding ciphertext
char *decode (char *ciphertext, unsigned long key){

    //removing ASCII armour
    
    
    unsigned int newcount=0;
    while(ciphertext[newcount] != '\0'){
        newcount++;
        }
    //std::cout << "decode newcount " << newcount << std::endl;
    int out_size = (int)(ceil(newcount/5.0))*4+1;
    //std::cout << "decode out size " << out_size << std::endl;
    
    unsigned int *newbin = new unsigned int[(int)(newcount/5.0)];
    unsigned char *newfour= new unsigned char[out_size];
    
        for(int i=0; i<newcount; i+=5){
        ciphertext[i] = ciphertext[i] - 33;
        ciphertext[i+1] = ciphertext[i+1] - 33;
        ciphertext[i+2] = ciphertext[i+2] - 33;
        ciphertext[i+3] = ciphertext[i+3] - 33;
        ciphertext[i+4] = ciphertext[i+4] - 33;
            
        newbin[i/5] = ciphertext[i+4]*1+ ciphertext[i+3]*85 + ciphertext[i+2]*85*85 + ciphertext[i+1]*85*85*85 + ciphertext[i]*85*85*85*85;
            //std::cout<<newbin;
        }
    
    for(int x=0,y=0; x<out_size; x+=4,++y){
        newfour[x+3]= newbin[y] & 255;
        newfour[x+2]= (newbin[y]>>8) & 255;
        newfour[x+1]= (newbin[y]>>16) & 255;
        newfour[x] = (newbin[y]>>24) & 255;
    }
    newfour[out_size-1]='\0';
   //resrambling state array
    int i=0;
    int j=0;
    unsigned char S[256];
    
    
    for(int v=0; v<256; v++){
        S[v] = v;
    }
    
    for(int b=0; b<256; b++){
           
        int k= i % 64;
        j= (j + S[i] + ((key>>k)&1))%256;
            
        int Temp;
        Temp=S[i];
        S[i]=S[j];
        S[j]=Temp;
        i=(i+1)%256;
    }
    
    char *newprint = new char[out_size];
    
    for(int a=0; a<out_size-1; a++){
        
        i=(i+1)%256;
        j=(j+S[i])%256;
        
        int temp1;
        temp1=S[i];
        S[i]=S[j];
        S[j]=temp1;
        
        int r = (S[i]+S[j])%256;
        unsigned char R=S[r];
        
        newprint[a]= (char)R^static_cast < char> (newfour[a]);
        
        
        /*for(int x=0; x<(newcount/5)*4; x++){
       
            }*/
       
        }
    delete [] newbin;
    delete [] newfour;
    newprint[out_size-1]='\0';
    return newprint;
    }


 
#ifndef MARMOSET_TESTING
int main() {
    char plaintext[]{"Hello World!"};
    unsigned long key= 51323;
    std::cout<<plaintext<<std::endl;
    char *ciphertext = encode(plaintext,key);
    std::cout<<ciphertext<<std::endl;
    
    //char [1000000]={}
    
    
    //char *decoded { decode(ciphertext, key)};
   // std::cout<<decoded<<std::endl;
    
   // ciphertext = encode(decoded,key);
   // std::cout<<ciphertext<<std::endl;
    //decoded = {decode(ciphertext, key)};
    //std::cout<<decoded<<std::endl;

    //std::cout<< encode(plaintext, key)<<std::endl;
    //encode(plaintext, key);
    decode(ciphertext, key);
    return 0;
    }
#endif
