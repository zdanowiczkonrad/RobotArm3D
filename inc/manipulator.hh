#ifndef MANIPULATOR_HH
#define MANIPULATOR_HH
#define PI 3.1415926535            // stala PI potrzebna do wyliczania katow
#include "lacze_do_gnuplota.hh"
using namespace std;

class Segment
{

  public:
   float kat;
   float kat_ox;
   float dlugosc;
   float obrot_podst;
   float x;
   float y;
   float plotx;
   float ploty;
   float plotz;
   Segment();
   Segment(float);
   Segment(float,float);
   Segment(float,float,float);

   void wyliczWspolrzedne(const Segment &);
};


class Manipulator
{
  public:
   int segmentow;
   float dlugosc;
   float obrot_podstawy;
   list <Segment> Lista;
   list <Segment>::iterator iter;

   PzG::LaczeDoGNUPlota * lacze;
   Manipulator();

   void interfejsUsera();
   void dodajSegment();
   void usunSegment();
   void zmienDlugoscListy(int);
   void wyswietlListe();
   void przeliczWspolrzedne();
   void zmienKat(int,float);
   float zwrocKat(int);
   void obrocPodstawe(float);
   void zmienDlugoscSegmentu(float);
   void wstawLacze(PzG::LaczeDoGNUPlota *);
   void rysujUklad();
};

   string int2str(int i);

#endif
