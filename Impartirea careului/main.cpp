#include <graphics.h>
#include <winbgim.h>

#include <cstdlib>

#include <cstring>

#include <windows.h>
#include <mmsystem.h>

#include <chrono>
#include <thread>

#define LATIME 1500
#define INALTIME 600
#define TITLU "Impartirea careului"

#define X_TABEL 100 ///latimea tabelului, pe axa xOy
#define Y_TABEL 100 ///inaltimea tabelului, pe axa xOy
#define LINII 9
#define COLOANE 9
#define LIBER 0
#define OCUPAT 9
#define PAGINA_PRINCIPALA 0 ///pagina meniului de start
#define PAGINA_INSTRUCTIUNI 1 ///pagina instructiunilor

int tabla[LINII][COLOANE];
static int nivelul1[9]= {31, 63, 72, 73, 76, 78, 83, 84},
                        nivelul2[9]= {46, 54, 65, 67, 75, 77, 78, 88},
                                     nivelul3[9]= {31, 33, 73, 74, 76, 81, 83, 84},
                                             nivelul4[9]= {12, 16, 17, 46, 51, 58, 86, 87},
                                                     nivelul5[9]= {11, 22, 24, 37, 38, 48, 57, 61},
                                                             nivelul6[9]= {14, 15, 17, 31, 32, 52, 58, 81},
                                                                     nivelul7[9]= {21, 23, 24, 28, 43, 51, 67, 76},
                                                                             nivelul8[9]= {33, 44, 45, 46, 54, 55, 63, 76};
///Vectorii, utilizati ca o matrice [8][2], ce stabilesc pozitiile exacte ale bulinelor in nivelele prestabilite
int paleta[12][8]= {COLOR(255, 0, 0), COLOR(255, 150, 50), COLOR(255, 225, 0), COLOR(0, 255, 0), COLOR(35, 225, 225), COLOR(0, 0, 255), COLOR(175, 0, 225), COLOR(250, 150, 200),
                    COLOR(239, 124, 142), COLOR(255, 163, 132), COLOR(239, 231, 188), COLOR(182, 226, 211), COLOR(66, 151, 160), COLOR(47, 80, 97), COLOR(216, 167, 177), COLOR(250, 232, 224),
                    COLOR(212, 30, 0), COLOR(4, 47, 89), COLOR(37, 175, 194), COLOR(202, 218, 227), COLOR(82, 58, 40), COLOR(222, 93, 104), COLOR(101, 70, 62), COLOR(198, 136, 121),
                    COLOR(255, 174, 188), COLOR(250, 195, 172), COLOR(251, 231, 198), COLOR(180, 248, 200), COLOR(160, 231, 229), COLOR(136, 123, 176), COLOR(205, 155, 128), COLOR(246, 81, 86),
                    COLOR(234, 133, 199), COLOR(255, 128, 0), COLOR(238, 255, 0), COLOR(5, 166, 58), COLOR(54, 238, 224), COLOR(219, 85, 212), COLOR(62, 0, 74), COLOR(93, 241, 93),
                    COLOR(243, 22, 30), COLOR(103, 41, 1), COLOR(247, 98, 1), COLOR(255, 222, 104), COLOR(255, 158, 79), COLOR(91, 112, 59), COLOR(11, 22, 38), COLOR(198, 136, 121),
                    COLOR(211, 205, 187), COLOR(93, 230, 28), COLOR(227, 168, 156), COLOR(180, 0, 2), COLOR(46, 118, 94), COLOR(223, 206, 204), COLOR(113, 74, 54), COLOR(216, 87, 64),
                    COLOR(93, 25, 102), COLOR(180, 151, 191), COLOR(252, 132, 31), COLOR(246, 205, 196), COLOR(92, 6, 1), COLOR(249, 249, 64), COLOR(209, 194, 194), COLOR(44, 44, 36),
                    COLOR(241, 210, 108), COLOR(43, 18, 0), COLOR(207, 92, 0), COLOR(141, 30, 0), COLOR(75, 68, 60), COLOR(77, 0, 17), COLOR(88, 168, 87), COLOR(191, 153, 107),
                    COLOR(67, 47, 112), COLOR(113, 55, 112), COLOR(179, 66, 112), COLOR(4, 52, 92), COLOR(16, 86, 79), COLOR(45, 64, 229), COLOR(252, 225, 104), COLOR(12, 20, 70),
                    COLOR(26, 86, 83), COLOR(16, 120, 105), COLOR(92, 216, 90), COLOR(8, 49, 58), COLOR(29, 198, 144), COLOR(39, 138, 176), COLOR(28, 70, 112), COLOR(234, 234, 224)
                   };
///Paletele sunt predefinite
int culoare[8]= {COLOR(255, 0, 0), COLOR(255, 150, 50), COLOR(255, 225, 0), COLOR(0, 255, 0), COLOR(35, 225, 225), COLOR(0, 0, 255), COLOR(175, 0, 225), COLOR(250, 150, 200)};
///Pastreaza culorile bulinelor intr-un vector => pot fi schimbate in setari
int FUNDAL=BLACK, SCRIS=WHITE, TABELE=COLOR(255, 225, 0), SELECTARI=COLOR(255, 0, 0);
///Fundalul este predefinit negru, scrisul alb, tabelele/meniurile galbene, iar scrisul evidentiat/butoanele selectate rosii

void castig() ///Afiseaza mesajul de castig
{
    using namespace std::this_thread;
    using namespace std::chrono;

    setcolor(SELECTARI); ///Seteaza culoarea de scriere
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);
    outtextxy(100, 550, "AI CASTIGAT! :D");
    printf("Ati castigat jocul. Speram ca va placut! :) \n");
    sleep_for(seconds(5));
    closegraph(CURRENT_WINDOW);
}

void fereastra_inchisa(int FEREASTRA_PRINCIPALA) ///Functia este apelata atunci cand se apasa butonul "MENIU" in timpul jocului
{
    cleardevice();
    closegraph(CURRENT_WINDOW);
    setcurrentwindow(FEREASTRA_PRINCIPALA); ///Aducem fereastra principala in fata
    printf("Ati inchis o fereastra si ati revenit la pagina principala. :( \n");
}

void deseneaza_tabel()
{
    setcolor(SCRIS); ///Seteaza culoarea de desen
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    line(X_TABEL, Y_TABEL, 400+X_TABEL, Y_TABEL); ///Traseaza linia de sus
    line(X_TABEL, Y_TABEL, X_TABEL, 400+Y_TABEL); ///Traseaza linia din stanga
    line(X_TABEL, 400+Y_TABEL, 400+X_TABEL, 400+Y_TABEL); ///Traseaza linia de jos
    line(400+X_TABEL, Y_TABEL, 400+X_TABEL, 400+Y_TABEL); ///Traseaza linia din dreapta

    for (int i=50; i<=400; i=i+50)
        ///Fiecare latura a patratelor este de 50
    {
        line(X_TABEL, Y_TABEL+i, 400+X_TABEL, Y_TABEL+i); ///Traseaza liniile orizontale
        line(X_TABEL+i, Y_TABEL, X_TABEL+i, 400+Y_TABEL); ///Traseaza liniile verticale
    }

    char linie[2]; ///Trebuie sa contina si caracterul NULL
    for(int i=115, c=1; c<9; i=i+50, c++) ///Numeroteaza linii
    {
        itoa(c, linie, 10); ///Converteste numarul c intr-un sir de caractere, linie, in baza 10
        outtextxy(70, i, linie);
    }
    for(int i=115, c=1; c<9; i=i+50, c++) ///Numeroteaza coloane
    {
        itoa(c, linie, 10);
        outtextxy(i, 70, linie);
    }
}

void butoane() ///Deseneaza butoanele in timpul jocului
{
    setcolor(SCRIS);
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    int pozitie_buton=15, x, y;
    outtextxy(pozitie_buton, pozitie_buton, "Meniu");
    ///Butonul "MENIU" intoarce utilizatorul la pagina principala => echivalent unui "NEW GAME"
    rectangle(pozitie_buton-5, pozitie_buton-5, pozitie_buton+textwidth("Meniu")+5, pozitie_buton+textheight("Meniu")+5);

    outtextxy(pozitie_buton+textwidth("Meniu")+15, pozitie_buton, "Reset");
    ///Butonul "RESET" sterge toate patratele selectate de utilizator => NU sterge nivelul selectat, sau tabla
    rectangle(pozitie_buton+textwidth("Meniu")+10, pozitie_buton-5, pozitie_buton+textwidth("Meniu")+textwidth("Reset")+20, pozitie_buton+textheight("Reset")+5);
}

void butoane_nivele() ///Creeaza butoanele nivelelor
{
    setcolor(SCRIS);
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    rectangle(600, 100, 610+textwidth("NIVELUL 8"), 150);
    outtextxy(605, 105, "NIVELUL 1");

    rectangle(600, 150, 610+textwidth("NIVELUL 8"), 200);
    outtextxy(605, 155, "NIVELUL 2");

    rectangle(600, 200, 610+textwidth("NIVELUL 8"), 250);
    outtextxy(605, 205, "NIVELUL 3");

    rectangle(600, 250, 610+textwidth("NIVELUL 8"), 300);
    outtextxy(605, 255, "NIVELUL 4");

    rectangle(600, 300, 610+textwidth("NIVELUL 8"), 350);
    outtextxy(605, 305, "NIVELUL 5");

    rectangle(600, 350, 610+textwidth("NIVELUL 8"), 400);
    outtextxy(605, 355, "NIVELUL 6");

    rectangle(600, 400, 610+textwidth("NIVELUL 8"), 450);
    outtextxy(605, 405, "NIVELUL 7");

    rectangle(600, 450, 610+textwidth("NIVELUL 8"), 500);
    outtextxy(605, 455, "NIVELUL 8");
}

void deseneaza_buline(int nivel[9])
///Functia ia valorile prestabilite, pastrate intr-un vector pentru a desena bulinele in acelasi loc mereu
{
    setcolor(SCRIS);
    for(int i=0; i<8; i++)
        ///Vectorii sunt initializati automat de la 0, nu de la 1
    {
        tabla[nivel[i]/10][nivel[i]%10]=OCUPAT;
        ///nivel[i]/10 => prima cifra (i in parcurgerea matricelor)
        ///nivel[i]%10 => a doua cifra (j in parcurgerea matricelor)
        circle(X_TABEL+(nivel[i]%10)*50-25, Y_TABEL+(nivel[i]/10)*50-25, 20);
        setfillstyle(SOLID_FILL, FUNDAL); ///Nu pot ramane aceeasi culoare ca scrisul, fiindca altfel nu vor fi recolorate (marginile au aceeasi culoare)
        fillellipse(X_TABEL+(nivel[i]%10)*50-25, Y_TABEL+(nivel[i]/10)*50-25, 20, 20);
    }
}

int* nivel_curent() ///"Evidentiaza" nivelul selectat de utilizator
{
    setcolor(SELECTARI); ///Seteaza culoarea de scriere
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    int x, y;
    while(true) ///Verifica constant click-urile utilizatorului
    {
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>=600 && x<=610+textwidth("NIVELUL 8") && y<=150 && y>=100)
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            outtextxy(605, 105, "NIVELUL 1");
            deseneaza_buline(nivelul1);
            printf("Ati selectat nivelul 1. Mult succes! :D \n");
            return nivelul1; ///Cand a fost selectat un nivel => iese din functie si continua cu programul
        }

        if(x>=600 && x<=610+textwidth("NIVELUL 8") && y<=200 && y>=150)
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            outtextxy(605, 155, "NIVELUL 2");
            deseneaza_buline(nivelul2);
            printf("Ati selectat nivelul 2. Mult succes! :D \n");
            return nivelul2;
        }

        if(x>=600 && x<=610+textwidth("NIVELUL 8") && y<=250 && y>=200)
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            outtextxy(605, 205, "NIVELUL 3");
            deseneaza_buline(nivelul3);
            printf("Ati selectat nivelul 3. Mult succes! :D \n");
            return nivelul3;
        }

        if(x>=600 && x<=610+textwidth("NIVELUL 8") && y<=300 && y>=250)
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            outtextxy(605, 255, "NIVELUL 4");
            deseneaza_buline(nivelul4);
            printf("Ati selectat nivelul 4. Mult succes! :D \n");
            return nivelul4;
        }

        if(x>=600 && x<=610+textwidth("NIVELUL 8") && y<=350 && y>=300)
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            outtextxy(605, 305, "NIVELUL 5");
            deseneaza_buline(nivelul5);
            printf("Ati selectat nivelul 5. Mult succes! :D \n");
            return nivelul5;
        }

        if(x>=600 && x<=610+textwidth("NIVELUL 8") && y<=400 && y>=350)
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            outtextxy(605, 355, "NIVELUL 6");
            deseneaza_buline(nivelul6);
            printf("Ati selectat nivelul 6. Mult succes! :D \n");
            return nivelul6;
        }

        if(x>=600 && x<=610+textwidth("NIVELUL 8") && y<=450 && y>=400)
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            outtextxy(605, 405, "NIVELUL 7");
            deseneaza_buline(nivelul7);
            printf("Ati selectat nivelul 7. Mult succes! :D \n");
            return nivelul7;
        }

        if(x>=600 && x<=610+textwidth("NIVELUL 8") && y<=500 && y>=450)
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            outtextxy(605, 455, "NIVELUL 8");
            deseneaza_buline(nivelul8);
            printf("Ati selectat nivelul 8. Mult succes! :D \n");
            return nivelul8;
        }
    }
}

void reset_nivele(int* nivel) ///Functia sterge toata activitatea utilizatorului, pentru modul de joc "NIVELE"
{
    setbkcolor(FUNDAL);
    cleardevice();

    for(int i=0; i<=8; i++)
    {
        for(int j=0; j<=8; j++)
        {
            tabla[i][j]=LIBER;
        }
    }

    deseneaza_tabel();
    butoane_nivele();

    setcolor(SELECTARI); ///Seteaza culoarea de scriere
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    if(nivel==nivelul1)
    {
        outtextxy(605, 105, "NIVELUL 1");
        deseneaza_buline(nivelul1);
    }

    if(nivel==nivelul2)
    {
        outtextxy(605, 155, "NIVELUL 2");
        deseneaza_buline(nivelul2);
    }

    if(nivel==nivelul3)
    {
        outtextxy(605, 205, "NIVELUL 3");
        deseneaza_buline(nivelul3);
    }

    if(nivel==nivelul4)
    {
        outtextxy(605, 255, "NIVELUL 4");
        deseneaza_buline(nivelul4);
    }

    if(nivel==nivelul5)
    {
        outtextxy(605, 305, "NIVELUL 5");
        deseneaza_buline(nivelul5);
    }

    if(nivel==nivelul6)
    {
        outtextxy(605, 355, "NIVELUL 6");
        deseneaza_buline(nivelul6);
    }

    if(nivel==nivelul7)
    {
        outtextxy(605, 405, "NIVELUL 7");
        deseneaza_buline(nivelul7);
    }

    if(nivel==nivelul8)
    {
        outtextxy(605, 455, "NIVELUL 8");
        deseneaza_buline(nivelul8);
    }

    butoane();
}

void reset(int* nivel) ///Functia sterge toata activitatea utilizatorului, pentru modurile de joc "RANDOM" si "CUSTOM"
{
    setbkcolor(FUNDAL);
    cleardevice();

    for(int i=0; i<=8; i++)
    {
        for(int j=0; j<=8; j++)
        {
            tabla[i][j]=LIBER;
        }
    }

    deseneaza_tabel();
    deseneaza_buline(nivel);
    butoane();
}

void nivele(int FEREASTRA_PRINCIPALA) ///Creeaza fereastra pentru modul de joc "NIVELE", avand nevoie si de fereastra principala pentru a se intoarce
{
    setbkcolor(FUNDAL);
    cleardevice();

    for(int i=0; i<=8; i++)
    {
        for(int j=0; j<=8; j++)
        {
            tabla[i][j]=LIBER;
        }
    }

    deseneaza_tabel();
    butoane_nivele();
    int* nivel=nivel_curent();
    butoane();

    int pozitie_buton=15, x, y, linie, coloana, contor=0, patrat=8, verif_castig=0, click_bulina=0;
    while(true)
    {
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>=(pozitie_buton-5) && x<=(pozitie_buton+textwidth("Meniu")+5) && y<=(pozitie_buton+textheight("Meniu")+5) && y>=(pozitie_buton-5))
            ///Butonul "MENIU" a fost apasat
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            fereastra_inchisa(FEREASTRA_PRINCIPALA);
            return;
        }

        if(x>=(pozitie_buton+textwidth("Meniu")+10) && x<=(pozitie_buton+textwidth("Meniu")+textwidth("Reset")+20) && y<=(pozitie_buton+textheight("Reset")+5) && y>=(pozitie_buton-5))
            ///Butonul "RESET" a fost apasat
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            printf("Ati resetat activitatea. :/ \n");
            contor=0;
            patrat=8;
            verif_castig=0;
            click_bulina=0;
            reset_nivele(nivel);
        }

        if(x>=100 && x<=500 && y<=500 && y>=100)
            ///A fost apasat in interiorul tablei de joc
        {
            linie=(y-Y_TABEL)/50+1, coloana=(x-X_TABEL)/50+1;

            if(click_bulina==0 && tabla[linie][coloana]==OCUPAT)
                ///Schimba valoarea memorata in tabel, daca s-a dat click pe o bulina
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati colorat bulina de pe linia %d si coloana %d. \n", linie, coloana);

                click_bulina=1;
                ///Nu permite sa se dea click pe mai multe buline in acelasi timp

                setcolor(culoare[contor]);
                setfillstyle(SOLID_FILL, culoare[contor]);
                floodfill((x-X_TABEL)/50*50+125, (y-Y_TABEL)/50*50+125, SCRIS);

                tabla[linie][coloana]=culoare[contor]; ///Pentru a verifica ca acest patrat este legat de altele de aceeasi culoare
                for(int i=1; i<3; i++)
                {
                    rectangle((x-X_TABEL)/50*50+X_TABEL+i, (y-Y_TABEL)/50*50+Y_TABEL+i, (x-X_TABEL)/50*50+X_TABEL+50-i, (y-Y_TABEL)/50*50+Y_TABEL+50-i);
                    ///Face un patrat de grosime 2, in functie de coordonatele click-ului, folosind o formula ca sa aproximeze marginile celulei din tabel
                }
                patrat--;
            }

            if(tabla[linie][coloana-1]==culoare[contor] || tabla[linie-1][coloana]==culoare[contor] || tabla[linie+1][coloana]==culoare[contor] || tabla[linie][coloana+1]==culoare[contor])
                ///Verifica daca tabla[linie][coloana] are macar un vecin de aceeasi culoare
            {
                if(tabla[linie][coloana]==LIBER)
                    ///Daca pe patratul unde am dat click, inca nu exista o culoare, desenam acum
                {
                    PlaySound("Click.wav", NULL, SND_ASYNC);
                    printf("Ati colorat patratul de pe linia %d si coloana %d. \n", linie, coloana);

                    tabla[linie][coloana]=culoare[contor];
                    for(int i=1; i<3; i++)
                    {
                        rectangle((x-X_TABEL)/50*50+X_TABEL+i, (y-Y_TABEL)/50*50+Y_TABEL+i, (x-X_TABEL)/50*50+X_TABEL+50-i, (y-Y_TABEL)/50*50+Y_TABEL+50-i);
                        ///Face un patrat de grosime 2, in functie de coordonatele click-ului, folosind o formula ca sa aproximeze marginile celulei din tabel
                    }
                    patrat--;
                    verif_castig++;
                }
            }

            if(verif_castig==56)
                ///Tabla are 64 de patrate, dintre care 8 sunt ocupate cu bulinele => daca restul 56 au fost colorate, inseamna ca jocul a fost castigat
            {
                castig();
                printf("Ati revenit la pagina principala. \n");
                setcurrentwindow(FEREASTRA_PRINCIPALA); ///Aducem fereastra principala in fata
                return;
            }

            if(patrat==0)
                ///Trebuie colorate toate patratele pentru a trece la urmatoarea culoare
            {
                contor++; ///Trece la urmatoarea culoare
                patrat=8; ///Avem voie sa coloram doar 7 patrate, avand 8 impreuna cu bulina
                click_bulina=0; ///Permite sa trecem la alta bulina, de alta culoare
            }
        }
    }
}

int* genereaza_buline() ///Functia genereaza aleatoriu pozitiile a 8 buline
{
    static int nivel[9];
    for(int i=0; i<8; i++)
    {
        nivel[i]=0;
    }

    int i=0, contor=0, linie, coloana;
    while(contor<8)
        ///Genereaza numere continuu pana cand sunt puse pe tabla doar 8 buline
    {
        linie=(rand()%400)/50+1, coloana=(rand()%400)/50+1; ///Genereaza numere de la 0 la 400
        ///Pozitia in tablou poate fi numai un numar de la 1 la 8 inclusiv, dupa formula

        if(tabla[linie][coloana]==LIBER) ///Se asigura ca se pun bulinele numai pe pozitii diferite
        {
            nivel[contor]=linie*10+coloana;

            setcolor(SCRIS);
            tabla[nivel[contor]/10][nivel[contor]%10]=OCUPAT;
            circle(X_TABEL+(nivel[contor]%10)*50-25, Y_TABEL+(nivel[contor]/10)*50-25, 20);
            setfillstyle(SOLID_FILL, FUNDAL); ///Nu pot ramane aceeasi culoare ca scrisul, fiindca altfel nu vor fi recolorate (marginile au aceeasi culoare)
            fillellipse(X_TABEL+(nivel[contor]%10)*50-25, Y_TABEL+(nivel[contor]/10)*50-25, 20, 20);

            contor++;
        }
    }
    return nivel;
}

void random(int FEREASTRA_PRINCIPALA) ///Creeaza fereastra pentru modul de joc "RANDOM", avand nevoie si de fereastra principala pentru a se intoarce
{
    setbkcolor(FUNDAL);
    cleardevice();

    for(int i=0; i<=8; i++)
    {
        for(int j=0; j<=8; j++)
        {
            tabla[i][j]=LIBER;
        }
    }

    deseneaza_tabel();
    int* nivel=genereaza_buline();
    deseneaza_buline(nivel);
    butoane();

    int pozitie_buton=15, x, y, linie, coloana, contor=0, patrat=8, verif_castig=0, click_bulina=0;
    while(true)
    {
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>=(pozitie_buton-5) && x<=(pozitie_buton+textwidth("Meniu")+5) && y<=(pozitie_buton+textheight("Meniu")+5) && y>=(pozitie_buton-5))
            ///Butonul "MENIU" a fost apasat
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            fereastra_inchisa(FEREASTRA_PRINCIPALA);
            return;
        }

        if(x>=(pozitie_buton+textwidth("Meniu")+10) && x<=(pozitie_buton+textwidth("Meniu")+textwidth("Reset")+20) && y<=(pozitie_buton+textheight("Reset")+5) && y>=(pozitie_buton-5))
            ///Butonul "RESET" a fost apasat
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            printf("Ati resetat activitatea. :/ \n");
            contor=0;
            patrat=8;
            verif_castig=0;
            click_bulina=0;
            reset(nivel);
        }

        if(x>=100 && x<=500 && y<=500 && y>=100)
            ///A fost apasat in interiorul tablei de joc
        {
            linie=(y-Y_TABEL)/50+1, coloana=(x-X_TABEL)/50+1;

            if(click_bulina==0 && tabla[linie][coloana]==OCUPAT)
                ///Schimba valoarea memorata in tabel, daca s-a dat click pe o bulina
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati colorat bulina de pe linia %d si coloana %d. \n", linie, coloana);

                click_bulina=1;
                ///Nu permite sa se dea click pe mai multe buline in acelasi timp

                setcolor(culoare[contor]);
                setfillstyle(SOLID_FILL, culoare[contor]);
                floodfill((x-X_TABEL)/50*50+125, (y-Y_TABEL)/50*50+125, SCRIS);

                tabla[linie][coloana]=culoare[contor]; ///Pentru a verifica ca acest patrat este legat de altele de aceeasi culoare
                for(int i=1; i<3; i++)
                {
                    rectangle((x-X_TABEL)/50*50+X_TABEL+i, (y-Y_TABEL)/50*50+Y_TABEL+i, (x-X_TABEL)/50*50+X_TABEL+50-i, (y-Y_TABEL)/50*50+Y_TABEL+50-i);
                    ///Face un patrat de grosime 2, in functie de coordonatele click-ului, folosind o formula ca sa aproximeze marginile celulei din tabel
                }
                patrat--;
            }

            if(tabla[linie][coloana-1]==culoare[contor] || tabla[linie-1][coloana]==culoare[contor] || tabla[linie+1][coloana]==culoare[contor] || tabla[linie][coloana+1]==culoare[contor])
                ///Verifica daca tabla[linie][coloana] are macar un vecin de aceeasi culoare
            {
                if(tabla[linie][coloana]==LIBER)
                    ///Daca pe patratul unde am dat click, inca nu exista o culoare, desenam acum
                {
                    PlaySound("Click.wav", NULL, SND_ASYNC);
                    printf("Ati colorat patratul de pe linia %d si coloana %d. \n", linie, coloana);

                    tabla[linie][coloana]=culoare[contor];
                    for(int i=1; i<3; i++)
                    {
                        rectangle((x-X_TABEL)/50*50+X_TABEL+i, (y-Y_TABEL)/50*50+Y_TABEL+i, (x-X_TABEL)/50*50+X_TABEL+50-i, (y-Y_TABEL)/50*50+Y_TABEL+50-i);
                    }
                    patrat--;
                    verif_castig++;
                }
            }

            if(verif_castig==56)
                ///Tabla are 64 de patrate, dintre care 8 sunt ocupate cu bulinele => daca restul 56 au fost colorate, inseamna ca jocul a fost castigat
            {
                castig();
                printf("Ati revenit la pagina principala. \n");
                setcurrentwindow(FEREASTRA_PRINCIPALA); ///Aducem fereastra principala in fata
                return;
            }

            if(patrat==0)
                ///Trebuie colorate toate patratele pentru a trece la urmatoarea culoare
            {
                contor++; ///Trece la urmatoarea culoare
                patrat=8; ///Avem voie sa coloram doar 7 patrate, avand 8 impreuna cu bulina
                click_bulina=0; ///Permite sa trecem la alta bulina, de alta culoare
            }
        }
    }
}

int* input_buline() ///Functia pastreaza in memorie pozitiile unde au fost asezate bulinele
{
    static int nivel[9];
    for(int i=0; i<8; i++)
    {
        nivel[i]=0;
    }

    int x_left, y_left, x_right, y_right, linie, coloana, contor=0;
    while(true)
    {
        getmouseclick(WM_LBUTTONDOWN, x_left, y_left);
        getmouseclick(WM_RBUTTONDOWN, x_right, y_right);

        if(x_left>=100 && x_left<=500 && y_left<=500 && y_left>=100)
            ///A fost apasat in interiorul tablei de joc
        {
            setcolor(SCRIS);
            linie=(y_left-Y_TABEL)/50+1, coloana=(x_left-X_TABEL)/50+1;

            if(tabla[linie][coloana]==LIBER) ///Se asigura ca se pun bulinele numai pe pozitii diferite
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                nivel[contor]=linie*10+coloana;

                tabla[nivel[contor]/10][nivel[contor]%10]=OCUPAT;
                circle(X_TABEL+(nivel[contor]%10)*50-25, Y_TABEL+(nivel[contor]/10)*50-25, 20);
                setfillstyle(SOLID_FILL, FUNDAL); ///Nu pot ramane aceeasi culoare ca scrisul, fiindca altfel nu vor fi recolorate (marginile au aceeasi culoare)
                fillellipse(X_TABEL+(nivel[contor]%10)*50-25, Y_TABEL+(nivel[contor]/10)*50-25, 20, 20);

                contor++;
            }
        }

        if(x_right>=100 && x_right<=500 && y_right<=500 && y_right>=100)
            ///A fost apasat in interiorul tablei de joc
        {
            setcolor(FUNDAL);
            linie=(y_right-Y_TABEL)/50+1, coloana=(x_right-X_TABEL)/50+1;

            if(tabla[linie][coloana]==OCUPAT) ///Se asigura ca se poate sterge numai unde este deja o bulina
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                for(int i=0; i<contor-1; i++)
                {
                    if(nivel[i]==linie*10+coloana)
                    {
                        for(int j=i+1; j<contor; j++)
                        {
                            nivel[j-1]=nivel[j];
                        }
                        nivel[contor-1]=0;
                    }
                }

                tabla[linie][coloana]=LIBER;
                circle(X_TABEL+(coloana)*50-25, Y_TABEL+(linie)*50-25, 20);
                setfillstyle(SOLID_FILL, FUNDAL); ///Se coloreaza peste buline culoarea fundalului, stergand in acelasi timp urma lor
                floodfill(coloana, linie, SCRIS);

                contor--;
            }
        }

        if(contor==8) ///Se asigura ca se pun doar 8 buline pe tabla
        {
            return nivel;
        }
    }
}

void custom(int FEREASTRA_PRINCIPALA) ///Creeaza fereastra pentru modul de joc "CUSTOM", avand nevoie si de fereastra principala pentru a se intoarce
{
    setbkcolor(FUNDAL);
    cleardevice();

    for(int i=0; i<=8; i++)
    {
        for(int j=0; j<=8; j++)
        {
            tabla[i][j]=LIBER;
        }
    }

    deseneaza_tabel();
    int* nivel=input_buline();
    reset(nivel);

    int pozitie_buton=15, x, y, linie, coloana, contor=0, patrat=8, verif_castig=0, click_bulina=0;
    while(true)
    {
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>=(pozitie_buton-5) && x<=(pozitie_buton+textwidth("Meniu")+5) && y<=(pozitie_buton+textheight("Meniu")+5) && y>=(pozitie_buton-5))
            ///Butonul "MENIU" a fost apasat
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            fereastra_inchisa(FEREASTRA_PRINCIPALA);
            return;
        }

        if(x>=(pozitie_buton+textwidth("Meniu")+10) && x<=(pozitie_buton+textwidth("Meniu")+textwidth("Reset")+20) && y<=(pozitie_buton+textheight("Reset")+5) && y>=(pozitie_buton-5))
            ///Butonul "RESET" a fost apasat
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            printf("Ati resetat activitatea. :/ \n");
            contor=0;
            patrat=8;
            verif_castig=0;
            click_bulina=0;
            reset(nivel);
        }

        if(x>=100 && x<=500 && y<=500 && y>=100)
            ///A fost apasat in interiorul tablei de joc
        {
            linie=(y-Y_TABEL)/50+1, coloana=(x-X_TABEL)/50+1;

            if(click_bulina==0 && tabla[linie][coloana]==OCUPAT)
                ///Schimba valoarea memorata in tabel, daca s-a dat click pe o bulina
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati colorat bulina de pe linia %d si coloana %d. \n", linie, coloana);

                click_bulina=1;
                ///Nu permite sa se dea click pe mai multe buline in acelasi timp

                setcolor(culoare[contor]);
                setfillstyle(SOLID_FILL, culoare[contor]);
                floodfill((x-X_TABEL)/50*50+125, (y-Y_TABEL)/50*50+125, SCRIS);

                tabla[linie][coloana]=culoare[contor]; ///Pentru a verifica ca acest patrat este legat de altele de aceeasi culoare
                for(int i=1; i<3; i++)
                {
                    rectangle((x-X_TABEL)/50*50+X_TABEL+i, (y-Y_TABEL)/50*50+Y_TABEL+i, (x-X_TABEL)/50*50+X_TABEL+50-i, (y-Y_TABEL)/50*50+Y_TABEL+50-i);
                    ///Face un patrat de grosime 2, in functie de coordonatele click-ului, folosind o formula ca sa aproximeze marginile celulei din tabel
                }
                patrat--;
            }

            if(tabla[linie][coloana-1]==culoare[contor] || tabla[linie-1][coloana]==culoare[contor] || tabla[linie+1][coloana]==culoare[contor] || tabla[linie][coloana+1]==culoare[contor])
                ///Verifica daca tabla[linie][coloana] are macar un vecin de aceeasi culoare
            {
                if(tabla[linie][coloana]==LIBER)
                    ///Daca pe patratul unde am dat click, inca nu exista o culoare, desenam acum
                {
                    PlaySound("Click.wav", NULL, SND_ASYNC);
                    printf("Ati colorat patratul de pe linia %d si coloana %d. \n", linie, coloana);

                    tabla[linie][coloana]=culoare[contor];
                    for(int i=1; i<3; i++)
                    {
                        rectangle((x-X_TABEL)/50*50+X_TABEL+i, (y-Y_TABEL)/50*50+Y_TABEL+i, (x-X_TABEL)/50*50+X_TABEL+50-i, (y-Y_TABEL)/50*50+Y_TABEL+50-i);
                    }
                    patrat--;
                    verif_castig++;
                }
            }

            if(verif_castig==56)
                ///Tabla are 64 de patrate, dintre care 8 sunt ocupate cu bulinele => daca restul 56 au fost colorate, inseamna ca jocul a fost castigat
            {
                castig();
                printf("Ati revenit la pagina principala. \n");

                setcurrentwindow(FEREASTRA_PRINCIPALA); ///Aducem fereastra principala in fata
                return;
            }

            if(patrat==0)
                ///Trebuie colorate toate patratele pentru a trece la urmatoarea culoare
            {
                contor++; ///Trece la urmatoarea culoare
                patrat=8; ///Avem voie sa coloram doar 7 patrate, avand 8 impreuna cu bulina
                click_bulina=0; ///Permite sa trecem la alta bulina, de alta culoare
            }
        }
    }
}

void instructiuni() ///Creeaza pagina pentru instructiuni
{
    setactivepage(PAGINA_INSTRUCTIUNI);
    setvisualpage(PAGINA_INSTRUCTIUNI);
    cleardevice();

    setcolor(SCRIS);
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    int pozitie_buton=15, x, y;
    outtextxy(pozitie_buton, pozitie_buton, "Meniu");
    rectangle(pozitie_buton-5, pozitie_buton-5, pozitie_buton+textwidth("Meniu")+5, pozitie_buton+textheight("Meniu")+5);

    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 2);
    outtextxy(15, 4*pozitie_buton, "Pentru a incepe jocul, apasati butonul \"START\" si alegeti un mod de joc.");
    outtextxy(15, 6*pozitie_buton, "Cand jocul incepe, apasati pe o bulina si colorati 8 patrate continue, utilizand <CLICK STANGA>.");
    outtextxy(15, 8*pozitie_buton, "Jocul se termina cand este completat careul.");
    outtextxy(15, 12*pozitie_buton, "Modurile de joc sunt:");
    outtextxy(75, 14*pozitie_buton, "\"NIVELE\" => Mod de joc unde poate fi aleasa dificultatea (de la 1 la 8), pozitiile bulinelor fiind prestabilite");
    outtextxy(75, 16*pozitie_buton, "\"RANDOM\" => Mod de joc unde pozitionarea bulinelor este mereu generata aleatoriu");
    outtextxy(75+textwidth("\"RANDOM\" "), 18*pozitie_buton, "=> Exista posibilitatea ca jocul sa nu poata fi rezolvat");
    outtextxy(75, 20*pozitie_buton, "\"CUSTOM\" => Mod de joc unde poate fi aleasa pozitionarea bulinelor");
    outtextxy(75+textwidth("\"CUSTOM\" "), 22*pozitie_buton, "=> <CLICK STANGA> - pozitioneaza bulina");
    outtextxy(75+textwidth("\"CUSTOM\" "), 24*pozitie_buton, "=> <CLICK DREAPTA> - sterge bulina");
    outtextxy(15, 28*pozitie_buton, "Alte butoane:");
    outtextxy(75, 30*pozitie_buton, "\"MENIU\" => Permite intoarcerea la pagina principala");
    outtextxy(75, 32*pozitie_buton, "\"RESET\" => Permite resetarea patratelelor colorate de utilizator");

    while(true)
    {
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(x>=(pozitie_buton-5) && x<=(pozitie_buton+textwidth("Meniu")+5) && y<=(pozitie_buton+textheight("Meniu")+5) && y>=(pozitie_buton-5))
        {
            PlaySound("Click.wav", NULL, SND_ASYNC);
            printf("Ati revenit la pagina principala. \n");
            setactivepage(PAGINA_PRINCIPALA);
            setvisualpage(PAGINA_PRINCIPALA);
            return;
        }
    }
}

struct coordonata_x
{
    int left, right;
};
struct coordonata_y
{
    int top, bottom;
};

void deseneaza_pagina_principala(coordonata_x x_coord[23], coordonata_y y_coord[23]) ///Functia se ocupa cu desenul meniului
{
    setcolor(SCRIS); ///Seteaza culoarea de desen a butoanelor
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    int x=15, y=15;
    for(int i=0; i<4; i++) ///Afla coordonatele butoanelor "INSTRUCTIUNI", "START", "SETARI" si "EXIT"
    {
        if(i==0)
        {
            outtextxy(x, y, "INSTRUCTIUNI");
        }
        if(i==1)
        {
            outtextxy(x, y, "START");
        }
        if(i==2)
        {
            outtextxy(x, y, "EXIT");
        }
        if(i==3)
        {
            outtextxy(x, y, "SETARI");
        }

        x_coord[i].left=x-5;
        x_coord[i].right=x+textwidth("INSTRUCTIUNI")+5;
        y_coord[i].top=y-5;
        y_coord[i].bottom=y+textheight("INSTRUCTIUNI")+5;

        rectangle(x-5, y-5, x+textwidth("INSTRUCTIUNI")+5, y+textheight("INSTRUCTIUNI")+5);
        y=y+textheight("INSTRUCTIUNI")+10;
    }
}

void deseneaza_meniu_start(coordonata_x x_coord[23], coordonata_y y_coord[23]) ///Functia se ocupa cu desenul meniului pentru modurile de joc
{
    cleardevice();
    deseneaza_pagina_principala(x_coord, y_coord);

    setcolor(SCRIS); ///Seteaza culoarea de desen a butoanelor
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    line(x_coord[1].right+20, y_coord[1].bottom, x_coord[1].right+20, y_coord[1].top); ///Deseneaza sageata catre selectia modului de joc
    line(x_coord[1].right+20, y_coord[1].top, x_coord[1].right+40, (y_coord[1].bottom+y_coord[1].top)/2);
    line(x_coord[1].right+20, y_coord[1].bottom, x_coord[1].right+40, (y_coord[1].bottom+y_coord[1].top)/2);

    setcolor(TABELE); ///Seteaza culoarea de desen a butoanelor

    int x=x_coord[1].right+60, y=15;
    for(int i=4; i<7; i++) ///Afla coordonatele butoanelor "NIVELE", "RANDOM" si "CUSTOM"
    {
        if(i==4)
        {
            outtextxy(x, y, "NIVELE");
        }
        if(i==5)
        {
            outtextxy(x, y, "RANDOM");
        }
        if(i==6)
        {
            outtextxy(x, y, "CUSTOM");
        }
        x_coord[i].left=x-5;
        x_coord[i].right=x+textwidth("RANDOM")+5;
        y_coord[i].top=y-5;
        y_coord[i].bottom=y+textheight("RANDOM")+5;

        rectangle(x-5, y-5, x+textwidth("RANDOM")+5, y+textheight("RANDOM")+5);
        y=y+textheight("RANDOM")+10;
    }
}

void deseneaza_meniu_setari(coordonata_x x_coord[22], coordonata_y y_coord[22]) ///Functia se ocupa cu desenul meniului pentru setari
///Tema Intunecata: Fundal: BLACK // Scris: WHITE // Tabele: COLOR(255, 225, 0), galben // Selectari: COLOR(255, 0, 0), rosu // Hover: COLOR(255, 0, 115), roz
///Tema Luminoasa: Fundal: WHITE // Scris: BLACK // Tabele: COLOR(0, 30, 255), albastru // Selectari: COLOR(0, 255, 255), cyan // Hover: COLOR(0, 255, 140), verde
{
    cleardevice();
    deseneaza_pagina_principala(x_coord, y_coord);

    setcolor(SCRIS); ///Seteaza culoarea de desen a butoanelor
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    line(x_coord[3].right+20, y_coord[3].bottom, x_coord[3].right+20, y_coord[3].top); ///Deseneaza sageata catre selectia setarii
    line(x_coord[3].right+20, y_coord[3].top, x_coord[3].right+40, (y_coord[3].bottom+y_coord[3].top)/2);
    line(x_coord[3].right+20, y_coord[3].bottom, x_coord[3].right+40, (y_coord[3].bottom+y_coord[3].top)/2);

    setcolor(TABELE); ///Seteaza culoarea de desen a butoanelor

    int x=x_coord[3].right+60, y=15+(textheight("INSTRUCTIUNI")+10)*3;
    for(int i=7; i<9; i++) ///Afla coordonatele butoanelor "TEMA" si "PALETA DE CULORI"
    {
        if(i==7)
        {
            outtextxy(x, y, "TEMA");
        }
        if(i==8)
        {
            outtextxy(x, y, "PALETA DE CULORI");
        }
        x_coord[i].left=x-5;
        x_coord[i].right=x+textwidth("PALETA DE CULORI")+5;
        y_coord[i].top=y-5;
        y_coord[i].bottom=y+textheight("PALETA DE CULORI")+5;

        rectangle(x-5, y-5, x+textwidth("PALETA DE CULORI")+5, y+textheight("PALETA DE CULORI")+5);
        y=y+textheight("PALETA DE CULORI")+10;
    }
}

void deseneaza_meniu_tema(coordonata_x x_coord[23], coordonata_y y_coord[23]) ///Functia se ocupa cu desenul meniului pentru selectia temei
{
    cleardevice();
    deseneaza_pagina_principala(x_coord, y_coord);
    deseneaza_meniu_setari(x_coord, y_coord);

    setcolor(SCRIS); ///Seteaza culoarea de desen a butoanelor
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    line(x_coord[7].right+20, y_coord[7].bottom, x_coord[7].right+20, y_coord[7].top); ///Deseneaza sageata catre selectia temei
    line(x_coord[7].right+20, y_coord[7].top, x_coord[7].right+40, (y_coord[7].bottom+y_coord[7].top)/2);
    line(x_coord[7].right+20, y_coord[7].bottom, x_coord[7].right+40, (y_coord[7].bottom+y_coord[7].top)/2);

    setcolor(TABELE); ///Seteaza culoarea de desen a butoanelor

    int x=x_coord[7].right+60, y=15+(textheight("INSTRUCTIUNI")+10)*3;
    for(int i=9; i<11; i++) ///Afla coordonatele butoanelor "INTUNECATA" si "LUMINOASA"
    {
        if(i==9)
        {
            outtextxy(x, y, "INTUNECATA");
        }
        if(i==10)
        {
            outtextxy(x, y, "LUMINOASA");
        }
        x_coord[i].left=x-5;
        x_coord[i].right=x+textwidth("INTUNECATA")+5;
        y_coord[i].top=y-5;
        y_coord[i].bottom=y+textheight("INTUNECATA")+5;

        rectangle(x-5, y-5, x+textwidth("INTUNECATA")+5, y+textheight("INTUNECATA")+5);
        y=y+textheight("INTUNECATA")+10;
    }
}

void deseneaza_tabel_paleta(coordonata_x x_coord[23], coordonata_y y_coord[23], int tabel_x, int tabel_y)
{
    setcolor(TABELE); ///Seteaza culoarea de desen a butoanelor
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    line(tabel_x, tabel_y, 400+tabel_x, tabel_y); ///Traseaza linia de sus
    line(tabel_x, tabel_y, tabel_x, 550+tabel_y); ///Traseaza linia din stanga
    line(tabel_x, 550+tabel_y, 400+tabel_x, 550+tabel_y); ///Traseaza linia de jos
    line(400+tabel_x, tabel_y, 400+tabel_x, 550+tabel_y); ///Traseaza linia din dreapta

    int x=x_coord[8].right+60, y=20+textheight("INSTRUCTIUNI");
    for(int i=11; i<22; i++) ///Afla coordonatele butoanelor fiecarei palete
    {
        if(i==11)
        {
            outtextxy(x, y, "STANDARD");
        }
        if(i==12)
        {
            outtextxy(x, y, "PASTEL");
        }
        if(i==13)
        {
            outtextxy(x, y, "IARNA");
        }
        if(i==14)
        {
            outtextxy(x, y, "PRIMAVARA");
        }
        if(i==15)
        {
            outtextxy(x, y, "VARA");
        }
        if(i==16)
        {
            outtextxy(x, y, "TOAMNA");
        }
        if(i==17)
        {
            outtextxy(x, y, "CRACIUN");
        }
        if(i==18)
        {
            outtextxy(x, y, "HALLOWEEN");
        }
        if(i==19)
        {
            outtextxy(x, y, "CAFEA");
        }
        if(i==20)
        {
            outtextxy(x, y, "GALAXIE");
        }
        if(i==21)
        {
            outtextxy(x, y, "AURORA BOREALA");
        }
        x_coord[i].left=x-5;
        x_coord[i].right=x+textwidth("AURORA BOREALA")+5;
        y_coord[i].top=y-5;
        y_coord[i].bottom=y+45;

        rectangle(x-5, y-5, x+textwidth("AURORA BOREALA")+5, y+textheight("AURORA BOREALA")+5);
        y=y+50;
    }

    for (int i=50; i<=550; i=i+50)
        ///Fiecare latura a patratelor este de 50
    {
        line(tabel_x, tabel_y+i, 400+tabel_x, tabel_y+i); ///Traseaza liniile orizontale
    }

    for (int i=50; i<=400; i=i+50)
        ///Fiecare latura a patratelor este de 50
    {
        line(tabel_x+i, tabel_y, tabel_x+i, 550+tabel_y); ///Traseaza liniile verticale
    }

    char linie[2]; ///Trebuie sa contina si caracterul NULL
    for(int i=tabel_x+15, c=1; c<9; i=i+50, c++) ///Numeroteaza coloane
    {
        itoa(c, linie, 10); ///Converteste numarul c intr-un sir de caractere, linie, in baza 10
        outtextxy(i, tabel_y-30, linie);
    }

    for(int i=0; i<11; i++)
        ///Deseneaza paleta cu numarul i
    {
        for(int j=0; j<8; j++)
        {
            circle(tabel_x+50+j*50-25, tabel_y+50+i*50-25, 20);
            setfillstyle(SOLID_FILL, paleta[i][j]);
            fillellipse(tabel_x+50+j*50-25, tabel_y+50+i*50-25, 20, 20);
        }
    }
}

void deseneaza_meniu_paleta(coordonata_x x_coord[23], coordonata_y y_coord[23]) ///Functia se ocupa cu desenul meniului pentru selectia paletei
{
    cleardevice();
    deseneaza_pagina_principala(x_coord, y_coord);
    deseneaza_meniu_setari(x_coord, y_coord);

    setcolor(SCRIS); ///Seteaza culoarea de desen a butoanelor
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 3);

    line(x_coord[8].right+20, y_coord[8].bottom, x_coord[8].right+20, y_coord[8].top); ///Deseneaza sageata catre selectia paletei
    line(x_coord[8].right+20, y_coord[8].top, x_coord[8].right+40, (y_coord[8].bottom+y_coord[8].top)/2);
    line(x_coord[8].right+20, y_coord[8].bottom, x_coord[8].right+40, (y_coord[8].bottom+y_coord[8].top)/2);

    int x=x_coord[8].right+60+textwidth("AURORA BOREALA")+5, y=15+textheight("AURORA BOREALA");
    deseneaza_tabel_paleta(x_coord, y_coord, x, y);
}

void creeaza_pagina_principala(int FEREASTRA_PRINCIPALA) ///Creeaza cea mai importanta pagina
{
    setactivepage(PAGINA_PRINCIPALA);
    setvisualpage(PAGINA_PRINCIPALA);

    int click_start=0, click_setari=0, click_tema=0, click_paleta=0, x_mouse, y_mouse;
    coordonata_x x_coord[23];
    coordonata_y y_coord[23];
    deseneaza_pagina_principala(x_coord, y_coord);

    while(true)
    {
        getmouseclick(WM_LBUTTONDOWN, x_mouse, y_mouse);
        if(getcurrentwindow()==FEREASTRA_PRINCIPALA)
        {
            if(x_mouse>=x_coord[0].left && x_mouse<=x_coord[0].right && y_mouse<=y_coord[0].bottom && y_mouse>=y_coord[0].top)
                ///Butonul "INSTRUCTIUNI" a fost apasat
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati deschis pagina cu instructiuni. Daca apar neclaritati, ne puteti intreba! :) \n");
                instructiuni();
                cleardevice();
                creeaza_pagina_principala(FEREASTRA_PRINCIPALA);
            }

            if(click_start==0 && (x_mouse>=x_coord[1].left && x_mouse<=x_coord[1].right && y_mouse<=y_coord[1].bottom && y_mouse>=y_coord[1].top))
                ///Butonul "START" a fost apasat pentru prima data de cand s-a deschis/revenit la pagina principala
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                click_start=1;
                click_setari=0, click_tema=0, click_paleta=0;
                deseneaza_meniu_start(x_coord, y_coord);
            }

            if(click_start!=0 && (x_mouse>=x_coord[4].left && x_mouse<=x_coord[4].right && y_mouse<=y_coord[4].bottom && y_mouse>=y_coord[4].top))
                ///Butonul "NIVELE" a fost apasat, posibil doar dupa ce s-a apasat pe "START"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat modul de joc \"NIVELE\". Mult succes! :D \n");
                int PAGINA_START_NIVELE=initwindow(LATIME, INALTIME, "Modul de joc: \"NIVELE\"", 5, 175);
                setcurrentwindow(PAGINA_START_NIVELE);
                nivele(FEREASTRA_PRINCIPALA);
            }

            if(click_start!=0 && (x_mouse>=x_coord[5].left && x_mouse<=x_coord[5].right && y_mouse<=y_coord[5].bottom && y_mouse>=y_coord[5].top))
                ///Butonul "RANDOM" a fost apasat, posibil doar dupa ce s-a apasat pe "START"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat modul de joc \"RANDOM\". Mult succes! :D \n");
                int PAGINA_START_RANDOM=initwindow(LATIME, INALTIME, "Modul de joc: \"RANDOM\"", 5, 175);
                setcurrentwindow(PAGINA_START_RANDOM);
                random(FEREASTRA_PRINCIPALA);
            }

            if(click_start!=0 && (x_mouse>=x_coord[6].left && x_mouse<=x_coord[6].right && y_mouse<=y_coord[6].bottom && y_mouse>=y_coord[6].top))
                ///Butonul "CUSTOM" a fost apasat, posibil doar dupa ce s-a apasat pe "START"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat modul de joc \"CUSTOM\". Mult succes! :D \n");
                int PAGINA_START_CUSTOM=initwindow(LATIME, INALTIME, "Modul de joc: \"CUSTOM\"", 5, 175);
                setcurrentwindow(PAGINA_START_CUSTOM);
                custom(FEREASTRA_PRINCIPALA);
            }

            if(click_setari==0 && (x_mouse>=x_coord[3].left && x_mouse<=x_coord[3].right && y_mouse<=y_coord[3].bottom && y_mouse>=y_coord[3].top))
                ///Butonul "SETARI" a fost apasat pentru prima data de cand s-a deschis/revenit la pagina principala
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                click_setari=1;
                click_start=0;
                deseneaza_meniu_setari(x_coord, y_coord);
            }

            if(click_setari!=0 && click_tema==0 && (x_mouse>=x_coord[7].left && x_mouse<=x_coord[7].right && y_mouse<=y_coord[7].bottom && y_mouse>=y_coord[7].top))
                ///Butonul "TEMA" a fost apasat, posibil doar dupa ce s-a apasat pe "SETARI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                click_tema=1;
                click_paleta=0;
                deseneaza_meniu_tema(x_coord, y_coord);
            }

            if(click_tema!=0 && (x_mouse>=x_coord[9].left && x_mouse<=x_coord[9].right && y_mouse<=y_coord[9].bottom && y_mouse>=y_coord[9].top))
                ///Butonul "INTUNECATA" a fost apasat, posibil doar dupa ce s-a apasat pe "TEMA"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat tema \"INTUNECATA\". \n");
                FUNDAL=BLACK, SCRIS=WHITE, TABELE=COLOR(255, 225, 0), SELECTARI=COLOR(255, 0, 0);
                setbkcolor(FUNDAL);
                deseneaza_meniu_tema(x_coord, y_coord);
            }

            if(click_tema!=0 && (x_mouse>=x_coord[10].left && x_mouse<=x_coord[10].right && y_mouse<=y_coord[10].bottom && y_mouse>=y_coord[10].top))
                ///Butonul "LUMINOASA" a fost apasat, posibil doar dupa ce s-a apasat pe "TEMA"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat tema \"LUMINOASA\". \n");
                FUNDAL=WHITE, SCRIS=BLACK, TABELE=COLOR(0, 30, 255), SELECTARI=COLOR(0, 255, 255);
                setbkcolor(FUNDAL);
                deseneaza_meniu_tema(x_coord, y_coord);
            }

            if(click_setari!=0 && click_paleta==0 && (x_mouse>=x_coord[8].left && x_mouse<=x_coord[8].right && y_mouse<=y_coord[8].bottom && y_mouse>=y_coord[8].top))
                ///Butonul "PALETA" a fost apasat, posibil doar dupa ce s-a apasat pe "SETARI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                click_paleta=1;
                click_tema=0;
                deseneaza_meniu_paleta(x_coord, y_coord);
            }

            if(click_paleta!=0 && (x_mouse>=x_coord[11].left && x_mouse<=x_coord[11].right && y_mouse<=y_coord[11].bottom && y_mouse>=y_coord[11].top))
                ///Butonul "STANDARD" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"STANDARD\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[0][i];
                }
            }

            if(click_paleta!=0 && (x_mouse>=x_coord[12].left && x_mouse<=x_coord[12].right && y_mouse<=y_coord[12].bottom && y_mouse>=y_coord[12].top))
                ///Butonul "PASTEL" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"PASTEL\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[1][i];
                }
            }

            if(click_paleta!=0 && (x_mouse>=x_coord[13].left && x_mouse<=x_coord[13].right && y_mouse<=y_coord[13].bottom && y_mouse>=y_coord[13].top))
                ///Butonul "IARNA" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"IARNA\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[2][i];
                }
            }

            if(click_paleta!=0 && (x_mouse>=x_coord[14].left && x_mouse<=x_coord[14].right && y_mouse<=y_coord[14].bottom && y_mouse>=y_coord[14].top))
                ///Butonul "PRIMAVARA" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"PRIMAVARA\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[3][i];
                }
            }
            if(click_paleta!=0 && (x_mouse>=x_coord[15].left && x_mouse<=x_coord[15].right && y_mouse<=y_coord[15].bottom && y_mouse>=y_coord[15].top))
                ///Butonul "VARA" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"VARA\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[4][i];
                }
            }
            if(click_paleta!=0 && (x_mouse>=x_coord[16].left && x_mouse<=x_coord[16].right && y_mouse<=y_coord[16].bottom && y_mouse>=y_coord[16].top))
                ///Butonul "TOAMNA" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"TOAMNA\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[5][i];
                }
            }
            if(click_paleta!=0 && (x_mouse>=x_coord[17].left && x_mouse<=x_coord[17].right && y_mouse<=y_coord[17].bottom && y_mouse>=y_coord[17].top))
                ///Butonul "CRACIUN" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"CRACIUN\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[6][i];
                }
            }
            if(click_paleta!=0 && (x_mouse>=x_coord[18].left && x_mouse<=x_coord[18].right && y_mouse<=y_coord[18].bottom && y_mouse>=y_coord[18].top))
                ///Butonul "HALLOWEEN" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"HALLOWEEN\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[7][i];
                }
            }
            if(click_paleta!=0 && (x_mouse>=x_coord[19].left && x_mouse<=x_coord[19].right && y_mouse<=y_coord[19].bottom && y_mouse>=y_coord[19].top))
                ///Butonul "CAFEA" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"CAFEA\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[8][i];
                }
            }
            if(click_paleta!=0 && (x_mouse>=x_coord[20].left && x_mouse<=x_coord[20].right && y_mouse<=y_coord[20].bottom && y_mouse>=y_coord[20].top))
                ///Butonul "GALAXIE" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"GALAXIE\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[9][i];
                }
            }
            if(click_paleta!=0 && (x_mouse>=x_coord[21].left && x_mouse<=x_coord[21].right && y_mouse<=y_coord[21].bottom && y_mouse>=y_coord[21].top))
                ///Butonul "AURORA BOREALA" a fost apasat, posibil doar dupa ce s-a apasat pe "PALETA DE CULORI"
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Ati selectat paleta \"AURORA BOREALA\". \n");
                for(int i=0; i<8; i++)
                {
                    culoare[i]=paleta[10][i];
                }
            }

            if(x_mouse>=x_coord[2].left && x_mouse<=x_coord[2].right && y_mouse<=y_coord[2].bottom && y_mouse>=y_coord[2].top)
                ///Butonul "EXIT" a fost apasat => inchide programul
            {
                PlaySound("Click.wav", NULL, SND_ASYNC);
                printf("Jocul a fost inchis. Speram ca veti mai juca! :) \n");
                exit(0); ///Inchide fereastra fara erori
            }
        }
    }
}

int main()
{
    int FEREASTRA_PRINCIPALA=initwindow(LATIME, INALTIME, TITLU, 0, 0);
    creeaza_pagina_principala(FEREASTRA_PRINCIPALA);

    getch();
    closegraph();
    return 0;
}
