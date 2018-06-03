#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <time.h>
#include <pthread.h>





SDL_Surface *notif = NULL, *texte = NULL;
TTF_Font *police = NULL;
SDL_Rect position;
Mix_Music *musique;
Mix_Music *musique2;
SDL_Color couleurNoire = {0, 0, 0};
SDL_Color couleurVerte = {0, 255, 0};
SDL_Color couleurRouge = {255, 0, 0};
FILE* fichier = NULL;
FILE* fichier2 = NULL;
time_t tm;

void pause() {
    int continuer = 1;
    SDL_Event event;
 
    while (continuer) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}


double recup_time() {
	time(&tm);
	return (double) tm; 
}
 
void affichage(TTF_Font *police, SDL_Surface *surfacesrc, SDL_Surface *surfacedest, char *texte, SDL_Rect position, SDL_Color couleur,int x, int y){
	surfacesrc = TTF_RenderText_Solid(police, texte, couleur);
	position.x = x;
	position.y = y;
	SDL_BlitSurface(surfacesrc, NULL, surfacedest, &position);
}

void evenement(TTF_Font *police, SDL_Surface *surfacesrc, SDL_Surface *surfacedest, SDL_Rect position, SDL_Color couleur) {
	int continuer = 1;
	SDL_Event event;

	while (continuer) {
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_MOUSEBUTTONDOWN :
				if (event.button.x >= 100 && event.button.x <= 134 && event.button.y >=59 && event.button.y <=81 ) {
				
					SDL_FillRect(surfacedest, NULL, SDL_MapRGB(surfacedest->format, 200, 173, 127));
					affichage(police,surfacesrc,surfacedest,"BackUP en cours ...",position,couleur,55,35);
					SDL_Flip(notif);
					sleep(1);
					if(system("/home/mehdi/Bureau/BackUP/compress.bash")!=0){
						SDL_FillRect(surfacedest, NULL, SDL_MapRGB(surfacedest->format, 200, 173, 127));
						affichage(police,surfacesrc,surfacedest,"Erreur lors du BackUP !",position,couleurRouge,55,35);
						SDL_Flip(notif);
						sleep(3);
						exit(0);
					}
					sleep(1);
					
					SDL_FillRect(surfacedest, NULL, SDL_MapRGB(surfacedest->format, 200, 173, 127));
					affichage(police,surfacesrc,surfacedest,"Envois sur Dropbox",position,couleur,55,35);
					SDL_Flip(notif);
					sleep(1);
					if(system("/home/mehdi/Bureau/BackUP/move_dropbox.bash")!=0){
						SDL_FillRect(surfacedest, NULL, SDL_MapRGB(surfacedest->format, 200, 173, 127));
						affichage(police,surfacesrc,surfacedest,"Erreur lors de l envois !",position,couleurRouge,55,35);
						SDL_Flip(notif);
						sleep(3);
						exit(0);
					}
					sleep(1);
					// Demande de brancher la clé USB
					SDL_FillRect(notif, NULL, SDL_MapRGB(notif->format, 200, 173, 127));
					affichage(police,texte,notif,"En attente de cle USB",position,couleurNoire,100,25);
					SDL_Flip(notif);
					
					// Tant que cle usb pas branché
					while (!(fopen("/media/mehdi/CLEUSB","r"))) {
						sleep(10);
					}

					SDL_FillRect(notif, NULL, SDL_MapRGB(notif->format, 200, 173, 127));
					affichage(police,texte,notif,"Cle USB detecte",position,couleurVerte,100,25);
					SDL_Flip(notif);
					sleep(4);
	
					
					SDL_FillRect(surfacedest, NULL, SDL_MapRGB(surfacedest->format, 200, 173, 127));
					affichage(police,surfacesrc,surfacedest,"Deplacement sur la cle USB ...",position,couleur,0,35);
					SDL_Flip(notif);
					if(system("/home/mehdi/Bureau/BackUP/move.bash")!=0){
						SDL_FillRect(surfacedest, NULL, SDL_MapRGB(surfacedest->format, 200, 173, 127));
						affichage(police,surfacesrc,surfacedest,"Erreur lors du Transfert de Donnees !",position,couleurRouge,1,35);
						SDL_Flip(notif);
						sleep(3);
						exit(0);
					}
					sleep(3);
					SDL_FillRect(surfacedest, NULL, SDL_MapRGB(surfacedest->format, 200, 173, 127));
					affichage(police,surfacesrc,surfacedest,"Deplacement Effectuee",position,couleur,0,35);
					SDL_Flip(notif);
					continuer = 0;
					
				}
				else if (event.button.x >= 200 && event.button.x <= 245 && event.button.y >=59 && event.button.y <=81 ) {
					SDL_Quit();
					
					printf("NON\n");
					
					// Recuperation du temps	
					double actual_time;
					double past_time;
					char chaine[100];
		
					fichier2 =fopen("24heure.txt","r");
					//printf("ouvre en r\n");
					if ( fichier2 == NULL ) {
						fichier2 = fopen("24heure.txt","w+");
						//printf("ouvre en w+\n");
						actual_time=recup_time();
						//printf("%f\n",actual_time);
						snprintf(chaine,100,"%f",actual_time);
						fputs(chaine,fichier2);
		
					}
					else {
						// On remet le fichier 24h a jour		
						fclose(fichier2);
						fichier2=fopen("24heure.txt","w");
						actual_time=recup_time();
						snprintf(chaine,100,"%f",actual_time);
						fputs(chaine,fichier2);
					}
					fclose(fichier2);
					fichier2=fopen("24heure.txt","r+");
					
					fgets(chaine,100,fichier2);
					sscanf(chaine,"%lf", &past_time);
					sleep(3);
					
					actual_time = recup_time();
					while ( actual_time < past_time + 2*60*60 ) { // 2 heures
						sleep(7200);
						actual_time = recup_time();
						
					}
						
						
					//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ); // Initialisation de la SDL
					//SDL_putenv("SDL_VIDEO_WINDOW_POS=1090,0");
    				notif = SDL_SetVideoMode(400, 85, 32, SDL_HWSURFACE | SDL_NOFRAME); 
					SDL_FillRect(notif, NULL, SDL_MapRGB(notif->format, 200, 173, 127));
					
					printf("Rappel lancer\n");
					affichage(police,texte,notif,"Rappel, un BackUP doit etre effectue",position,couleurNoire,50,15);
					SDL_Flip(notif);
					
					sleep(3);
					
					
					SDL_FillRect(notif, NULL, SDL_MapRGB(notif->format, 200, 173, 127));
					affichage(police,texte,notif,"Voulez-vous sauvegarder ?",position,couleurNoire,50,1);
					affichage(police,texte,notif,"OUI",position,couleurNoire,100,60);
					affichage(police,texte,notif,"NON",position,couleurNoire,200,60);
					SDL_Flip(notif);
					
					fclose(fichier2);
					evenement(police,texte,notif,position,couleurNoire);
				}		
				break;
    	}
	}
}
	
int main(int argc, char *argv[]) {
	
	// Initialisation des libraries
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ); // Initialisation de la SDL
	TTF_Init(); // Initialisation de la biblio SDL_TTF
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
 	musique = Mix_LoadMUS("/home/mehdi/Documents/SDL/TwitterSonNotif.mp3"); 
 	//musique2 = Mix_LoadMUS("/home/mehdi/Documents/SDL/tinderNotif.mp3"); 
	police = TTF_OpenFont("/home/mehdi/Documents/SDL/angelina.TTF", 27);
		 
	// Recuperation du temps	
	double actual_time;
	double past_time;
	char chaine[100];
	
	// Test de l ouverture du fichier et sauvegarde de l'heure dedans
	fichier =fopen("3mois.txt","r");
	if ( fichier == NULL ) {
		fichier=fopen("3mois.txt","w+");
		actual_time=recup_time();
		
		snprintf(chaine,100,"%f",actual_time);
		fputs(chaine,fichier);
		
	}
	else {
		fclose(fichier);
		fichier=fopen("3mois.txt","r+");
	}
	
	fgets(chaine,100,fichier);
	sscanf(chaine,"%lf", &past_time);

	while(1) {
		actual_time = recup_time();
		if ( actual_time >= past_time + 25*24*60*60) {//7516800) { 
			
			// Ouverture de la fenêtre
			SDL_putenv("SDL_VIDEO_WINDOW_POS=1090,0");
    		notif = SDL_SetVideoMode(400, 85, 32, SDL_HWSURFACE | SDL_NOFRAME); 
			SDL_FillRect(notif, NULL, SDL_MapRGB(notif->format, 200, 173, 127));
			
			// Si l'ouverture a échouée			
			if (notif == NULL) { 
  				fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        		exit(EXIT_FAILURE);
    		}
		
			//Lancement de l'affichage de la notification
			affichage(police,texte,notif,"Nouvelle Notification",position,couleurVerte,100,0);
			affichage(police,texte,notif,"Rappel",position,couleurRouge,0,30);
			affichage(police,texte,notif, " : Une Sauvegarde des donnees",position,couleurNoire,70,30);
			affichage(police,texte,notif,"doit etre effectuee",position,couleurNoire,0,60);
			SDL_Flip(notif);
			
			
			//musique = Mix_LoadMUS("/home/mehdi/Documents/SDL/TwitterSonNotif.mp3");
			//Mix_PlayMusic(musique,1);
			Mix_FadeInMusic(musique, 1, 1000);
			

			sleep(4);
			Mix_RewindMusic();
			
				
			//SDL_putenv("SDL_VIDEO_WINDOW_POS=1090,0");
    		//notif = SDL_SetVideoMode(400, 85, 32, SDL_HWSURFACE | SDL_NOFRAME);
			SDL_FillRect(notif, NULL, SDL_MapRGB(notif->format, 200, 173, 127));
			affichage(police,texte,notif,"Voulez-vous sauvegarder ?",position,couleurNoire,50,1);
			affichage(police,texte,notif,"OUi",position,couleurNoire,100,60);
			affichage(police,texte,notif,"NON",position,couleurNoire,200,60);
			SDL_Flip(notif);
			evenement(police,texte,notif,position,couleurNoire);
			
			// On met le fichier a jour
			fclose(fichier);
			fichier=fopen("3mois.txt","w");
			actual_time=recup_time();
			snprintf(chaine,100,"%f",actual_time);
			fputs(chaine,fichier);
			
			// on met a jour past_time
			fgets(chaine,100,fichier);
			sscanf(chaine,"%lf", &past_time);
		}
		else {
			sleep(4*24*60*61); // 4 jours
		}
	}
    pause(); // Mise en pause du programme
	
	//Fermeture des variables et libraries
	Mix_FreeMusic(musique);
	Mix_FreeMusic(musique2);
	TTF_CloseFont(police);
	TTF_Quit(); 
   	SDL_Quit(); // Arrêt de la SDL
	Mix_CloseAudio();
	fclose(fichier);
	fclose(fichier2);
 
	//Fermeture du programme
    return EXIT_SUCCESS;
}
 
