/****************************************************************************
 * Copyright (C) 2025 Universite de Rennes. All rights reserved.
 *
 * This software was developed at:
 * Universite de Rennes
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 *
 * This file uses the ViSP library.
*****************************************************************************/

/*!
 tp3.cpp : Histogrammes
 
 */


/****************************************************************************
 * NOMS - PRENOMS : 
 *  - VAILLAND Damien
 *  - PLESSIS Axel
 * 
 * Date : 24/09/2024
 *****************************************************************************/


#include <iostream>

#include <visp/vpConfig.h>
#include <visp/vpDebug.h>

#include <visp/vpImage.h>
#include <visp/vpImageIo.h>
#include <visp/vpDisplayX.h>

using namespace std;


////////////////////// FONCTIONS AFFICHAGE ////////////////////////////////////////////

/*
 Affiche un histogramme dans une fenêtre graphique à la position (posX, posY) sur l'écran.
 Parametres d'entree :
 - max est la valeur maximum (en ordonnée) de l'histogramme
 - size est la dimension de l'histogramme
 - posX, posY sont les paramètres de position de la fenêtre d'affichage sur l'écran
 */
void tracer_histo(const float *histo, const float max, const int size, const int posX, const int posY) {
	
	vpImage<unsigned char> Ih(100,size) ;
	Ih = 0;
	
	vpDisplayX d2(Ih,posX,posY) ;
	vpDisplay::display(Ih) ;
	vpDisplay::flush(Ih) ;
	
	int i;
	for(i=0;i<size;i++){
		vpDisplay::displayLine(Ih,99,i,99-(int)(histo[i]*99/max),i,vpColor::red);
	}
	vpDisplay::flush(Ih) ;
	vpDisplay::getClick(Ih) ;
	vpDisplay::close(Ih) ;	
}

void tracer_histocumul(const float *histocumul, const int size, const int posX, const int posY){
	vpImage<vpRGBa> Ihc(100,256) ;

	for(int i=0; i<100; i++){
		for(int j=0; j<256;j++){
			Ihc[i][j].R = 0;
			Ihc[i][j].G = 0;
			Ihc[i][j].B = 0;
		}
	}

	int y;

	for(int j = 0; j < size; j++) {
        y = 99 - (int)(histocumul[j] * 99);

		Ihc[y][j].R = 0;
		Ihc[y][j].G = 0;
		Ihc[y][j].B = 255;
    }


  	vpDisplayX d4(Ihc,384,384) ;
	vpDisplay::setTitle(Ihc, "Histogramme cumule");
	vpDisplay::display(Ihc);
	vpDisplay::flush(Ihc) ;	
	vpDisplay::getClick(Ihc);
}



/**
 * @brief affiche une image à l'écran à la position (posX,posY) et attend un clic
 * @param img : l'image à afficher
 * @param posX, posY : coordonnées spatiales pour positionner affichage de la fenetre sur l'écran
 * @warning : fonction bloquante
 */
void afficheImage(vpImage<unsigned char> img, int posX, int posY, const char *title)
{
    vpDisplayX d(img, posX, posY, title);
    vpDisplay::display(img);
    vpDisplay::flush(img);
    vpDisplay::getClick(img);
    vpDisplay::close(img);
}



/////////////////////////////////////////////////////////////////////////////////////

/* max est la valeur maximum de l'histogramme (le mode principal), 
utilisee pour l'affichage de l'histogramme
*/
void histogramme(const vpImage<unsigned char>  &I, float* histo, float &max)
{
	for (int i = 0; i < 256; i++) {
        histo[i] = 0;
    }
	
    for (int i = 0; i < I.getHeight(); i++) {
        for (int j = 0; j < I.getWidth(); j++) {
            histo[I[i][j]]++;
        }
    }

    max = histo[0];
    for (int key = 1; key < 256; key++) {
        if (histo[key] > max) {
            max = histo[key];
        }
    }

	// for(int key = 0; key < 256 ; key++){
	// 	histo[key] = histo[key] / (I.getHeight() * I.getWidth());
	// }
}


void histocumule(float* histo, float* histocumul)
{
	for (int i = 0; i < 256; i++) {
        histocumul[i] = 0;
    }

    for(int key=0; key<256; key++){
		if(key == 0){
			histocumul[key]=histo[key];
		} else {
			histocumul[key]=histo[key]+histocumul[key-1];
		}
	}

	for(int key=0; key<256; key++){
		histocumul[key]=histocumul[key]/histocumul[255];
	}
}



/*calcul de la moyenne, l'ecart-type et la dynamique d'une image à partir de son histogramme*/
void stat(float *histo, int h, int w, double & moyenne, double & ecart_type, unsigned char &min, unsigned char &max){

	moyenne = 0;
	for(int key=0; key<256; key++){
		moyenne+=histo[key] * key;
	}
	moyenne /= (h*w) ;

	double variance = 0;
    for (int key = 0; key < 256; key++) {
        variance += histo[key] * (key - moyenne) * (key - moyenne);
    }

	variance /= (h*w);

	cout << "variance " << variance << endl ;
	ecart_type = sqrt(variance);

	min = 0;
	while(histo[min]==0){
		min++;
	}

	max = 255;
	while(histo[max]==0){
		max--;
	}
}



/* calcul du nombre de niveaux de gris occupés dans une image*/
int nbNDGOccupe (const float *histo){
	int niv_gris = 0 ;
	for (int key = 0; key < 256; key++) {
		histo[key] == 0 ? niv_gris : niv_gris++ ;
	}

    return niv_gris;
}



/* Calcule et affiche les stats d'une image,
 ainsi que l'histogramme dans une fenêtre graphique à la position (posX, posY) sur l'écran.*/
void display_stat(const vpImage<unsigned char>  &I0, const int posX, const int posY)
{
	int size = 256;

	float histo[size];
	float max;
	
	histogramme(I0,histo,max);
	tracer_histo(histo, max, size, 500, 500);


	float histocumul[size];
	histocumule(histo,histocumul);
	tracer_histocumul(histocumul,size,500,500);
	
	double moyenne;
	double ecart_type;
	unsigned char min_stat;
	unsigned char max_stat;

	stat(histo,I0.getWidth(),I0.getHeight(),moyenne,ecart_type,min_stat,max_stat);

	cout << "Moyenne : " << moyenne << endl ;
	cout << "Ecart type : " << ecart_type << endl ;

	cout << "Dynamique : [" << (int) min_stat << "," << (int) max_stat << "]" << endl ;

	cout << "Nombre de niveaux de gris distincts : " << nbNDGOccupe(histo) << endl ;

}

void anamorphose1(const vpImage<unsigned char>  &I0, const float pente)
{
	vpImage<unsigned char> I1;
	I1.resize(I0.getHeight(), I0.getWidth());

	int valeur;
	for (int i = 0; i < I0.getHeight(); i++) {
        for (int j = 0; j < I0.getWidth(); j++) {
            valeur = (int)(I0[i][j] * pente); 
			I1[i][j] = (unsigned char) valeur;
        }
    }

	vpDisplayX d5(I1,384,384) ;
	vpDisplay::setTitle(I1, "Pente");
	vpDisplay::display(I1);
	vpDisplay::flush(I1) ;	
	vpDisplay::getClick(I1);
}


void anamorphose2(const vpImage<unsigned char>  &I0, const unsigned int seuil)
{
	vpImage<unsigned char> I2;
	I2.resize(I0.getHeight(), I0.getWidth());
	int valeur;

	for (int i = 0; i < I0.getHeight(); i++) {
        for (int j = 0; j < I0.getWidth(); j++) {
			valeur = (int)(I0[i][j]*(255/seuil));
            I0[i][j] < seuil ? I2[i][j] = (unsigned char) valeur : I2[i][j] = 255 ;
        }
    }

	vpDisplayX d6(I2,384,384) ;
	vpDisplay::setTitle(I2, "Seuil");
	vpDisplay::display(I2);
	vpDisplay::flush(I2) ;	
	vpDisplay::getClick(I2);
}


void egalisation(const vpImage<unsigned char>  &I)
{
	vpImage<unsigned char> I3;
	I3.resize(I.getHeight(), I.getWidth());

	int size = 256;
	float histo[size];
	float histocumul[size];
	int g[size];
	float max;
	
	histogramme(I,histo,max);
	histocumule(histo,histocumul);
	
	for(int key=0; key<256; key++){
		g[key] = round(255 * histocumul[key]);
	}

	for(int i=0; i<I.getHeight();i++){
		for(int j=0; j<I.getWidth();j++){
			I3[i][j] = g[I[i][j]];
		}
	}

	display_stat(I3,356,300);

	vpDisplayX d6(I3,356,100) ;
	vpDisplay::setTitle(I3, "Egalisation");
	vpDisplay::display(I3);
	vpDisplay::flush(I3) ;	
	vpDisplay::getClick(I3);
}


/* calcul l'EQM entre l'image originale I et l'image après quantification Iq*/
double EQM (const vpImage<unsigned char>  &I, const vpImage<unsigned char>  &Iq){
    double EQM ;

	for (int i = 0; i < I.getHeight(); i++) {
        for (int j = 0; j < I.getWidth(); j++) {
			EQM += pow(Iq[i][j] - I[i][j],2) ;
        }
    }
	
	EQM /= (I.getWidth()*I.getHeight());

    return EQM;
}


void quantification_uniforme(const vpImage<unsigned char>  &I, int nbbits)
{
	vpImage<unsigned char> I4;
	I4.resize(I.getHeight(), I.getWidth());

	int niv_gris = pow(2,nbbits);

	// int distrib[256];
	int tab_niv_gris[niv_gris];
	int ecart_tranche = 256 / niv_gris ;

	for(int x = 0 ; x < niv_gris ; x++){
		// tab_niv_gris[x] = ecart_tranche * x;
		tab_niv_gris[x] = ecart_tranche * x + ecart_tranche / 2;
	}

	for(int i=0 ; i<I.getHeight() ; i++){
		for(int j=0 ; j<I.getWidth() ; j++){
			I4[i][j] = tab_niv_gris[I[i][j] / ecart_tranche];
		}
	}

	display_stat(I4,356,300);
	cout << "EQM" << EQM(I,I4) << endl ;


	vpDisplayX d7(I4,356,100) ;
	vpDisplay::setTitle(I4, "Quantification");
	vpDisplay::display(I4);
	vpDisplay::flush(I4) ;	
	vpDisplay::getClick(I4);
}




int main(int argc, char **argv)
{

  cout << "BINP TP3 : HISTOGRAMMES ET QUANTIFICATION" << endl ;
  cout << "--" << endl ;

	vpImage<unsigned char>  I0;
	string sIm;	
	vpDisplayX d0(100,100,"Image originale") ;
	float pente;
	unsigned int seuil;
	
	int choix=0;
	while(choix<4)
	{
		cout << "\n-------------------------------------" << endl;
		cout<<"1. Affichage des stats d'une image"<<endl;
		cout<<"2. Anamorphoses"<<endl;
		cout<<"3. Egalisation d'histogramme"<<endl;
 		cout<<"4. Quantification uniforme"<<endl;
		cout<<"5. Quitter"<<endl;
		
		cin>>choix;
		cout << "-------------------------------------" << endl;
		
		
		switch(choix)
		{
			case 1 :
				cout << "Nom de l'image : "; cin >> sIm; // Ex: ../images/nom_image	
				sIm = "../images/" + sIm; // Modif : Ajout du ../images/ automatiquement
				vpImageIo::read(I0,sIm) ;
                /* pour debugger :
                 vpImageIo::read(I0,"../data/nom_image") ;
                */
				d0.init(I0);
				vpDisplay::display(I0);
				vpDisplay::flush(I0) ;
				cout << "\n Valeurs image initiale : " << endl;

				display_stat(I0, 100, 500);
				
				vpDisplay::getClick(I0);
				vpDisplay::close(I0) ;
				break;
			case 2 :
				cout << "Nom de l'image : "; cin >> sIm; 	
				sIm = "../images/" + sIm;
				vpImageIo::read(I0,sIm) ;
				d0.init(I0);
				vpDisplay::display(I0);
				vpDisplay::flush(I0) ;	
				
				// Premiere anamorphose
				cout << "pente ? "; cin >> pente; 	
				anamorphose1(I0,pente);
				
				// Deuxieme anamorphose
				cout << "\nseuil ? "; cin >> seuil; 
				anamorphose2(I0, seuil);	
				
				vpDisplay::getClick(I0);
				vpDisplay::close(I0) ;
				break;
			case 3 :
                cout << "Nom de l'image : "; cin >> sIm;	
				sIm = "../images/" + sIm;
                vpImageIo::read(I0,sIm) ;
				d0.init(I0);
				vpDisplay::display(I0);
				vpDisplay::flush(I0) ;	
				
				// Egalisation
				egalisation(I0);
				
				vpDisplay::getClick(I0);
				vpDisplay::close(I0) ;
				break;
                
            case 4 :
                cout << "Nom de l'image : "; cin >> sIm;	
				sIm = "../images/" + sIm;
                vpImageIo::read(I0,sIm) ;
                d0.init(I0);
                vpDisplay::display(I0);
                vpDisplay::flush(I0) ;
                
                int nbbits;
                cout <<"entrer le nombre bits par composante :"<<endl;
                cin >> nbbits;
                
                // Quantification uniforme
                quantification_uniforme(I0, nbbits);
                
                vpDisplay::getClick(I0);
                vpDisplay::close(I0) ;
                break;
                
				
			default :
				break;
		}
	}
	

	
  cout << "Fin du programme " << endl ;
  return(0);
}















