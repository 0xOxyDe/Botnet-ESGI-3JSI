![Logo](https://i.imgur.com/1IestUQ.png)


# Botnet C&C

BotnetC2, un projet pédagogique développé par des élèves de 3e année en cybersécurité à l'ESGI.
Ce projet est conçu pour infecter une machine et en prendre le contrôle via le panneau d'administration.


## Compilation

Compilation sous windows obligatoire.

```bash
gcc .\malware.c -o malware -lwsock32 -lws2_32
```

```bash
gcc .\installer.c -o installer
```
    
## Auteurs

- [@Nicolas Torres-Sannier](https://github.com/Nicolas-Torres-Sannier)
- [@OxyDe](https://github.com/0xOxyDe)
- [@Skriix](https://github.com/Skriix)


## Features

- DDOS TCP / UDP ✅
- Collect d'informations software/hardware ✅
- BombeFork ✅
- Persistance ✅
- Pop-up Audio/Visuel ✅
- Enumération des fichiers ✅
- Suppression de fichiers ✅
- Envoie de commande systèmes ✅
- Suppression des traces ✅
- Altération des fichiers ✅
- Crypto-miner 🔜
- Extraction des mot de passes 🔜