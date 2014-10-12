#include <cmath>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>


#include "manipulator.hh"
#include "lacze_do_gnuplota.hh"

using namespace std;

/***************************************************************************/
/* program MANIPULATOR                                                     */
/* ======================================================================= */
/* opis dzialania                                                          */
/* ----------------------------------------------------------------------- */
/* program obslugujacy ramie manipulatora w trzech wymiarach. dlugosc      */
/* kazdego segmentu jest taka sama (uzytkownik moze ja dowolnie zmieniac). */
/* kat nachylenia miedzy ramionami jest dla kazdej pary ramion dowolny i   */
/* moze byc zmieniany w trakcie programu. liczba segmentow rowniez moze    */
/* byc modyfikowana.                                                       */
/*                                                                         */
/* w przyszlosci bedzie opcja wprowadzenia przeszkod w postaci wielo-      */
/* bokow i wykrywa kolizje miedzy wielobokami i ramionami manipulatora.    */
/*                                                                         */
/* program za kazda zmiana kata nachylenia lub dlugosci/liczby segmentow   */
/* a takze gdy wprowadzona zostanie przeszkoda, rysuje schematycznie       */
/* manipulator za pomoca programu 'gnuplot'                                */
/* ======================================================================= */
/* warunki PRE                                                             */
/* ----------------------------------------------------------------------- */
/* kazdy segment nalezy wprowadzic za pomoca jednej wspolrzednej - kata    */
/* "odchylenia" wzgl. poprzedniego segmentu - innymi slowy kata odchylenia */
/* od normalnej poprzedniego ramienia                                      */
/*                                                                         */
/* przeszkody wprowadzane sa za pomoca wspolrzednych bezwglednych, czyli   */
/* pary dwoch wspolrzednych ukladu kartezjanskiego. Manipulator umieszcza  */
/* sie w poczatku ukladu wspolrzednych, za kat pierwszego ramienia uznaje  */
/* sie natomiast nachylenie do osi OX                                      */
/*                                                                         */
/* gdy program wykryje kolizje jakiejkolwiek czesci manipulatora z prze-   */
/* szkoda (nawet gdy nastapi zetkniecie krawedzi) - nie wyswietlany jest   */
/* schematyczny obraz manipulatora, tylko wyswietlony jest blad.           */
/* ======================================================================= */
/* autor: Konrad Zdanowicz, 180394                                         */
/***************************************************************************/


/***************************************************************************/
/* funkcje pomocnicze                                                      */
/* ======================================================================= */
/* float deg2rad(float)   | przelicza katy na radiany                      */
/***************************************************************************/

float deg2rad(float stopnie) { return (stopnie/180)*PI; }


/***************************************************************************/
/* klasa Segment                                                           */
/* ======================================================================= */
/* opis klasy                                                              */
/* ----------------------------------------------------------------------- */
/* w tej klasie przechowywane beda informacje o pojedynczym segmencie,     */
/* czyli kat odchylenia od poprzedniego segmentu. dlugosc kazdego segmentu */
/* jest stala, dlatego te informacje bede przechowywal w Manipulatorze     */
/*                                                                         */
/* w klasie tej rowniez zawarte sa pola na wspolrzedne bezwgledne (karte-  */
/* zjanskie) konca segmentu (poczatek segmentu jest wyznaczony przez       */
/* koniec segmentu poprzedniego).                                          */
/*                                                                         */
/* metody klasy to te podstawowe, umozliwiajace zmiane kata i wyliczajace  */
/* wspolrzedne konca segmentu.                                             */
/*                                                                         */
/* wszystkie pola sa typu public, ze wzgledu na wygode stosowania.         */
/* ======================================================================= */
/* pola                                                                    */
/* ----------------------------------------------------------------------- */
/* float kat               | odchylenie od poprzedniego segmentu           */
/* float kat_ox            | kat nachylenia od osi OX - kat bezwzgledny    */
/* float dlugosc           | dlugosc ramienia segmentu                     */
/* float x                 | wspolrzedna x konca segmentu \ pomocnicze     */
/* float y                 | wspolrzedna y konca segmentu / do obliczen    */
/* float plotx             | wspolrzedna x konca segmentu - do rysowania   */
/* float ploty             | wspolrzedna y konca segmentu - do rysowania   */
/* float plotz             | wspolrzedna y konca segmentu - do rysowania   */
/* float obrot_podst       | wartosc kata obrotu podstawy manipulatora     */
/* ======================================================================= */
/* metody                                                                  */
/* ----------------------------------------------------------------------- */
/* Segment()                               | konstruktor bezparametryczny  */
/* Segment(float)                          | konstruktor z katem           */
/* Segment(float,float)                    | konstruktor z katem i dlug.   */
/* Segment(float,float,float) | konstr. z katem, dlug i rotacja podstawy   */
/* void wyliczWspolrzedne(const Segment &) | wylicza wspolrzedne posilkujac*/
/*                                         | sie poprzednim segmentem      */
/***************************************************************************/


/* ======================================================================= */
/* metoda Segment (konstruktory parametryczne i bezparametryczne)          */
/* ----------------------------------------------------------------------- */
/* konstruuje klase Segment w zaleznosci od liczby parametrow. mozna albo  */
/* od razu podac wartosc katow i dlugosci, albo pierw skonstruowac, a potem*/
/* dopiero przypisac wartosci pol. w programie poslugiwac sie bede         */
/* WYLACZNIE konstruktorem dwuparametrowym - od razu zainicjuje segment    */
/* wartoscia kata i dlugosci ramienia                                      */
/* ----------------------------------------------------------------------- */
/* warunki PRE                                                             */
/* ----------------------------------------------------------------------- */
/* jedynie dlugosc musi byc wartoscia nieujemna. oczywistym jest ponadto,ze*/
/* kat i dlugosc wlasnie sa liczbami rzeczywistymi, dlatego wymagany jest  */
/* typ zmiennoprzecinkowy lub calkowity                                    */
/* ======================================================================= */

Segment::Segment()                             //konstruktor bezparametryczny
{ 
   kat=0;
   x=0;
   y=0;
   kat_ox=0;
   plotx=0;
   ploty=0;
   plotz=0;
}

Segment::Segment(float k)                             //konstruktor z katem
{
   kat=k;
   x=0;
   y=0;
   kat_ox=0;
   plotx=0;
   ploty=0;
   plotz=0;
}

Segment::Segment(float k,float dl)        //konstruktor z katem i dlugoscia
{
   kat=k;

   if(dl<0)     // jezeli dlugosc jest ujemna, wypisuje blad i zmienia znak
   {
      cout<<"Niedozwolona dlugosc."<<endl;
      dl*=-1;
   }
   dlugosc=dl;
   x=0;
   y=0;
   kat_ox=0;
   plotx=0;
   ploty=0;
   plotz=0;
}

Segment::Segment(float k,float dl,float rot) //jw, tylko ze z rotacja pod.
{
   kat=k;

   if(dl<0)     // jezeli dlugosc jest ujemna, wypisuje blad i zmienia znak
   {
      cout<<"Niedozwolona dlugosc."<<endl;
      dl*=-1;
   }
   dlugosc=dl;
   x=0;
   y=0;
   plotx=0;
   ploty=0;
   plotz=0;
   kat_ox=k;
   obrot_podst=rot;
}

/* ======================================================================= */
/* metoda wyliczWspolrzedne                                                */
/* ----------------------------------------------------------------------- */
/* metoda ta sluzy do wyliczenia wspolrzednych bezwglednych (ukladu Karte- */
/* zjanskiego) KONCA wybranego segmentu (czyli przegubu nastepnego, lub,   */
/* gdy mamy doczynienia z ostatnim segmentem, wspolrzedne efektora)        */
/*                                                                         */
/* korzystam tu z elementarnych zasad matematycznych i funkcji trygonome-  */
/* trycznych.                                                              */
/* ----------------------------------------------------------------------- */
/* warunki PRE                                                             */
/* ----------------------------------------------------------------------- */
/* funkcja dostaje na wejscie referencje na poprzedni segment. domyslnie   */
/* poprzedni segment polaczony jest z nastepnym przegubem, jest to istotne */
/* w poprawnosci dzialania tego algorytmu; jest to jedyny warunek wymagany.*/
/* ----------------------------------------------------------------------- */
/* dzialanie algorytmu                                                     */
/* ----------------------------------------------------------------------- */
/* 1. znajdz kat nachylenia przegubu do osi OX                             */
/*    (wykorzystaj to, ze masz nachylenie poprzedniego przegubu do osi OX  */
/*     oraz odchylenie obecnego przegubu do poprzedniego. oczywistym jest  */
/*     wiec, ze szukane nachylenie do OX jest superpozycja obu tych katow) */
/*                                                                         */
/*  . zatem kat_do_OX = kat_miedzy_segmentami + kat_do_OX_poprzedniego     */
/*                                                                         */
/* 2. przesun segment do poczatku ukladu, policz wspolrzedne punktu konca  */
/*    z wlasnosci:                                                         */
/*                                                                         */
/*     x =  cos(kat_do_OX) * R    \  gdzie R to dlugos obecnego segmentu   */
/*     y = sin(kat_do_OX) * R    /                                         */
/*                                                                         */
/* 3. spowrotem przesun segment do miejsca, gdzie konczy sie poprzedni     */
/*    przegub.                                                             */
/*                                                                         */
/*    x = x + x_poprzedniego                                               */
/*    y = y + y_poprzedniego                                               */
/* ======================================================================= */

void Segment::wyliczWspolrzedne(const Segment & poprz)
{
   kat_ox=kat+poprz.kat_ox;
   x=poprz.x+dlugosc*cos(deg2rad(kat_ox));
   y=poprz.y+dlugosc*sin(deg2rad(kat_ox));
   plotx=0;
   ploty=x;
   plotz=y;

}

/***************************************************************************/
/* klasa Manipulator                                                       */
/* ======================================================================= */
/* opis klasy                                                              */
/* ----------------------------------------------------------------------- */
/* klasa ta sluzy do obslugi manipulatora. Zawarte sa w niej pola, ktore   */
/* sa de facto lista segmentow (obiekty typu Segment), liczba segmentow    */
/* i dlugosc pojedynczego segmentu                                         */
/*                                                                         */
/* zawiera rowniez metody obslugujace wyswietlanie menu uzytkownika,       */
/* procedure przeliczajaca wspolrzedne bezwgledne wszystkich segmentow,    */
/* funkcje umozliwiajace dodanie/usuniecie segmentu i zmiane kata segmentu */
/* ======================================================================= */
/* pola                                                                    */
/* ----------------------------------------------------------------------- */
/* int segmentow           | przechowuje liczbe segmentow                  */
/* float dlugosc           | dlugosc kazdego z segmentow                   */
/* list <Segment> Lista    | lista Segmentow                               */
/* list <Segment> iter     | iterator listy                                */
/* float obrot_podstawy    | przechowuje kat obrotu podstawy manipulatora; */
/*                         | pole niezbedne dla wizualizacji 3D            */
/* PzG::LaczeDoGNUPlota * lacze | wskaznik na lacze. obecnosc wskaznika    */
/*                              | na ten obiekt umozliwia rysowanie z      */
/*                              | z poziomu metod klasy manipulator        */
/* ======================================================================= */
/* metody                                                                  */
/* ----------------------------------------------------------------------- */
/* Manipulator()              | konstruktor klasy                          */
/* void interfejsUsera()      | procedura wyswietlajaca i obslugujaca menu */
/* void dodajSegment()        | dodaje segment o zadanej wartosci kata     */
/* void usunSegment()         | usuwa ostatni segment z listy              */
/* void zmienDlugoscListy(int)| zmienia dlugosc listy                      */
/* void wyswietlListe() const | wyswietla wspolrzedne przegubow            */
/* void przeliczWspolrzedne() | procedura uruchamiajaca wyliczanie wspolrz.*/
/* void zmienKat(int,float)   | zmienia wartosc kata o danym indeksie      */
/* float zwrocKat(int)        | zwraca wartosc kata segmentu               */
/* void zmienDlugoscSegmentu(float)                                        */
/* void obrocPodstawe(float)  | metoda sluzaca do obracania podstawy robota*/
/*                            | zmienia dlugosc wszystkich segmentow       */
/* void zapiszDoPliku()       | funkcja zapisujaca do pliku wartosci wspol.*/
/* void rysujPlot()           | funkcja obslugujaca gnuplota               */
/* void wstawLacze(PzG::LaczeDoGNUPlota *) | wstawia do klasy wskaznik na  */
/*                                         | LaczeDoGNUPlota               */
/***************************************************************************/

/* ======================================================================= */
/* metoda Manipulator (konstruktor bezparametryczny)                       */
/* ----------------------------------------------------------------------- */
/* ustawia dlugosc segmentu na 1, liczbe segmentow na 0, tworzy zerowy     */
/* element listy o parametrach (0,0)                                       */
/* ======================================================================= */

Manipulator::Manipulator()
{
   Segment zerowy(0,0);
   Lista.push_back(zerowy);
   dlugosc=0;
   segmentow=0;
   obrot_podstawy=0;
}

/* ======================================================================= */
/* metoda interfejsUsera                                                   */
/* ----------------------------------------------------------------------- */
/* wyswietla menu z wraz z lista segmentow                                 */
/* ======================================================================= */

void Manipulator::interfejsUsera()
{
   int i;

   cout<<"Aktualne wspolrzedne i parametry:"<<endl;
   wyswietlListe();
   cout<<"Menu:"<<endl;
 
   for(i=0;i<segmentow;i++)
   cout<<" "<<i<<" - Zmiana kata q"<<i<<endl;
   cout<<" r - Zmiana rotacji podstawy"<<endl;
   cout<<" + - dodaj segment"<<endl;
   cout<<" - - usun segment"<<endl;
   cout<<" z - zmiana liczby segmentow"<<endl;
   cout<<" l - zmiana dlugosci segmentu"<<endl;
   cout<<" p - wyswietl rysunek"<<endl;;
   cout<<" ? - ponowne wyswietlenie menu"<<endl;
   cout<<" k - koniec programu"<<endl<<endl;
   cout<<"Twoj wybor?> "<<flush;
}





/* ======================================================================= */
/* metoda dodajSegment                                                     */
/* ----------------------------------------------------------------------- */
/* dodaje na koniec listy nowy element typu Segment o parametrze: kat      */
/* dlugosc jest pobierana domyslnie z klasy Manipulator.                   */   
/* Element dodawany jest przez utworzenie kopii tymczasowej i przekazanie  */
/* jej do Manipulatora.                                                    */
/*                                                                         */
/* zwieksza liczbe segmentow po dodaniu nowego elementu                    */
/* ======================================================================= */

void Manipulator::dodajSegment()
{
   Segment temp(0,dlugosc,obrot_podstawy);
   Lista.push_back(temp);
   segmentow++;
}

/* ======================================================================= */
/* metoda usunSegment                                                      */
/* ----------------------------------------------------------------------- */
/* usuwa ostatni Segment z listy i zmniejsza liczbe segmentow              */ 
/* ======================================================================= */

void Manipulator::usunSegment()
{
   if(segmentow>0)
   {
      Lista.pop_back();
      --segmentow;
   }
}

/* ======================================================================= */
/* metoda zmienDlugoscListy                                                */
/* ----------------------------------------------------------------------- */
/* za pomoca metod dodajSegment i usunSegment modyfikuje dlugosc listy     */ 
/* w zaleznosci od parametru nowa_dlugosc                                  */
/* kiedy doda lub usunie odpowiednia liczbe elementow z listy, przypisuje  */
/* nowa liczbe segmentow                                                   */
/* ======================================================================= */

void Manipulator::zmienDlugoscListy(int nowa)
{
   int i=segmentow-nowa;
   if(nowa<0) cout<<"Dlugosc nie moze byc ujemna!"<<endl;
   else if(segmentow>nowa) while(i--) usunSegment();
   else if(segmentow<nowa) while((i++)*(-1)) dodajSegment();
   else cout<<"Dlugosc jest taka sama!"<<endl;
   
   if(nowa>0) segmentow=nowa;

  
}

/* ======================================================================= */
/* metoda wyswietlListe                                                    */
/* ----------------------------------------------------------------------- */
/* wypisuje wspolrzedne przegubow                                          */
/* ======================================================================= */

void Manipulator::wyswietlListe()
{
   int i=0;   //iterator potrzebny do sprawdzenia czy element jest efektorem
   iter=Lista.begin();               //iterator listy, ustawiamy na poczatek
   while(iter!=Lista.end())         //dopoki iterator nie trafi na koniec...
   {
      if(i!=segmentow)              //dopoki nie trafisz na przegub efektora
      {
         ++iter;                          //wyswietl kat nachylenia przegubu
         cout<<" q"<<setw(3)<<left<<i<<" = "<<setw(5)<<iter->kat<<"  | ";
         --iter;
      }
      else cout<<setw(20)<<" ";    //a jesli efektor, pomin wyswietlanie kata

      if(i==segmentow) cout<<"Efektor :"<<flush;     //jezeli efektor, napisz
      else             cout<<"Przegub q"<<setw(2)<<left<<i<<":"<<flush;
                                                         //jesli nie, przegub
      cout<<setprecision(2+1)                            //wypisz wspolrzedne
          <<" ("
          <<iter->x*cos(deg2rad(obrot_podstawy))<<","
          <<iter->x*sin(deg2rad(obrot_podstawy))<<","
          <<iter->y<<")"<<endl;      
      ++iter;                                        //przesun iterator listy
      ++i;                                  //oraz iterator kontroli efektora

   }
   cout<<endl;
   cout<<"  dlugosc ramienia: l = "<<dlugosc<<endl                  //dlugosc
       <<"    obrot podstawy: r = "<<obrot_podstawy<<endl<<endl;   
                                               // obrot podstawy manipulatora
}

/* ======================================================================= */
/* metoda przeliczWspolrzedne                                              */
/* ----------------------------------------------------------------------- */
/* funkcja ta rozpoczyna wedrowke od poczatku listy i przelicza kolejne    */
/* wartosci wspolrzednych przegubow za pomoca metody klasy Segment -       */
/* - wyliczWspolrzedne                                                     */
/*                                                                         */
/* metoda bedzie zbudowana iteracyjnie. wykona sie tyle razy, ile jest     */
/* przegubow. Za kazdym powtorzeniem metoda wpisze do Segmentu nowe        */
/* wartosci wspolrzednych bezwglednych                                     */
/*                                                                         */
/* wykorzystuje tu funkcje wyliczWspolrzedne:                              */
/*    Nastepny.wyliczWspolrzedne(Poprzedni);                               */
/*                                                                         */
/* rownolegle z przeliczaniem, do pliku manipulator.dat zapisuja sie       */
/* wspolrzedne do wyrysowania                                              */
/* ======================================================================= */

void Manipulator::przeliczWspolrzedne()
{
   list <Segment>::iterator iter,iter2;              //konstrukcja iteratorow
   iter=Lista.begin();                    //oba iteratory ustaw na pocztek
   iter2=Lista.begin();
   Segment zerowy;                            //konstrukcja segmentu zerowego
   
   while(iter!=Lista.end())           //dopoki nie trafisz na koniec listy
   {
      if(iter==Lista.begin())               //jezeli iter jest na poczatku
      {
         (*iter).wyliczWspolrzedne(zerowy);  //przelicz wspolrzedne z zerowym
         ++iter;  //a nastepnie zwieksz pierwszy iterator, ktory teraz bedzie
      }                 //odpowiadal za iterator Nastepny. iter2 to Poprzedni
      else
      {
         (*(iter)).wyliczWspolrzedne(*iter2);            //wylicz wspolrzedne
         ++iter;                                      //zwieksz oba iteratory
         ++iter2;     
      }      
   }
}

/* ======================================================================= */
/* metoda zmienKat                                                         */
/* ----------------------------------------------------------------------- */
/* funkcja ta rozpoczyna wedrowke od poczatku listy i szuka elementu o     */
/* i-tym indeksie. gdy ten element napotka, zmienia wartosc kata w nim     */
/* na zadana.                                                              */
/* ======================================================================= */

void Manipulator::zmienKat(int indeks, float nowy)
{
   int i=0;     //ustaw iterator kontroli natrafienia na wskazany indeks na 0
   iter=Lista.begin();                     //ustaw iterator listy na poczatek
   ++iter;       //na wstepie zwieksz iterator - pominiecie zerowego przegubu

   while(iter!=Lista.end()) //lec po liscie, az nie trafisz na sam jej koniec
   {
      if(i==indeks)            //jesli trafiles na indeks, zmien wartosc kata
      {
         iter->kat=nowy;
         iter=Lista.end();     //i ustaw iterator listy na koniec,by skonczyc
      }
      ++i;           //inkrementacja iteratora kontroli indeksu i konca listy
      ++iter;
   }
}

/* ======================================================================= */
/* metoda zwrotKat                                                         */
/* ----------------------------------------------------------------------- */
/* funkcja ta rozpoczyna wedrowke od poczatku listy i szuka elementu o     */
/* i-tym indeksie. gdy ten element napotka, zwraca wartosc indeksu         */
/* ======================================================================= */

float Manipulator::zwrocKat(int indeks)
{
   float szukany=0;
   int i=0;     //ustaw iterator kontroli natrafienia na wskazany indeks na 0
   iter=Lista.begin();                     //ustaw iterator listy na poczatek
   ++iter;       //na wstepie zwieksz iterator - pominiecie zerowego przegubu

   while(iter!=Lista.end()) //lec po liscie, az nie trafisz na sam jej koniec
   {
      if(i==indeks)                 //jesli trafiles na indeks, zwroc wartosc
      {
         szukany=iter->kat;
         iter=Lista.end();     //i ustaw iterator listy na koniec,by skonczyc
      }
      ++i;           //inkrementacja iteratora kontroli indeksu i konca listy
      ++iter;
   }
   return szukany;
}

/* ======================================================================= */
/* metoda zmienDlugoscSegmentu                                             */
/* ----------------------------------------------------------------------- */
/* aby zwiekszyc elastycznosc manipulatora (mozliwosc wprowadzania roznych */
/* dlugosci segmentow) - ta metoda sluzy do ustawienia jednej dlugosci     */
/* dla wszystkich segmentow manipulatora                                   */
/* ======================================================================= */

void Manipulator::zmienDlugoscSegmentu(float nowy)
{
   iter=Lista.begin();                     //ustaw iterator listy na poczatek
   ++iter;       //na wstepie zwieksz iterator - pominiecie zerowego przegubu

   while(iter!=Lista.end()) //lec po liscie, az nie trafisz na sam jej koniec
   {
      iter->dlugosc=nowy;
      ++iter; 
   }
   dlugosc=nowy;
}

/* ======================================================================= */
/* metoda obrocPodstawe                                                    */
/* ----------------------------------------------------------------------- */
/* metoda sluzy do zmiany obrotu podstawy manipulatora (3d - obrot w XY)   */
/* ======================================================================= */


void Manipulator::obrocPodstawe(float nowy)
{
   iter=Lista.begin();                     //ustaw iterator listy na poczatek
   ++iter;       //na wstepie zwieksz iterator - pominiecie zerowego przegubu

   while(iter!=Lista.end()) //lec po liscie, az nie trafisz na sam jej koniec
   {
      iter->obrot_podst=nowy;
      ++iter; 
   }
   obrot_podstawy=nowy;
}


/* ======================================================================= */
/* metoda wstawLacze                                                       */
/* ----------------------------------------------------------------------- */
/* umozliwia podstawienie do metody wskaznika na laczeGNUPlota. konieczne  */
/* aby moc poprawnie rysowac z poziomu metod klasy Manipulator             */
/* ======================================================================= */

void Manipulator::wstawLacze(PzG::LaczeDoGNUPlota * wsk)
{
   lacze=wsk;

}
 
/* ======================================================================= */
/* funkcja int2str                                                         */
/* ----------------------------------------------------------------------- */
/* jak sama nazwa wskazuje, sluzy do przeksztalcenia liczby calkowitej     */
/* typu int na ciag string. Dzieki tej funkcji bedzie mozliwe np. utworze- */
/* nie pliku nazwanego liczba porzadkowa.                                  */
/* ======================================================================= */

string int2str(int liczba)
{
    stringstream strm;
    string wyjscie;
    strm << liczba;
    strm >> wyjscie;
    return wyjscie;
}

/* ======================================================================= */
/* metoda rysujUklad                                                       */
/* ----------------------------------------------------------------------- */
/* metoda sluzaca do rysowania ukladu                                      */
/* zakladamy, ze wspolrzedne sa poprawnie przeliczone.                     */
/* ----------------------------------------------------------------------- */
/* funkcja zapisuje kazdy segment w osobnym pliku jako prostopadloscian    */
/* a nastepnie wszystkie prostopadlosciany umieszcza na jednym wykresie    */
/* tak, ze manipulator wydaje sie byc trojwymiarowa bryla                  */
/* ======================================================================= */

void Manipulator::rysujUklad()
{
   float v=segmentow/2;               // wartosc grubosci segmentu na rysunku
   
   float i=1;                     //iterator, sluzy do kontrolowania liczby
                              //segmentow i zmniejszania grubosci kolejnych
   
   /* WEKTOR POSZERZENIA *************************************************/ 
   float v_x;                         //wspolrzedna X wektora "poszerzenia"
   float v_y;                         //wspolrzedna Y wektora "poszerzenia"
   float v_z;                         //wspolrzedna Z wektora "poszerzenia"
   /* ------------------------------------------------------------------ */
   /* wektor ten jest niezbedny w wyznaczaniu wspolrzednych zwiazanych z */
   /* wspolrzednymi manipulatora PO obrocie. zaleznie od kata obrotu     */
   /* podstawy manipulatora, wektor poszerzenia ma rozne wartosci wspol- */
   /* rzednych. wspolrzedne te sa albo dodawane, albo odejmowane do      */
   /* "bazowego" manipulatora - linii prostej, przez co tworza tworzy sie*/
   /* wrazenie, ze manipulator ma pewna grubosc, a nie jest jedynie linia*/
   /* prosta *************************************************************/
   
   
   float pomx;       //wspolrzedne pomocnicze trzymajace wspolrzedne punktu
   float pomy;       //ktory znajduje sie na poczatku danego segmentu
   float pomz;
     
   float sink;    //zmienne pomocnicze, ktore trzymaja wartosci odpowiednio
   float cosk;    //sink - sinus, cosk - cosinus kata obrotu 
   
   /* STRUMIENIOWANIE WEJSCIA *******************************************/    
   string nazwa_pliku;
   string rozsz=".dat";
   ofstream output;
   stringstream temp;
   /* ------------------------------------------------------------------ */
   /* wykorzystuje tutaj przekierowania wejscia i wyjscia, aby bylo mozna*/
   /* utworzyc pliki ze wspolrzednymi nazywane kolejno:                  */
   /* 1.dat - pierwszy segment                                           */
   /* 2.dat - drugi segment, itd.                                        */
   /**********************************************************************/
   
   
   
   (*lacze).UsunWszystkieNazwyPlikow();      //ustawienia lacza do GNUPlota

   sink=sin(deg2rad(obrot_podstawy));        //wyliczenie sinusa i cosinusa
   cosk=cos(deg2rad(obrot_podstawy));        //funkcji obrotu wokol osi OZ
   
   
   iter=Lista.begin();               
 
   while(iter!=Lista.end())                 
   {
   
     /* ZAPISANIE DO PLIKU ********************************************/
      nazwa_pliku="";
      nazwa_pliku=int2str(i++);
      nazwa_pliku+=rozsz;
      output.open(nazwa_pliku.c_str());
     /* ------------------------------------------------------------- */
     /* dzieki tym operacjom kazdy segment ma wspolrzedne zapisane    */
     /* w osobnym pliku, co z kolei pozwala na trojwymiarowy obraz    */
     /* manipulatora w programie GNUPlot                              */
     /*****************************************************************/
     
      pomx=iter->plotx;        //podstawienie wspolrzednych punktu
      pomy=iter->ploty;        //porzedniego do zmiennych pomocniczych
      pomz=iter->plotz;
      
      ++iter;   //zwiekszamy iterator, aby dostac wspolrzedne i kat
                //segmentu ktory nastepuje po punkcie poczatkowym
                //uprzednio wczytanym do zmiennych pomocniczych pom_.
     
      v_x=v;                                //ustawienia wartosci
      v_y=-v*sin(deg2rad(iter->kat_ox));    //wektora "poszerzenia"
      v_z=v*cos(deg2rad(iter->kat_ox));     
     /* ------------------------------------------------------------- */
     /* v_x - opisuje poszerzenie segmentu wzdluz osi OX - osi prosto-*/
     /*       padlej do monitora skierowanej do uzytkownika.          */
     /*       na poczatku obrot wzgl. osi OZ = 0, dlatego v_x=v.      */
     /*       po obrocie wektor ten bedzie mial nieco inna wartosc    */
     /*       v_x=|V|                                                 */
     /* ------------------------------------------------------------- */
     /* v_y - oznaczmy wektor V=[v_y,v_z] i zrzutujmy przestrzen na   */
     /* v_z   plaszczyzne YOZ. chcemy, aby wektor V byl styczny do    */
     /*       ramienia segmentu, zatem tworzy z tymze segmentem kat   */
     /*       prosty.                                                 */
     /*        Jednakoz wektor ten jest nachylony pod pewnym katem do */
     /*       osi OY, oznaczmy ten kat jako B. Za kat A przyjme nachy-*/
     /*       lenie segmentu do OY.                                   */
     /*        Otrzymujemy:                                           */
     /*       90=A+B => B=90-A.                                       */
     /*        Nastepnie z wlasnosci funkcji trygonometrycznych       */
     /*       v_y=|V|*cosB, v_z=|V|*sinB                              */
     /*        podstawiajac B=90-A i stosujac wzory redukcyjne:       */
     /*       v_y=-|V|*sinA,v_z=|V|*cosA                              */
     /*****************************************************************/  


  
     /* WYPISANIE DO PLIKU ********************************************/
     /* w tej sekcji obliczane sa wspolrzedne po obrocie z zaleznosci */
     /*                                                               */
     /*     { x'=x*cosA - y*sinA                                      */
     /* p'= { y'=x*sinA + y*cosA                                      */
     /*     { z'=z                                                    */
     /*                                                               */
     /* gdzie p=(x,y,z); A - kat obrotu                               */
     /*****************************************************************/
   
   output
         <<pomx*cosk-pomy*sink<<" "
         <<pomx*sink+pomy*cosk<<" "
         <<pomz
      <<endl
         <<(pomx+v_x)*cosk-(pomy+v_y)*sink<<" "
         <<(pomx+v_x)*sink+(pomy+v_y)*cosk<<" "
         <<pomz+v_z
      <<endl
         <<(iter->plotx+v_x)*cosk-(iter->ploty+v_y)*sink<<" "
         <<(iter->plotx+v_x)*sink+(iter->ploty+v_y)*cosk<<" "
         <<iter->plotz+v_z
      <<endl
         <<(iter->plotx)*cosk-(iter->ploty)*sink<<" "
         <<(iter->plotx)*sink+(iter->ploty)*cosk<<" "
         <<iter->plotz
      <<endl
      <<"#"<<endl<<endl;
      
  
   output
         <<pomx*cosk-pomy*sink<<" "
         <<pomx*sink+pomy*cosk<<" "
         <<pomz
      <<endl
         <<(pomx-v_x)*cosk-(pomy+v_y)*sink<<" "
         <<(pomx-v_x)*sink+(pomy+v_y)*cosk<<" "
         <<pomz+v_z
      <<endl
         <<(iter->plotx-v_x)*cosk-(iter->ploty+v_y)*sink<<" "
         <<(iter->plotx-v_x)*sink+(iter->ploty+v_y)*cosk<<" "
         <<iter->plotz+v_z
      <<endl
         <<(iter->plotx)*cosk-(iter->ploty)*sink<<" "
         <<(iter->plotx)*sink+(iter->ploty)*cosk<<" "
         <<iter->plotz
      <<endl
      <<"#"<<endl<<endl;

 output
         <<pomx*cosk-pomy*sink<<" "
         <<pomx*sink+pomy*cosk<<" "
         <<pomz
      <<endl
         <<(pomx-v_x)*cosk-(pomy-v_y)*sink<<" "
         <<(pomx-v_x)*sink+(pomy-v_y)*cosk<<" "
         <<pomz-v_z
      <<endl
         <<(iter->plotx-v_x)*cosk-(iter->ploty-v_y)*sink<<" "
         <<(iter->plotx-v_x)*sink+(iter->ploty-v_y)*cosk<<" "
         <<iter->plotz-v_z
      <<endl
         <<(iter->plotx)*cosk-(iter->ploty)*sink<<" "
         <<(iter->plotx)*sink+(iter->ploty)*cosk<<" "
         <<iter->plotz
      <<endl
      <<"#"<<endl<<endl;




   output
         <<pomx*cosk-pomy*sink<<" "
         <<pomx*sink+pomy*cosk<<" "
         <<pomz
      <<endl
         <<(pomx+v_x)*cosk-(pomy-v_y)*sink<<" "
         <<(pomx+v_x)*sink+(pomy-v_y)*cosk<<" "
         <<pomz-v_z
      <<endl
         <<(iter->plotx+v_x)*cosk-(iter->ploty-v_y)*sink<<" "
         <<(iter->plotx+v_x)*sink+(iter->ploty-v_y)*cosk<<" "
         <<iter->plotz-v_z
      <<endl
         <<(iter->plotx)*cosk-(iter->ploty)*sink<<" "
         <<(iter->plotx)*sink+(iter->ploty)*cosk<<" "
         <<iter->plotz
      <<endl
      <<"#"<<endl<<endl;

   output
         <<pomx*cosk-pomy*sink<<" "
         <<pomx*sink+pomy*cosk<<" "
         <<pomz
      <<endl
         <<(pomx+v_x)*cosk-(pomy+v_y)*sink<<" "
         <<(pomx+v_x)*sink+(pomy+v_y)*cosk<<" "
         <<pomz+v_z
      <<endl
         <<(iter->plotx+v_x)*cosk-(iter->ploty+v_y)*sink<<" "
         <<(iter->plotx+v_x)*sink+(iter->ploty+v_y)*cosk<<" "
         <<iter->plotz+v_z
      <<endl
         <<(iter->plotx)*cosk-(iter->ploty)*sink<<" "
         <<(iter->plotx)*sink+(iter->ploty)*cosk<<" "
         <<iter->plotz
      <<endl
      <<"#"<<endl<<endl;






      
      
      
      output.close();                //zamkniecie pliku ze wspolrzednymi
      if(iter!=Lista.end())
      (*lacze).DodajNazwePliku(nazwa_pliku.c_str()); 
      //jezeli nie jest to ostatni segment (efektor), program go rysuje 


       v-=0.2;
       if(v<0) iter=Lista.end();
   }
   
   
     /* RYSUNEK PODSTAWY **********************************************/
     /* w zaleznosci od liczby ramion i grubosci najgrubszego         */
     /* algorytm ten wylicza wspolrzedne podstawy po obrocie          */
     /*****************************************************************/
   
     output.open("podstawa.dat");                         //otworz plik
     
     v=segmentow/2 +0.5;        //zmienna v warunkuje wielkosc podstawy
     output
         <<"0 0 0"<<endl
         <<(v+1)*(cosk-sink)<<" "
         <<(v+1)*(sink+cosk)
         <<" 0"<<endl
         <<(v+1)*(cosk-sink)<<" "
         <<(v+1)*(sink+cosk)<<" "
         <<v+0.5<<endl
         <<(v)*(cosk-sink)<<" "
         <<(v)*(sink+cosk)<<" "
         <<v+1<<endl   
         <<"0 0 "<<v+1<<endl     
           <<"#"<<endl<<endl
           
         <<"0 0 0"<<endl
         <<-(v+1)*(cosk+sink)<<" "
         <<-(v+1)*(sink-cosk)
         <<" 0"<<endl
         <<-(v+1)*(cosk+sink)<<" "
         <<-(v+1)*(sink-cosk)<<" "
         <<v+0.5<<endl
         <<-(v)*(cosk+sink)<<" "
         <<-(v)*(sink-cosk)<<" "
         <<v+1<<endl   
         <<"0 0 "<<v+1<<endl     
           <<"#"<<endl<<endl
      
           
         <<"0 0 0"<<endl
         <<-(v+1)*(cosk-sink)<<" "
         <<-(v+1)*(sink+cosk)
         <<" 0"<<endl
         <<-(v+1)*(cosk-sink)<<" "
         <<-(v+1)*(sink+cosk)<<" "
         <<v+0.5<<endl
         <<-(v)*(cosk-sink)<<" "
         <<-(v)*(sink+cosk)<<" "
         <<v+1<<endl   
         <<"0 0 "<<v+1<<endl     
           <<"#"<<endl<<endl
           
         <<"0 0 0"<<endl
         <<(v+1)*(cosk+sink)<<" "
         <<(v+1)*(sink-cosk)
         <<" 0"<<endl
         <<(v+1)*(cosk+sink)<<" "
         <<(v+1)*(sink-cosk)<<" "
         <<v+0.5<<endl
         <<(v)*(cosk+sink)<<" "
         <<(v)*(sink-cosk)<<" "
         <<v+1<<endl   
         <<"0 0 "<<v+1<<endl     
           <<"#"<<endl<<endl
                    
         <<"0 0 0"<<endl
         <<(v+1)*(cosk-sink)<<" "
         <<(v+1)*(sink+cosk)
         <<" 0"<<endl
         <<(v+1)*(cosk-sink)<<" "
         <<(v+1)*(sink+cosk)<<" "
         <<v+0.5<<endl
         <<(v)*(cosk-sink)<<" "
         <<(v)*(sink+cosk)<<" "
         <<v+1<<endl   
         <<"0 0 "<<v+1<<endl     
           <<"#"<<endl<<endl;      
           
    output.close();                   //zamkniecie pliku ze wspolrzednymi

   (*lacze).DodajNazwePliku("podstawa.dat");//dodaj do rysowania podstawe
   (*lacze).Rysuj();                        //narysuj calosc
}

