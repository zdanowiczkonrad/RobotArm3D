#ifndef LACZE_DO_GNUPLOTA_HH
#define LACZE_DO_GNUPLOTA_HH
#include <string>
#include <list>
#include <vector>

#ifdef __GNUG__
#pragma interface
#endif

/*!
 * \file  lacze_do_gnuplota.hh
 *  
 *  Plik zawiera definicjê klasy realizuj±cej interfejs
 *  komunikacyjny do programu gnuplot.
 */


/*!
 * \brief Modu³ narzêdzi umo¿liwiaj±cych po³±cznie z GNUPlotem
 *
 * Niniejsza przestrzeñ nazw stanowi modu³ logiczny zawieraj±cy
 * narzêdzia umo¿liwiaj±ce realizacjê po³±czenia z programem \p gnuplot.
 */
namespace PzG {

  /*!
   * \brief Okre¶la tryb rysowania realizowanego przez program \p gnuplot
   *
   *  Typ wyliczeniowy okre¶laj±ce dopuszczalne tryby rysowania
   *  realizowanego przez program \p gnuplot. Wybór trybu wi±¿e siê
   *  ze zmian± sposobu interpretacji danych zawartych pliku. Je¶li 
   *  np. wybrany zostanie tryb 2D, to zak³ada siê, ¿e w ka¿dej linii
   *  pliku z danymi znajduj± siê warto¶ci wspó³rzêdnych \e x, \e y.
   *  Warto¶ci typu:
   *   \li \p TR_2D - rysowanie w trybie 2D, co sprowadza siê do 
   *                  rysowania wykresów funkcji jednej zmiennej.
   *   \li \p TR_3D - rysowanie w trybie 3D. Oznacza to mo¿liwo¶æ
   *                  rysowania wykresów funkcji dwóch zmiennych.
   *                 
   */
  enum TrybRysowania { TR_2D,  TR_3D };
  /*!
   * \brief Sposób rysowania linii
   * 
   * Okre¶la sposób rysowania linii.
   */
  enum RodzajRysowania { RR_Ciagly, RR_Punktowy }; 

  /*!
   * \brief Zestaw informacji dotycz±cy pliku i sposobu rysowania
   *
   * Klasa modeluje zestaw informacji dotycz±cy pliku i sposobu
   * w jaki maj± byæ wizualizowane zawarte w nim dane.
   */
  class InfoPlikuDoRysowania {
    public:
     /*!
      * Inicjalizuje obiekt.
      *  \param NazwaPliku - nazwa pliku, z którego pobierane bêd± dane,
      *  \param RodzRys - rodzaj rysowania linii,
      *  \param Szerokosc - szerokosc linii.
      */
      InfoPlikuDoRysowania(const char* NazwaPliku, RodzajRysowania  RodzRys, int Szerokosc)
      {
        _NazwaPliku = NazwaPliku;
        _RodzRys = RodzRys;
        _Szerokosc = Szerokosc;
      }
     /*!
      * \brief Udostêpia nazwê pliku do rysowania
      *
      * Udostêpnia nazwê pliku z danymi do rysowania.
      */
      const std::string WezNazwePliku() const { return _NazwaPliku; }
     /*!
      * \brief Zmienia nazwê pliku do rysowania
      *
      * Zmienia nazwê pliku z danymi do rysowania.
      */
      void ZmienNazwePliku(const std::string& NazwaPliku) { _NazwaPliku = NazwaPliku; }
     /*!
      * \brief Udostêpnia sposób rysowanej linii
      *
      * Udostêpnia informacjê o sposóbie rysowania linii.
      */
      RodzajRysowania WezRodzRys() const { return _RodzRys; }
     /*!
      * \brief  Udostêpnia informacjê o szeroko¶ci linii.
      *
      *  Udostêpnia informacjê o szeroko¶ci rysowanej linii.
      */
      int WezSzerokosc() const { return _Szerokosc; }

    private:
    /*!
     * \brief Nazwa pliku z danymi do rysowania
     *
     * Nazwa pliku z danymi do rysowania.
     */
      std::string  _NazwaPliku;
    /*!
     * \brief Szeroko¶æ u¿ytego piórka
     *
     * Okre¶la szeroko¶æ piórka, jakie ma byæ u¿yte
     * do rysowania obiektów graficznych.
     */
      int  _Szerokosc;
    /*!
     * \brief Sposób rysowania danej linii
     *
     * Przechowuje informacje o sposobie rysowania linii.
     */
      RodzajRysowania  _RodzRys;
  };


/*!
 * \brief  Klasa realizuje interfejs do programu GNUPlot.
 *
 * Klasa realizuje interfejs do programu GNUPlot. Pozwala ona na wskazanie
 * zbioru punktów p³aszczyzn umieszczonych w pliku lub plikach.
 * Ka¿dy taki zbiór mo¿e byæ nastêpnie wizualizowany przez program 
 * gnuplot w postaci oddzielnych p³aszczyzn z wycinaniem czê¶ci zas³anianych.
 */
class LaczeDoGNUPlota {
 protected:
  /*!
   * \brief Lista nazw plików z danymi dla \e gnuplota.
   *
   * Pole jest zarz±dc± listy nazw plików, z których s± wczytywane
   * dane dotycz±ce rysowania obrysu bry³ przez program \e gnuplot.
   * Operacja ta wykonywana jest po wywo³aniu polecenia.
   * \link LaczeDoGNUPlota::Rysuj Rysuj\endlink.
   */
  static std::list<InfoPlikuDoRysowania>  _InfoPlikow;

   /*!
    *  Pole przechowuje deskryptor do wej¶cia standardowego uruchomionego
    *  programu gnuplot.
    */
  int           _Wejscie_GNUPlota;
   /*!
    *  Pole przechowuje deskryptor do weyj¶cia standardowego uruchomionego
    *  programu gnuplot.
    */
  int           _Wyjscie_GNUPlota;
   /*!
    *  \brief Decyduje czy maj± byæ wy¶wietlane komunikaty o b³êdach,
    *         czy te¿ nie.
    *
    *  Warto¶æ tego pola decyduje o tym czy komunikaty o b³êdach bêd± 
    *  wy¶wietlane na wyj¶cie standardowe b³êdów (\b cerr), czy te¿ nie.
    *   \li \p true - komunikaty bêdê wy¶wietlane,
    *   \li \p false -  komunikaty nie bêdê wy¶wietlane.
    */
  bool          _WyswietlajKomunikatyOBledach;

  /*!
   * \brief Okre¶la aktualny tryb rysowania
   *
   * Zawarto¶æ pola determinuje sposób rysowania, jaki zostanie 
   * wymuszony na programie \p gnuplot poprzez wys³anie do niego
   * odpowiednich poleceñ. Wspomniane wymuszenie jest realizowane
   * poprzez wywo³anie polecenia 
   * \link LaczeDoGNUPlota::Rysuj Rysuj()\endlink
   */
  TrybRysowania  _TrybRys;
   /*!
    *  \brief Dolny zakres wy¶wietlanej skali skali dla osi \e OX.
    *
    *  Okre¶la dolny zakres wy¶wietlanej skali dla osi \e OX.
    */
  float  _Xmin;
   /*!
    *  \brief Górny zakres wy¶wietlanej skali skali dla osi \e OX.
    *
    *  Okre¶la górny zakres wy¶wietlanej skali dla osi \e OX.
    */
  float  _Xmax;
   /*!
    *  \brief Dolny zakres wy¶wietlanej skali skali dla osi \e OY.
    *
    *  Okre¶la dolny zakres wy¶wietlanej skali dla osi \e OY.
    */
  float  _Ymin;
   /*!
    *  \brief Górny zakres wy¶wietlanej skali skali dla osi \e OY.
    *
    *  Okre¶la górny zakres wy¶wietlanej skali dla osi \e OY.
    */
  float  _Ymax;
   /*!
    *  \brief Dolny zakres wy¶wietlanej skali skali dla osi \e OZ.
    *
    *  Okre¶la dolny zakres wy¶wietlanej skali dla osi \e OZ.
    */
  float  _Zmin;
   /*!
    *  \brief Górny zakres wy¶wietlanej skali skali dla osi \e OZ.
    *
    *  Okre¶la górny zakres wy¶wietlanej skali dla osi \e OZ.
    */
  float  _Zmax;
   /*!
    *  Warto¶æ tego pola definiuje skalowanie rysunku wzd³u¿ osi 
    *  \e OX (o¶ horyzontalna ekranu).
    */
  float  _Xskala;
   /*!
    *  Warto¶æ tego pola definiuje skalowanie rysunku wzd³u¿ osi 
    *  \e OZ (o¶ wertykalna ekranu).
    */
  float  _Zskala;
   /*!
    *  Warto¶æ tego pola definiuje rotacjê rysunku (zmiane punktu patrzenia)
    *  wzglêdem osi \e OX.
    */
  float  _Xrotacja;
   /*!
    *  Warto¶æ tego pola definiuje rotacjê rysunku (zmiane punktu patrzenia)
    *  wzglêdem osi \e OZ.
    */
  float  _Zrotacja;

  /*!
   * \brief Czy o¶ OX ma byæ widoczna
   *
   * Przechowuje informacjê decyduj±c± o tym czy o¶ OX bêdzie
   * widoczna na rysunku (\p true), czy te¿ nie (\p false).
   */
  bool _PokazOs_OX;

  /*!
   * \brief Czy o¶ OY ma byæ widoczna
   *
   * Przechowuje informacjê decyduj±c± o tym czy o¶ OY bêdzie
   * widoczna na rysunku (\p true), czy te¿ nie (\p false).
   */
  bool _PokazOs_OY;



  /*!
   * \brief Tworzy listê parametrów umo¿liwiaj±cych rysowanie dodatkowych elementów
   *
   * Metoda ta przewidziana jest jako element rozszerzenia pozwalaj±cego
   * w klasach pochodnych powiêkszyæ listê rysowanych elementów.
   * \pre Parametr \e Polecenie powinien zawieraæ polecenie \e plot lub \e splot,
   *      do którego bêdzie mo¿liwe dopisanie dalszego ci±gu.
   * \param Polecenie - polecenie rysowania, do którego maj± byæ dopisane 
   *                    nazwy plików i odpowiednie parametry dla polecenia plot.
   * \param Sep - zawiera znak separatora miêdzy poszczególnymi
   *              parametrami. Je¿eli parametry listy przeszkód
   *              s± generowane jako pierwsze, to zmienna ta musi 
   *              byæ wska¼nikiem do wska¼nika na ³añcuch: " ".
   */
  virtual bool DopiszPlikiDoPoleceniaRysowania( std::string &Polecenie, char const **Sep );

  /*!
   *  \brief Tworzy polecenie ustawiaj±ce zakres dla danej wspó³rzêdnej.
   *
   *  Tworzy polecenie dla programu \e gnuplot ustawiaj±ce zakres 
   *  wspó³rzêdnych wybranej wspó³rzêdnej \e x, \e y lub \e z, 
   *  dla której ma byæ tworzony dany rysunek.
   *  \param Os - zawiera znak okre¶laj±cy wspó³rzêdn±, dla której
   *             ma zostaæ wygenerowane polecenie ustawienia zakresu.
   *  \return ³añcuch znaków polecenia ustawiaj±cego ¿±dany zakres
   *          dla wybranej wspó³rzêdnej.
   */
  std::string ZapiszUstawienieZakresu(char Os) const;
  /*!
   *  \brief Tworzy polecenie ustawiaj±ce punkt obserwacji.
   *
   *  Tworzy polecenie dla programu \e gnuplot ustawiajaj±ce punkt obserwacji
   *  poprzez zadanie rotacji i skali dla poszczególnych osi.
   */
  std::string ZapiszUstawienieRotacjiISkali() const;
   /*!
    * Przesy³a na wej¶cie programu \e gnuplot zadany ci±g znaków.
    *  \param Polecenie - komunikat przeznaczony do przeslania.
    *
    * \pre  Musi byæ zainicjowane po³±czenie z programem gnuplot.
    *
    * \retval true  - jesli przeslanie polecenia zakoñczy³o siê powodzeniem,
    * \retval false - w przypadku przeciwnym.
    *
    */
  bool PrzeslijDoGNUPlota(const char *Polecenie);
  /*!
   * \brief  Udostêpnia informacjê czy maj± byæ wy¶wietlane informacje o b³êdach.
   *
   *  Udostêpnia warto¶æ pola
   *  \link LaczeDoGNUPlota::_WyswietlajKomunikatyOBledach
   *            _WyswietlajKomunikatyOBledach\endlink.
   *  Okre¶la ono, czy maj± byæ wy¶wietlane komunikaty o b³êdach na wyj¶cie
   *  standardowe, czy te¿ nie.
   */
  bool CzyWyswietlacKomunikaty() const { return _WyswietlajKomunikatyOBledach;}
  /*!
   * \brief Uruchamia program \e gnuplot jako proces potomny.
   */
  bool UtworzProcesPotomny();
  /*!
   * Wy¶wietla na wyj¶cie "standard error" komunikat (przekazany jako
   * parametr), o ile pole 
   *   \link LaczeDoGNUPlota::_WyswietlajKomunikatyOBledach
   *          _WyswietlajKomunikatyOBledach\endlink  ma warto¶æ
   * \p true. W przypadku przeciwnym komunikat nie jest wy¶wietlany.
   */
  void KomunikatBledu(const char *Komunikat) const;

  /*!
   * \brief Tworzy preambu³ê poprzedzaj±c± polecenie rysowania
   *
   *  Tworzy zbiór poleceñ, które ustawiaj± w³a¶ciwy tryb rysowania
   *  oraz zakresy wspó³rzêdnych, jak te¿ wszystkie inne parametry
   *  wynikaj±ce z przyjêtego trybu rysowania.
   */
  void BudujPreambulePoleceniaRysowania(std::string &Preambula) const;

  /*!
   * \brief Tworzy preambu³ê poprzedzaj±c± polecenie rysowania w trybie 2D
   *
   *  Tworzy zbiór poleceñ, które ustawiaj± w³a¶ciwy tryb rysowania
   *  oraz zakresy wspó³rzêdnych, jak te¿ wszystkie inne parametry
   *  wynikaj±ce z trybu rysowania 2D.
   */
  void BudujPreambule_2D(std::string &Preambula) const;


  /*!
   * \brief Tworzy preambu³ê poprzedzaj±c± polecenie rysowania w trybie 3D
   *
   *  Tworzy zbiór poleceñ, które ustawiaj± w³a¶ciwy tryb rysowania
   *  oraz zakresy wspó³rzêdnych, jak te¿ wszystkie inne parametry
   *  wynikaj±ce z trybu rysowania 3D.
   */
  void BudujPreambule_3D(std::string &Preambula) const;


 public:

  /*!
   * \brief Umo¿liwia lub zabrania rysowania osi OX
   *
   * Umo¿liwia lub zabrania rysowania osi \e OX na rysunku wykresu.
   * \param Pokaz - decyduje o tym czy o¶ \e OX bêdzie rysowana (\p true),
   *                czy te¿ nie (\p false).
   */
  void PokazOs_OX(bool Pokaz) { _PokazOs_OX = Pokaz; }

  /*!
   * \brief Czy o¶ OX ma byæ rysowana
   *
   * Udostêpnia informacjê czy o¶ \e OX ma byæ rysowana,
   * czy te¿ nie.
   * \retval true - gdy o¶ \e OX ma byæ rysowana,
   * \retval false - w przypadku przeciwnym.
   */
  bool PokazOs_OX() const { return _PokazOs_OX; }


  /*!
   * \brief Umo¿liwia lub zabrania rysowania osi OY
   *
   * Umo¿liwia lub zabrania rysowania osi \e OY na rysunku wykresu.
   * \param Pokaz - decyduje o tym czy o¶ \e OY bêdzie rysowana (\p true),
   *                czy te¿ nie (\p false).
   */
  void PokazOs_OY(bool Pokaz) { _PokazOs_OY = Pokaz; }

  /*!
   * \brief Czy o¶ OY ma byæ rysowana
   *
   * Udostêpnia informacjê czy o¶ \e OY ma byæ rysowana,
   * czy te¿ nie.
   * \retval true - gdy o¶ \e OY ma byæ rysowana,
   * \retval false - w przypadku przeciwnym.
   */
  bool PokazOs_OY() const { return _PokazOs_OY; }


  /*!
   *  Udostêpnia doln± warto¶æ zakresu skali wzd³u¿ osi \e OX.
   */
  float Xmin() const { return _Xmin; }
  /*!
   *  Udostêpnia górn± warto¶æ zakresu skali wzd³u¿ osi \e OX.
   */
  float Xmax() const { return _Xmax; }
  /*!
   *  Udostêpnia doln± warto¶æ zakresu skali wzd³u¿ osi \e OY.
   */
  float Ymin() const { return _Ymin; }
  /*!
   *  Udostêpnia górn± warto¶æ zakresu skali wzd³u¿ osi \e OY.
   */
  float Ymax() const { return _Ymax; }
  /*!
   *  Udostêpnia doln± warto¶æ zakresu skali wzd³u¿ osi \e OZ.
   */
  float Zmin() const { return _Zmin; }
  /*!
   *  Udostêpnia górn± warto¶æ zakresu skali wzd³u¿ osi \e OZ.
   */
  float Zmax() const { return _Zmax; }

  /*!
   * \brief Zmienia tryb rysowania
   *
   * Zmienia tryb rysowania jaki zostanie wymuszony na programie
   * \p gnuplot.
   * \param Tryb - warto¶æ parametru okre¶la nowy tryb rysowania.
   */
  void ZmienTrybRys(TrybRysowania  Tryb) { _TrybRys = Tryb; }

  /*!
   * \brief Udostêpnia aktualny tryb rysowania
   *
   * Udostêpnia informacjê o aktualnym trybie rysowania.
   */
  TrybRysowania WezTrybRys() const { return _TrybRys; }

  /*!
   *  \brief Ustawia zakres osi \e OX
   *
   * Ustawia zakres osi \e OX. Ogranicza to obszar, który bêdzie
   * zwizualizowany przez programa \e gnuplot.
   * \param Xo - dolna granica obszaru rysowania dla osi \e OX.
   * \param Xn - górna granica obszaru rysowania dla osi \e OX.
   */
  void UstawZakresX(float Xo, float Xn) { _Xmin = Xo;  _Xmax = Xn; }
  /*!
   *  \brief Ustawia zakres osi \e OY
   *
   * Ustawia zakres osi \e OY. Ogranicza to obszar, który bêdzie
   * zwizualizowany przez programa \e gnuplot.
   * \param Yo - dolna granica obszaru rysowania dla osi \e OY.
   * \param Yn - górna granica obszaru rysowania dla osi \e OY.
   */
  void UstawZakresY(float Yo, float Yn) { _Ymin = Yo;  _Ymax = Yn; }
  /*!
   *  \brief Ustawia zakres osi \e OZ. 
   *
   * Ustawia zakres osi \e OZ. Ogranicza to obszar, który bêdzie
   * zwizualizowany przez programa \e gnuplot.
   * \param Zo - dolna granica obszaru rysowania dla osi \e OZ.
   * \param Zn - górna granica obszaru rysowania dla osi \e OZ.
   */
  void UstawZakresZ(float Zo, float Zn) { _Zmin = Zo;  _Zmax = Zn; }


  /*!
   *  \brief Udostêpnia skalê dla osi \e OX.
   *
   *  Udostêpnia skalê dla osi \e OX dla tworzonego rysunku.
   */
  float SkalaX() const { return _Xskala; }
  /*!
   *  \brief Udostêpnia skalê dla osi \e OZ.
   *
   *  Udostêpnia skalê dla osi \e OZ dla tworzonego rysunku.
   */
  float SkalaZ() const { return _Zskala; }
  /*!
   *  \brief Zadaje skalê wzd³u¿ osi \e OZ.
   *
   *  Zadaje skalê wzd³u¿ osi \e OX dla tworzonego rysunku.
   *  \param skala_x - skala wzd³u¿ osi \e OX.
   */
  void UstawSkaleX( float skala_x ) { _Xskala = skala_x; }
  /*!
   *  \brief Zadaje skalê wzd³u¿ osi \e OZ.
   *
   *  Zadaje skalê wzd³u¿ osi \e OZ dla tworzonego rysunku.
   *  \param skala_z - skala wzd³u¿ osi \e OZ.
   */
  void UstawSkaleZ( float skala_z ) { _Zskala = skala_z; }
  /*!
   *  \brief Zadaje skalê wzd³u¿ osi \e OX i \e OZ.
   *
   *  Zadaje skalê wzd³u¿ osi \e OX i \e OZ dla tworzonego rysunku.
   *  \param skala_x - skala wzd³u¿ osi \e OX.
   *  \param skala_z - skala wzd³u¿ osi \e OZ.
   */
  void UstawSkaleXZ( float skala_x, float skala_z ) 
          { UstawSkaleX(skala_x);  UstawSkaleZ(skala_z); }

  /*!
   *  Udostêpnia warto¶æ k±ta rotacji renderowanego rysunku wokó³
   *  osi \e OX. Zwracana warto¶æ wyra¿ona jest w stopiniach.
   */
  float RotacjaX() const { return _Xrotacja; }
  /*!
   *  Udostêpnia warto¶æ k±ta rotacji renderowanego rysunku wokó³
   *  osi \e OZ. Zwracana warto¶æ wyra¿ona jest w stopiniach.
   */
  float RotacjaZ() const { return _Zrotacja; }
  /*!
   * \brief Ustawia rotacjê wokó³ osi \e OX.
   *
   * Zadaje k±t rotacji wokó³ osi \e OX. Umo¿liwia to zmianê
   * punktu obserwacji renderowanego rysunku.
   * \param  kat_x - warto¶æ k±ta rotacji. Jego warto¶æ podawana 
   *                 jest w stopniach.
   */
  void UstawRotacjeX( float kat_x ) { _Xrotacja = kat_x; }
  /*!
   * \brief Ustawia rotacjê wokó³ osi \e OZ.
   *
   * Zadaje k±t rotacji wokó³ osi \e OZ. Umo¿liwia to zmianê
   * punktu obserwacji renderowanego rysunku.
   * \param  kat_z - warto¶æ k±ta rotacji. Jego warto¶æ podawana 
   *                 jest w stopniach.
   */
  void UstawRotacjeZ( float kat_z ) { _Zrotacja = kat_z; }
  /*!
   * \brief Ustawia rotacjê wokó³ osi \e OX i \e OZ.
   *
   * Zadaje jednocze¶nie k±t rotacji wokó³ osi \e OX i \e OZ. 
   * Umo¿liwia to zmianê
   * punktu obserwacji renderowanego rysunku.
   * \param  kat_x - warto¶æ k±ta rotacji wzglêdem osi \e OX. 
   *                 Jego warto¶æ podawana 
   *                 jest w stopniach.
   * \param  kat_z - warto¶æ k±ta rotacji wzglêdem osi \e OZ. 
   *                 Jego warto¶æ podawana 
   *                 jest w stopniach.
   */
  void UstawRotacjeXZ( float kat_x, float kat_z ) 
    { UstawRotacjeX(kat_x);  UstawRotacjeZ(kat_z); }  

  /*!
   *  \brief Zezwala lub zabrania wy¶wietlania komunikatów.
   *
   *  Metoda pozwala, albo te¿ zabrania wy¶wietlania komunikatów o blêdach.
   *  Je¿eli jaka¶ z operacji nie powiedzie siê, to jako wynik zwracana
   *  jest warto¶æ \p false. Oprócz tego metody takie moga wy¶wietlaæ
   *  komunikaty, które kierowane s± na wyj¶cie "standard error"
   *  Domy¶lnie przymuje siê, ¿e programista nie chce dodatkwego wy¶wietlania
   *  komunikatów.
   */
  void WyswietlajKomunikatyBledow( bool Tryb = true );
   /*!
    * \brief  Dodaje nazwê pliku.
    *
    * Powoduje dodanie do listy plików zawierajacych dane dla \e gnuplota,
    * nowej nazwy pliku.
    *
    * \param  NazwaPliku  - nazwa pliku z danymi dla gnuplota.
    *
    * \retval true - je¿eli istnieje plik o nazwie udostêpnionej poprzez
    *            parametr
    *            \e NazwaPliku oraz jest zezwolenie na jego czytanie.
    *            Nazwa pliku zostaje dodana do listy plików z danymi 
    *            dla \e gnuplota.
    * \retval false - Je¿eli nie istnieje plik o nazwie przekazanej poprzez
    *            parametr \e NazwaPliku. 
    *            Nazwa pliku zostaje dodana do listy plików z danymi 
    *            dla \e gnuplota.
    */
   bool DodajNazwePliku( const char       * NazwaPliku, 
                         RodzajRysowania    RodzRys = RR_Ciagly, 
                         int                Szerokosc = 1
                       );

    /*!
     * \brief Tworzy listê parametrów umo¿liwiaj±cych rysowanie bry³ z plików.
     */
    bool DopiszRysowanieZPlikow( std::string &Polecenie, char const **Sep );

   /*!
    * \brief Informuje, czy po³±czenie z \e gnuplot'em jest zainicjalizowane.
    *
    * Informuje, czy po³±czenie z programem \e gnuplot jest zainicjowane.
    * \retval true - je¶li tak,
    * \retval false - w przypadku przeciwnym.
    */ 
    bool CzyPolaczenieJestZainicjowane() const;

  /*!
   *  Je¿eli lista plików nie jest pusta, to generuje sekwencje poleceñ
   *  dla programu \e gnuplot maj±ca na celu narysowanie p³aszczyzn na
   *  na podstawie danych zawartych w plikach z listy. 
   *
   *  \pre  Lista plików nie powinna byæ pusta. Nazwy plików na niej
   *        mo¿na umie¶ciæ za pomoca metody 
   *        \link LaczeDoGNUPlota::DodajNazwe DodajNazwe\endlink.
   *        Metoda nie wymaga wcze¶niejszego zainicjowania po³±czenia
   *        z \e gnuplotem.
   *  \retval true   - gdy zostaj± poprawnie wys³ane polecenia dla gnuplota.
   *                 Nie oznacza to jednak, ¿e proces rysowania zakoñczy³
   *                 siê pomy¶lnie.
   *  \retval false   - gdy po³±czenie z gnuplotem nie mo¿e zostaæ poprawnie
   *                 zainicjalizowane lub gdy lista plików jest pusta.
   */
  bool Rysuj();
  /*!
   *  Dzia³a analogicznie jak metoda 
   *  \link LaczeDoGNUPlota::Rysuj Rysuj\endlink, z t± ró¿nic±, ¿e 
   *  rysunek robota
   *  sk³adowany jest w pliku o nazwie przekazanej przez parametr 
   *  \e NazwaPliku.
   *  Rysunek jest zapisywany w formacie \e PNG.
   *
   *  \post Lista plików nie powinna byæ pusta ponadto powinno byæ
   *        mo¿liwe otwarcie do zapisu pliku o nazwie przekazanej przez
   *        parametr \e NazwaPliku, do której do³±czane jest rozszerzenie
   *        .ps .
   *        Metoda nie wymaga wcze¶niejszego zainicjowania po³±czenia
   *        z programem \e gnuplot. 
   *
   *  \retval true   - gdy zostaj± poprawnie wys³ane polecenia dla 
   *                 \e gnuplota.
   *                 Nie oznacza to jednak, ¿e proces rysowania zakoñczy³
   *                 siê pomy¶lnie.
   *  \retval false   - gdy po³±czenie z gnuplotem nie mo¿e zostaæ poprawnie
   *                 zainicjalizowane lub gdy lista plików jest pusta lub
   *                 te¿ gdy nie mo¿na otworzyæ pliku do zapisu.
   */
  bool RysujDoPliku(const char *NazwaPliku);
  /*!
   *  \brief  Inicjalizuje po³±czenie z programem \e gnuplot.
   *
   *  Inicjalizuje po³±czenie z programem \e gnuplot. Realizowane jest to
   *  poprzez rozwidlenie procesu i uruchomienie jako procesu potomnego
   *  programu \e gnuplot. Komunikacja z programem \e gnuplot realizowana jest
   *  poprzez przejêcie jego wej¶cia i wyj¶cia standardowego.
   *
   *  \retval true - gdy po³±czenie z programem \e 0gnuplot zosta³o poprawnie
   *               zainicjalizowane lub gdy ju¿ wcze¶niej by³o 
   *               zainicjalizowane.
   *  \retval false - gdy proces inicjalizacji po³±czenia zakoñczy³ siê
   *               niepowodzeniem.
   */
  bool Inicjalizuj();
  /*!
   *  \brief Usuwa ostatni± nazwê pliku.
   * 
   *  Usuwa ostatni± nazwê z listy nazw plików.
   */
  void UsunOstatniaNazwe();
  /*!
   *  \brief Kasuje zawarto¶æ listy nazw plików.
   * 
   *  Calkowicie kasuje zawarto¶æ listy nazw plików.
   */
  void UsunWszystkieNazwyPlikow();

  LaczeDoGNUPlota();
  virtual ~LaczeDoGNUPlota();
};

  inline
  bool LaczeDoGNUPlota::DopiszPlikiDoPoleceniaRysowania( std::string &,
                                                         char const  **
                                                       )
  { return true; }



}

#endif
