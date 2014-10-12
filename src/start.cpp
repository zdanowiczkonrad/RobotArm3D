/* PROGRAM MANIPULATOR 1.0 *************************************************/
/* autor: Konrad Zdanowicz, nr albumu: 180394                              */
/* email: zdanowicz.konrad@gmail.com                                       */
/***************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

#include "lacze_do_gnuplota.hh"
#include "manipulator.hh"

using namespace std;

/* funkcja zamieniajaca stringa na inta ************************************/
/* za pomoca przekierowania strmienia wejsciowego i zczytania powodzenia   */
/* operacji wczytania ciagu do typu int. jezeli sie powiodlo, oznacza ze   */
/* na wejsciu jest int, jezeli nie, ja wejsciu jest znak                   */
/***************************************************************************/
bool string2int(const string &ciag, int &liczba)
{
  istringstream strmwyj(ciag);
  if (strmwyj>>liczba) return true;
  else return false;
}

/* main ********************************************************************/
/*=========================================================================*/
/***************************************************************************/

int main()
{
   /* glowne klasy programu  ***********************************************/
   PzG::LaczeDoGNUPlota  Lacze;              //konstrukcja lacza do gnuplota
   Manipulator M;                                 //konstrukcja manipulatora
   M.wstawLacze(&Lacze);             //wstawienie lacza do klasy manipulator
   /* ustawienia animacji obrotu ramienia **********************************/
   int czas_animacji=5000000;                //czas animacji w mikrosekundach
   int klatek=50;                                   //liczba klatek animacji
   float delta_kat;                  //czastkowy kat - obrot na jedna klatke
   float kat_poczatkowy;     //poczatkowy kat segmentu od ktorego sie obraca
   int delta_t;                           //czastkowy czas - na jedna klatke
   int i;                                              //pomocniczy iterator

   /* zmienne pomocnicze - obsluga menu ************************************/
   string in;            // wybor uzytkownika przechowywany jako ciag znakow
   int wybor;// liczba calkowita, potrzebna do weryfikowania, czy na wejsciu
               // zadana jest liczba. jezeli tak - ta zmienna ja przechowuje
   float l_float;    // pomocnicza do wczytywania liczb zmiennoprzecinkowych
   int l_int;                  //pomocnicza do wczytywania liczb calkowitych
  
   /* ustawienia lacza do gnuplota *****************************************/
   Lacze.ZmienTrybRys(PzG::TR_3D);
   Lacze.Inicjalizuj();

   Lacze.UstawZakresX(-20, 40);
   Lacze.UstawZakresY(-20, 40);
   Lacze.UstawZakresZ(-16, 25);


  Lacze.UstawRotacjeXZ(59,108); // Tutaj ustawiany jest widok
  //Lacze.UstawRotacjeXZ(90,0); // Tutaj ustawiany jest widok
  //Lacze.UstawRotacjeXZ(90,90); // Tutaj ustawiany jest widok
   /* ustawienia manipulatora - poczatkowe dane ****************************/
   M.dlugosc=10;                             //ustaw dlugosc domyslnie na 10
   M.zmienDlugoscListy(3);                     //ustaw liczbe segmentow na 3
   M.zmienKat(0,105);                              //domyslne wartosci katow
   M.zmienKat(1,-70);
   M.zmienKat(2,-60);
   M.obrocPodstawe(200);
   M.przeliczWspolrzedne();                           //przelicz wspolrzedne
   M.obrocPodstawe(0);
   M.interfejsUsera();                                       //wyswietl menu
   M.rysujUklad();                                         //wyswietla uklad

   /* interakcja z uzytkownikiem *******************************************/
   cin>>in;                                                  //wczytaj wybor
   while(in!="k")                     //dopoki nie bedzie klawisza konca "k"
   {
      if((string2int(in,wybor) && wybor<M.segmentow && wybor>-1)|| in=="r")
                                        //jezeli wybor jest liczba calkowita
      {
         if(in=="r")
         {
            cout<<"Wprowadz nowy kat obrotu: "<<flush;
            cin>>l_float;
         }
         else                                  
         {
            cout<<"Wprowadz q"<<wybor<<": "<<flush; 
            cin>>l_float;                     //wczytujemy nowa wartosc kata
         }


   /* ===== animacja obrotu ===============================================*/

            delta_t=czas_animacji/klatek;          //czas pojedynczej klatki
            if(in=="r")                        //wez kat poczatkowy segmentu 
              kat_poczatkowy=M.obrot_podstawy;
            else
              kat_poczatkowy=M.zwrocKat(wybor);  
            delta_kat=(l_float-kat_poczatkowy)/klatek; //oblicz kat na klatke

          /* petla animacji ---------------------------------------------- */
            for(i=1;i<=klatek;i++)
            {
               if(in=="r")  
                 M.obrocPodstawe(kat_poczatkowy+delta_kat*i);
               else
                 M.zmienKat(wybor,kat_poczatkowy+delta_kat*i);   //ustaw kat
               M.przeliczWspolrzedne();               //przelicz wspolrzedne
               usleep(delta_t);                     //i odczekaj czas klatki
               M.rysujUklad();
            }
         /* animacja-koniec ===============================================*/

         }
      
      else                       // gdy uzytkownik nie wpisal liczby, a znak
      {
         /* zmiana liczby segmentow =======================================*/
         if(in=="z")                               
         {
            cout<<"Wprowadz nowa liczbe segmentow: "<<flush;
            cin>>l_int;
            M.zmienDlugoscListy(l_int);
         }

         /* zmiana dlugosci segmentu ======================================*/
         else if(in=="l")
         {
            cout<<"Wprowadz nowa dlugosc: "<<flush;
            cin>>l_float;
            M.zmienDlugoscSegmentu(l_float);
         }

         /* dodaj segment =================================================*/
         else if(in=="+") M.dodajSegment();      
         /* usun segment ==================================================*/
         else if(in=="-") M.usunSegment();

      }
   /* przelicz wspolrzedne po operacjach ***********************************/
                                 //gdy uzytkownik nic nie zmienil, nie rysuj
      if(in!="?")
      {
         M.przeliczWspolrzedne(); 
         M.rysujUklad();
      }
      M.interfejsUsera();                                //wyswietlenie menu
      cin>>in;               //wez na wejscie znak i skocz na poczatek petli
   }

   return 0;
}

