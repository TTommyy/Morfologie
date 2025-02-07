// Tomasz Koczar
// 15.04.2022AD
// Zadanie H
// Kurs "Programowanie 2"

#ifndef __MORFOLOGIA_CPP__
#define __MORFOLOGIA_CPP__

#include "Morfologia.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>

/*Test jawny zwraca:
Oryginalna bitmapa:
{
 {
  {0,0,0,0,0},
  {1,0,1,0,0},
  {0,0,1,1,0},
  {0,0,0,0,0}
 },
 {
  {0,0,0,0,0},
  {0,1,0,0,0},
  {0,1,1,1,0},
  {0,0,0,0,0}
 },
 {
  {0,0,0,0,0},
  {0,1,1,0,0},
  {0,1,1,1,0},
  {0,0,0,0,0}
 }
}

Przeksztalcona bitmapa:
{
 {
  {0,0,0,0,0},
  {0,0,1,1,0},
  {0,1,1,1,1},
  {0,0,1,1,0}
 },
 {
  {0,0,1,0,0},
  {0,1,1,1,0},
  {1,1,1,1,1},
  {0,1,1,1,0}
 },
 {
  {0,1,1,0,0},
  {1,1,1,1,0},
  {1,1,1,1,1},
  {0,1,1,1,0}
 }
}
Co jest zgodne z wyjsciem oczekiwanym.
*/



/*Do reprezentowania wsplrzednych*/
class Trojka{
    
public:
    unsigned x,y,z;
    Trojka(){};
    Trojka(unsigned x, unsigned y, unsigned z): x{x},y{y},z{z}{}
};

/** Klasa reprezentujaca bitmapy 3D.*/
class BitmapaExt : public Bitmapa{
   /*--------------Pola-----------------*/

   /**Pola zawierajace rozmiar bitmapy*/
   unsigned rangeX,rangeY,rangeZ;

   /**Pole reperzetujace Bitmape.
    * Kazdy bit moze byc bialy = false = 0.
    * Badz czarny = true != 0*/
   bool* bitmap;

   /*--------------Metody----------------*/
public:
   /**Konstruktor.
    * Przyjmuje rozmar x,y,z, jako parametry.
    * Tworzy biala bitmape.
    * */
    BitmapaExt(unsigned x, unsigned y, unsigned z): rangeX{x},rangeY{y},rangeZ{z}{
        /*bitmap = new bool[x*y*z]; // Tworzy tablice bitow o rozmiarze x*y*z.
        memset(bitmap,0,x*y*z *(sizeof(bool)));// na bialy*/
        bitmap = (bool*)calloc(x*y*z, sizeof(bool));
    }

    /**Konstruktory kopujacy*/
    BitmapaExt(const BitmapaExt& bm):rangeX{bm.rangeX} ,rangeY{bm.rangeY},rangeZ{bm.rangeZ}{
        bitmap = (bool*) malloc(rangeX*rangeY*rangeZ*sizeof(bool));
        memcpy(bitmap,bm.bitmap,sizeof(bool)*(rangeX*rangeY*rangeZ));
    }
    
    BitmapaExt& operator=(const BitmapaExt& bm){
    if(this!=&bm){
            rangeX=bm.rangeX ;rangeY=bm.rangeY;rangeZ=bm.rangeZ;//nowy rozmiar
            bitmap = (bool*) realloc(bitmap,rangeX*rangeY*rangeZ*sizeof(bool));
            memcpy(bitmap,bm.bitmap,sizeof(bool)*(rangeX*rangeY*rangeZ));//kopijujemy
        }
        return *this;
    }
    

    /**Destruktor*/
    ~BitmapaExt() override{ delete[] bitmap; }

    /**Getery dla rozmiaru*/
    unsigned sx() const override{
        return rangeX;
    }

    unsigned sy() const override{
        return rangeY;
    }

    unsigned sz() const override{
        return rangeZ;
    }

    /*Akcesory*/
    /**Akcesor do referencji*/
    bool& operator()(unsigned x, unsigned y, unsigned z) override{
        return bitmap[x*(rangeY*rangeZ)+(y*rangeZ)+z];
    }
    /**Akcesor do kopi*/
    bool operator()(unsigned x, unsigned y, unsigned z)const override{
        return bitmap[x*(rangeY*rangeZ)+(y*rangeZ)+z];
    }

};

/*Znajduje sasiadow bitu, ich liczbe przkazuje w parametrze
* Bit jest sasiadem jeszeli styka sie sciana oraz nie wykarcza poza zakres
*/
    void otoczenie(const Bitmapa& bm,unsigned x , unsigned y , unsigned z,int& liczbaSasaidow,Trojka* otoczenie){
        liczbaSasaidow = 0;
        if(x>0){
            otoczenie[liczbaSasaidow++] = Trojka(x-1,y,z); 
        }
        if(x+1<bm.sx()) { 
            otoczenie[liczbaSasaidow++] = Trojka(x+1,y,z); 
        }

        if(y>0){
            otoczenie[liczbaSasaidow++] = Trojka(x,y-1,z);
        } 
        if(y+1<bm.sy()) {
            otoczenie[liczbaSasaidow++] = Trojka(x,y+1,z);
        } 
        
        if(z>0) {
            otoczenie[liczbaSasaidow++] = Trojka(x,y,z-1);
        }
        if(z+1<bm.sz()){
            otoczenie[liczbaSasaidow++] = Trojka(x,y,z+1);
        }
    } 


/**Operator wyjscia dla klasy  BitMapa*/
std::ostream &operator <<( std::ostream& ostream, const  Bitmapa& bitmapa) {
    unsigned RX = bitmapa.sx(), RY = bitmapa.sy(), RZ = bitmapa.sz();
    ostream << "{\n";
    for( unsigned rX = 0; rX < RX; rX++ ){// wypisujemy dwuwymiarowe bloki
        ostream << " {\n";
        for( unsigned rY = 0; rY < RY; rY++ ){//wypsujemy wiersze w blokach
            ostream << "  {";
            for(unsigned rZ = 0; rZ < RZ; rZ++ ){//wypisujemy wiersze
                ostream << bitmapa(rX,rY,rZ);
                if(rZ+1<RZ) ostream << ",";
            }
            ostream << "}";
            if(rY+1<RY) ostream << ",";
            ostream <<"\n";
        }
        ostream << " }";
        if(rX+1<RX) ostream << ",";
        ostream <<"\n";
    }
    ostream << "}";
    return ostream;
}

/*-----------------------Potomkowie klasy Przeksztalecnie-----------------*/

/**Zamiana pikseli czarnych na biale i viceversa*/
class Inwersja:public Przeksztalcenie{
public:
   
    //funkcja przeksztalcajaca
    void przeksztalc(Bitmapa& bm) override{
        unsigned rX = bm.sx(), rY = bm.sy(), rZ = bm.sz();
        for(unsigned x = 0; x<rX; x++){//dla kazego punktu
            for(unsigned y=0; y<rY; y++){
                for(unsigned z=0; z<rZ; z++){
                    bm(x,y,z) = !bm(x,y,z);//zamiana koloru
                }
            }
        }
    }

};

/**Przez piksel brzegowy obrazu rozumiemy piksel czarny, 
 * którego jednym z sąsiadów jest piksel biały.
 *  Operacja erozji polega na tym, że najpierw 
 * lokalizowane są wszystkie piksele brzegowe w danym obrazie, 
 * a następnie ich kolor jest ustawiany na biały.*/
class Erozja:public Przeksztalcenie{
public:
    
    /**Funkcja Przeksztalcajaca*/
    void przeksztalc(Bitmapa& bm) override{
       /*Znajdumemy punkty podantne na erozje*/
        unsigned rX = bm.sx(), rY = bm.sy() , rZ = bm.sz();
        std::vector<Trojka> doErozji;
        doErozji.reserve(rX*rY*rZ);
        Trojka sasiedzi[6];
        int liczbaSasiadow ;
        for(unsigned x=0; x<rX ;x++){//Dla kazdego punktu
            for(unsigned y=0; y<rY; y++){
                for(unsigned z=0; z<rZ ; z++){
                    if(bm(x,y,z)){//Jesli jest czarny
                        otoczenie(bm,x,y,z,liczbaSasiadow,sasiedzi);//Znajedumy sasiadow
                        for(int sasiad = 0; sasiad < liczbaSasiadow; sasiad++){//Dla kazdego sasiada
                            if(!bm(sasiedzi[sasiad].x,sasiedzi[sasiad].y,sasiedzi[sasiad].z)){
                            // Jesli jakis jest bialy
                                doErozji.push_back(Trojka(x,y,z));//To do erozji
                                break;//Idzemy dalej
                            }
                        }
                    }
                }
            }
        }
        for(auto i : doErozji){//Dla kazdego punktu podatnego na erozje
            bm(i.x,i.y,i.z) = false; //Zmieniamy kolor na bialy
        }

    }
};

/**Operacja odwrotna do erozji*/
class Dylatacja:public Przeksztalcenie{
public:

   /**Funkcja Przeksztalcajaca*/
    void przeksztalc(Bitmapa& bm) override{
       /*Znajdumemy punkty podantne na erozje*/
        unsigned rX = bm.sx(), rY = bm.sy() , rZ = bm.sz();
        std::vector<Trojka> doDylatacji;
        doDylatacji.reserve(rX*rY*rZ);
        Trojka sasiedzi[6];
        int liczbaSasiadow ;
        for(unsigned x=0; x<rX ;x++){//Dla kazdego punktu
            for(unsigned y=0; y<rY; y++){
                for(unsigned z=0; z< rZ; z++){
                    if(!bm(x,y,z)){//Jesli jest bialy
                        otoczenie(bm,x,y,z,liczbaSasiadow,sasiedzi);//Znajedumy sasiadow
                        for(int sasiad = 0; sasiad < liczbaSasiadow; sasiad++){//Dla kazdego sasiada
                            if(bm(sasiedzi[sasiad].x,sasiedzi[sasiad].y,sasiedzi[sasiad].z)){
                                //Jesli jakis jest czarny
                                doDylatacji.push_back(Trojka(x,y,z));//To do dylatacji
                                break;//Idzemy dalej
                            }
                        }
                    }
                }
            }
        }
        for(auto i : doDylatacji){//Dla kazdego punktu do zmiany
            bm(i.x,i.y,i.z) = true; //Zmien kolor na czarny
        }

    }
};

/**Ustawia wszytko na bialy*/
class Zerowanie:public Przeksztalcenie{
public:
    /**Funkcja Przeksztalcajaca*/
    void przeksztalc(Bitmapa& bm) override{
        unsigned rX = bm.sx(), rY = bm.sy(), rZ = bm.sz();
        for(unsigned x = 0; x<rX; x++)
            for(unsigned y = 0; y<rY; y++)
                for(unsigned z = 0; z<rZ; z++)  
                    bm(x,y,z) = false;//Zmieniemy kolor na bialy
    }
};

/**dla każdego piksela p obrazu sprawdzamy liczbę 
 * sąsiadujących z nim pikseli białych i czarnych. 
 * Jeśli ma on więcej niż trzech sąsiadów w kolorze k, to nowym kolorem 
 * piksela p jest kolor k. W przeciwnym razie kolor piksela nie zmienia się.
 *  Sąsiadów rozważamy w oryginalnym obrazie, nie w obrazie częściowo uśrednionym!
 * */
class Usrednianie:public Przeksztalcenie{
public:
    /**Funkcja Przeksztalcajaca*/
    void przeksztalc(Bitmapa&bm) override{
        unsigned rX = bm.sx(), rY = bm.sy(), rZ = bm.sz();
        std::vector<Trojka> doZmiany;
        doZmiany.reserve(rX*rY*rZ);
        Trojka sasiedzi[6];
        int iloscSasiadow,bialy,czarny;
       
        for(unsigned x = 0; x<rX; x++){//Dla kazego punktu
            for(unsigned y=0; y<rY; y++){
                for(unsigned z=0; z<rZ; z++){
                    otoczenie(bm,x,y,z,iloscSasiadow,sasiedzi);//Znajdz sasiadow
                    bialy = czarny = 0;//Wyzeruj licznki
                    for(int i=0;i <iloscSasiadow; i++){//Policz kazdego sasiada
                        bm(sasiedzi[i].x,sasiedzi[i].y,sasiedzi[i].z)? czarny++:bialy++;
                    }
                    if(bialy>3 && bm(x,y,z)) {
                        doZmiany.push_back(Trojka(x,y,z));
                    } // Jesli zachodzi taka potrzeba przekaz go do zmiany koloru
                    else if(czarny>3 &&  !bm(x,y,z)) doZmiany.push_back(Trojka(x,y,z));
                }
            }
        }
        for(auto i : doZmiany){//Dla kazdego punktu do zmiany
            bm(i.x,i.y,i.z) = !bm(i.x,i.y,i.z); //Zmien kolor na przeciwny
        }
    }
};


/*Klasa skladajaca przeksztalcenia*/
class ZlozeniePrzeksztalcen:public Przeksztalcenie{

std::vector<Przeksztalcenie*> tabelaPrzeksztalcen;

public:
    ZlozeniePrzeksztalcen(){}
    
    void dodajPrzeksztalcenie(Przeksztalcenie* p){
        tabelaPrzeksztalcen.push_back(p);
    }

    void przeksztalc(Bitmapa& bm)override{
        for(auto p:tabelaPrzeksztalcen)
            p->przeksztalc(bm);
        
    }
};

/*KONIEC*/
#endif