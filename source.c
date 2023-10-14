#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#pragma warning(disable : 4996)

enum {
	NB_SEMESTRES = 2,
	MIN_UE = 3,
	MAX_UE = 6,
	MAX_MATIERES = 10,
	MAX_EPREUVES = 5,
	MAX_ETUDIANTS = 100,
	MAX_CHAR = 30
};

typedef struct {
	char commande[31];
	int semestre;
} Commande;

typedef struct {
	char commande[31];
	double coef[MAX_UE];
	double note[MAX_ETUDIANTS];
} Epreuve;

typedef struct {
	char commande[31];
	unsigned int nbEpreuves;
	Epreuve epreuve[MAX_MATIERES];
} Matiere;

typedef struct {
	unsigned int nbMatieres;
	Matiere matieres[MAX_MATIERES];
} Semestre;

typedef struct {
	unsigned int nbUE;
	Semestre semestres[NB_SEMESTRES];
} Formation;

typedef struct {
	char commande[31];
} Etudiant;

typedef struct {
	unsigned int nbEtudiant;
	Etudiant etudiant[MAX_ETUDIANTS];
} Etudiants;


// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxDEBUT COMMANDE 2xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


int formation() {
	int nbUE;

	scanf("%d", &nbUE);
	/*on a utilisée un branchment conditionnel avec alternative qui selon la saisit de l'utilisateur va permettre le bon affichage*/
	if (nbUE >= 3 && nbUE <= 6) {
		printf("Le nombre d'UE est defini\n");
		return nbUE;
	}
	else {
		printf("Le nombre d'UE est incorrect\n");
		return 0;
	}
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxFIN COMMANDE 2xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/*cette fonction sera utilisée dans quasiment toutes les commandes prochaines*/
int sem(int s) {
	/*fonction me permettant de vérifier si le numéro de semestre saisit est correcte ou non et renverra le message adequoi grace au branchement conditionnel if */
	if (s == 1) {
		return 0;
	}
	else if (s == 2) {
		return 1;
	}
	else {
		printf("Le numero de semestre est incorrect\n");
		return -1;
	}
}

/*avec ces doubles boucles on a pu parcourir toutes les épreuves*/
int verifE(Semestre* s, char m[31], char e[31]) {
	for (unsigned int i = 0; i < s->nbMatieres; ++i) {
		/*vérifie s'il existe une matiere à la position i*/
		/*strcmp est une fonction provenant de la la bibliothèque string.h qui realise la comparaison entre deux chaines de caractère */
		/* ! correspond au == 0, si on trouve le même nom de matière on valide le if*/
		if (!strcmp(s->matieres[i].commande, m)) {
			for (unsigned int j = 0; j < MAX_EPREUVES; ++j) {
				/*vérifie s'il existe une épreuve j à la positon de la matiere i*/
				/*compare pour savoir s'il existe une épreuve du même nom */
				if (!strcmp(s->matieres[i].epreuve[j].commande, e)) {
					printf("Une meme epreuve existe deja\n");
					return 0;
				}
			}
		}
	}
	return 1;
}
/*Permets de savoir s'il existe une matière et nous renvoie sa position sinon return -1 qui signifie que cette matière n'existe pas et qu'il va falloir l'enregistrer */
int verifm(Semestre* s, char commande[31]) {
	for (unsigned int i = 0; i < MAX_MATIERES; i++) {
		if (!strcmp(s->matieres[i].commande, commande)) {
			return i;
		}
	}
	return -1;
}

int coefs(Epreuve* c, int nbUE) {
	double add = 0;
	for (int i = 0; i < nbUE; ++i) {
		scanf("%lf", &c->coef[i]);
		/*interdiction d'avoir un coef < 0 car on ne peut pas avoir une moyenne négative*/
		if (c->coef[i] < 0) {
			printf("Au moins un des coefficients est incorrect\n");
			return -1;
		}
		add += c->coef[i];
	}
	/*vérification importante de la somme de coef d'un UE sera important par la suite pour le calcul des moyennes, si la somme des coefs est nulle n ne pourra pas calculer
	la moyenne car divisé par 0 est impossible*/
	if (add != 0) {
		return 1;
	}
	else
		printf("Au moins un des coefficients est incorrect\n");
	return 0;
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxDEBUT COMMANDE 3xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void epreuve(Formation* f) {
	char m[31];
	char e[31];
	int seum;
	scanf("%d %s %s", &seum, m, e);
	seum = sem(seum);
	if (seum != -1) {
		if (verifE(&f->semestres[seum], m, e) == 1) {
			/*indique la position de la matière d'une matiére déjà enregistrée*/
			int nbMatieres = verifm(&f->semestres[seum], m);
			/* PARTIT SUR L'ENREGISTREMENT D'UNE NOUVELLE MATIÈRE*/
			/*ici la matière est saisie pour la première fois donc on l'enregistre et la positionne à l'emplacement nbMatiere qu'on va ensuite incrémenter pour les prochaines
			nouvelles matières, elle va aussi enregistrer l'épreuve a la bonne positon et incrémentera nbEpreuve de cette matière*/
			if (nbMatieres == -1) {
				if (coefs(&f->semestres[seum].matieres[f->semestres[seum].nbMatieres].epreuve[f->semestres[seum].matieres[f->semestres[seum].nbMatieres].nbEpreuves], f->nbUE) == 1) {
					/*strcpy fonction provenant de la bibliothèque string.h permettant de copier le contenue d'une chaine charactère dans une autre
					(en soi c'est l'enregistrement dont je parlais) */
					strcpy(f->semestres[seum].matieres[f->semestres[seum].nbMatieres].commande, m);
					printf("Matiere ajoutee a la formation\n");
					++f->semestres[seum].nbMatieres;

					nbMatieres = f->semestres[seum].nbMatieres - 1;
					strcpy(f->semestres[seum].matieres[nbMatieres].epreuve[f->semestres[seum].matieres[nbMatieres].nbEpreuves].commande, e);
					printf("Epreuve ajoutee a la formation\n");
					++f->semestres[seum].matieres[nbMatieres].nbEpreuves;
				}
			}
			/* PARTIT SUR UNE MATIERE DEJA ENREGISTRÉE*/
			/*la matière à déjà était enregistrée au moins une fois donc on positionne directement la matière à la position nbMatiere qui a été return grace à la fonction
			verifm, on incrémentera alors seulement le nbEpreuve de la matière concernée */
			else {
				if (coefs(&f->semestres[seum].matieres[nbMatieres].epreuve[f->semestres[seum].matieres[nbMatieres].nbEpreuves], f->nbUE) == 1) {
					strcpy(f->semestres[seum].matieres[nbMatieres].epreuve[f->semestres[seum].matieres[nbMatieres].nbEpreuves].commande, e);
					printf("Epreuve ajoutee a la formation\n");
					++f->semestres[seum].matieres[nbMatieres].nbEpreuves;
				}
			}
		}
	}
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxFIN COMMANDE 3xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxDEBUT COMMANDE 4xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


// pour cette fonction on utilise un int car on a réalisé toute la commande 4 dans une même fonction et pour qu'elle s'arrête à un message envoyé nous avions besoin de mettre un return
int coefficients(Formation* f) {
	int seum;
	double add = 0;
	scanf("%d", &seum);
	seum = sem(seum);
	if (seum >= 0 && seum <= 1) {
		/*s'il n'y a pas de matières il n'y a pas d'épreuves et s'il n'y a pas d'épreuves il n'y a pas de coefs, donc si nbMatiere = 0 donc calculer la somme des coefs d'un UE est inutile*/
		if (f->semestres[seum].nbMatieres == 0) {
			printf("Le semestre ne contient aucune epreuve\n");
			return -2;
		}
		else {
			for (unsigned int i = 0; i < f->nbUE; ++i) {
				/*initialisation pour qu'a chaque UE on a une nouvelle somme*/
				add = 0;
				for (unsigned int j = 0; j < f->semestres[seum].nbMatieres; ++j) {
					for (unsigned int k = 0; k < f->semestres[seum].matieres[j].nbEpreuves; ++k) {
						add = add + f->semestres[seum].matieres[j].epreuve[k].coef[i];
					}
				}
				/*impossible de diviser par zéro donc avec une somme de coef = 0 est embêtant donc on interdit l'utilisateur de mettre une UE avec que des coefs = 0*/
				if (add == 0) {
					printf("Les coefficients d'au moins une UE de ce semestre sont tous nuls\n");
					return -1;
				}
			}
			printf("Coefficients corrects\n");
			return 0;
		}
	}
	else {
		return -1;
	}
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxFIN COMMANDE 4xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int Mverif(Semestre* s, char commande[31]) {
	for (unsigned int i = 0; i < MAX_MATIERES; i++) {
		if (!strcmp(s->matieres[i].commande, commande)) {
			return i;
		}
	}
	return -1;
}

int Everif(Semestre* s, char m[31], char e[31]) {
	for (unsigned int i = 0; i < s->nbMatieres; ++i) {
		if (!strcmp(s->matieres[i].commande, m)) {
			for (unsigned int j = 0; j < MAX_EPREUVES; ++j) {
				if (!strcmp(s->matieres[i].epreuve[j].commande, e)) {
					return j;
				}
			}
		}
	}
	return -1;
}

int Etuverif(char nom[31], Etudiants* etu) {
	for (unsigned int i = 0; i < MAX_ETUDIANTS; i++) {
		if (!strcmp(etu->etudiant[i].commande, nom)) {
			return i;
		}
	}
	return -1;
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxDEBUT COMMANDE 5xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void note(Formation* f, Etudiants* etu) {
	int seum, position_matiere, position_epreuve, Verif_existance_etudiant;
	char m[31];
	char e[31];
	char nom[31];
	double note;
	scanf("%d %s %s %s %lf", &seum, nom, m, e, &note);
	seum = sem(seum);
	if (seum != -1) {
		/*pour retrouver la position de la matière avec le return i */
		position_matiere = Mverif(&f->semestres[seum], m);
		if (position_matiere != -1) {
			/*pour retrouver la position de l'épreuve avec le return j */
			position_epreuve = Everif(&f->semestres[seum], m, e);
			if (position_epreuve != -1) {
				/*permets de connaitre si l'étudiant existe ou non et s'il existe donne sa position avec le return i*/
				Verif_existance_etudiant = Etuverif(nom, etu);
				/* PARTIT SUR L'ENREGISTREMENT D'UN NOUVEL ETUDIANT*/
				/*ma fonction Etuverif à vérifier l'existence de l'étudiant dans cette partit l'étudiant n'a jamais été enregistrer donc on l'a stock à la position nbEtudiant
				(qu'on a initialisé à 0 dans le main) et qu'on incrémente a chaque ajout pour le prochain étudiant enregistrer, on lui enregistrera aussi sa note*/
				if (Verif_existance_etudiant == -1) {
					/*dans le main nous avons initialisé toutes les notes de chaque épreuve pour chaque étudiant à -1, cet indice nous permet de savoir si l'étudiant a son adresse de
					l'épreuve de la matière a déjà une note d'enregistrer ou non*/
					if (f->semestres[seum].matieres[position_matiere].epreuve[position_epreuve].note[etu->nbEtudiant] == -1) {
						if (note >= 0 && note <= 20) {
							printf("Etudiant ajoute a la formation\n");
							strcpy(etu->etudiant[etu->nbEtudiant].commande, nom);
							printf("Note ajoutee a l'etudiant\n");
							f->semestres[seum].matieres[position_matiere].epreuve[position_epreuve].note[etu->nbEtudiant] = note;
							++etu->nbEtudiant;
						}
						else
							printf("Note incorrecte\n");
					}
					else
						printf("Une note est deja definie pour cet etudiant\n");
				}
				/* PARTIT SUR UN ETUDIANT DEJA ENREGISTRE*/
				/*comme l'étudiant a déjà été enregister on connaît déjà sa position qu'on ira chercher dans la fonction Etuverif pour qu'on lui enregistre la note saisit par l'utilisateur*/
				if (Verif_existance_etudiant != -1) {
					/*dans le main nous avons initialié toutes les notes de chaque épreuve pour chaque étudiant à -1, cet indice nous permet de savoir si l'étudiant a son adresse de
					l'épreuve de la matière a déjà une note d'enregistrer ou non*/
					if (f->semestres[seum].matieres[position_matiere].epreuve[position_epreuve].note[Verif_existance_etudiant] == -1) {
						if (note >= 0 && note <= 20) {
							printf("Note ajoutee a l'etudiant\n");
							f->semestres[seum].matieres[position_matiere].epreuve[position_epreuve].note[Verif_existance_etudiant] = note;
						}
						else
							printf("Note incorrecte\n");
					}
					else
						printf("Une note est deja definie pour cet etudiant\n");
				}
			}
			else
				printf("Epreuve inconnue\n");
		}
		else {
			printf("Matiere inconnue\n");
		}
	}
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxFIN COMMANDE 5xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int Verif_nb_note(Semestre* s) {
	int compteur_note = 0;
	for (unsigned int i = 0; i < s->nbMatieres; ++i) {
		for (unsigned int j = 0; j < s->matieres[i].nbEpreuves; ++j) {
			++compteur_note;
		}
	}
	return compteur_note;
}

int Verif_nb_note_etudiant(Semestre* s, int position) {
	int compteur_note_etu = 0;
	for (unsigned int i = 0; i < s->nbMatieres; ++i) {
		for (unsigned int j = 0; j < s->matieres[i].nbEpreuves; ++j) {
			if (s->matieres[i].epreuve[j].note[position] != -1) {
				++compteur_note_etu;
			}
		}
	}
	return compteur_note_etu;
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxDEBUT COMMANDE 6xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void notes(Formation* f, Etudiants* etu) {
	int seum, nb_notes, nb_notes_etudiant, position_etu;
	char nom[31];
	scanf("%d %s", &seum, nom);
	seum = sem(seum);
	/*la fonction Etuverif me return i soit la position de l'étudiant si l'étudiant est inconnu elle va me return -1
	(fonction crée tout d'abord pour la commande 6 mais qui a pu être réutilisé ici)*/
	position_etu = Etuverif(nom, etu);
	if (seum != -1) {
		if (position_etu != -1) {
			/*dans ces variables je peux stocker dans l'une le nombre de notes enregistrées à l'étudiant et dans l'autre le nombre d'épreuves (toutes matières confondues) pour ainsi
			vérifier si on a bien enregistré toutes les notes à l'étudiant*/
			nb_notes = Verif_nb_note(&f->semestres[seum]);
			nb_notes_etudiant = Verif_nb_note_etudiant(&f->semestres[seum], position_etu);
			if (nb_notes == nb_notes_etudiant) {
				printf("Notes correctes\n");
			}
			else {
				printf("Il manque au moins une note pour cet etudiant\n");
			}
		}
		else
			printf("Etudiant inconnu\n");
	}
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxFIN COMMANDE 6xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int verif_coef(Formation* f, Semestre* s) {
	double somme = 0;
	/*Pour chaque épreuve on vérifie si la somme des coefs d'un UE n'est pas nul, il est important de renitialiser somme ? 0 ainsi de la réutiliser
	pour recalculer la somme des coefs d'un autre UE*/
	for (unsigned int i = 0; i < f->nbUE; ++i) {
		somme = 0;
		for (unsigned int j = 0; j < s->nbMatieres; ++j) {
			for (unsigned int k = 0; k < s->matieres[j].nbEpreuves; ++k) {
				somme = somme + s->matieres[j].epreuve[k].coef[i];
			}
		}
		/*il n'est pas possible de calculer la moyenne d'un UE d'un semestre si la somme des coefs est nulle c'est pour cela qu'on n'accepte pas la somme = 0 comme
		il est impossible de diviser par 0*/
		if (somme == 0) {
			return -1;
		}
	}
	return 0;
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxDEBUT COMMANDE 7xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void releve(Formation* f, Etudiants* etu) {
	int seum, etudiant, notes, notes_etudiant;
	char nom[31];
	int numero = 1;
	double note = 0;
	double somme_coef = 0;
	double moyenne_matiere;
	scanf("%d %s", &seum, nom);
	seum = sem(seum);
	etudiant = Etuverif(nom, etu);
	if (seum != -1) {
		if (etudiant != -1) {
			if (verif_coef(&f->nbUE, &f->semestres[seum]) == 0) {
				/*dans ces variables je peux stocker dans l'une le nombre de note enregistré à l'étudiant et dans l'autre le nombre d'épreuves (toutes matières confondues) pour ainsi
				vérifier si on a bien enregistré toutes les notes de l'étudiant (fonction crée tout d'abord pour la commande 6 mais qui a pu être réutilisé ici) */
				notes = Verif_nb_note(&f->semestres[seum]);
				notes_etudiant = Verif_nb_note_etudiant(&f->semestres[seum], etudiant);
				if (notes == notes_etudiant) {
					printf("                ");
					for (unsigned int i = 1; i <= f->nbUE; ++i) {
						printf("UE%d            ", i);
					}
					printf("\n");
					for (unsigned int j = 0; j < f->semestres[seum].nbMatieres; j++) {
						printf("%s       ", f->semestres[seum].matieres[j].commande);
						for (unsigned int k = 0; k < f->nbUE; ++k) {
							note = 0;
							somme_coef = 0;
							for (unsigned int l = 0; l < f->semestres[seum].matieres[j].nbEpreuves; ++l) {
								note += f->semestres[seum].matieres[j].epreuve[l].note[etudiant] * f->semestres[seum].matieres[j].epreuve[l].coef[k];
								somme_coef += f->semestres[seum].matieres[j].epreuve[l].coef[k];
							}
							/*une division par 0 est impossible, si la somme des coefs est nul et bien on ne peut pas définir de note */
							if (somme_coef <= 0) {
								printf("ND          ");
							}
							else {
								moyenne_matiere = note / somme_coef;
								moyenne_matiere = floorf(moyenne_matiere * 10) / 10;
								printf(" %.1f          ", moyenne_matiere);
							}
						}
						printf("\n");
					}
					printf("--\nMoyennes        ");
					for (unsigned int b = 0; b < f->nbUE; ++b) {
						note = 0;
						somme_coef = 0;
						for (unsigned int a = 0; a < f->semestres[seum].nbMatieres; a++) {
							for (unsigned int c = 0; c < f->semestres[seum].matieres[a].nbEpreuves; ++c) {
								note += f->semestres[seum].matieres[a].epreuve[c].note[etudiant] * f->semestres[seum].matieres[a].epreuve[c].coef[b];
								somme_coef += f->semestres[seum].matieres[a].epreuve[c].coef[b];
							}
						}
						note = note / somme_coef;
						note = floorf(note * 10) / 10;
						printf("%.1f           ", note);
					}
					printf("\n");
				}
				else
					printf("Il manque au moins une note pour cet etudiant\n");
			}
			else
				printf("Les coefficients de ce semestre sont incorrects\n");
		}
		else
			printf("Etudiant inconnu\n");
	}
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxFIN COMMANDE 7xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxDEBUT COMMANDE 8xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


void decision(Formation* f, Etudiants* etu) {
	char nom[31];
	double note = 0;
	double somme_coef = 0;
	int numero_etudiant, note_etudiant_sem1, note_etudiant_sem2, nb_note_sem1, nb_note_sem2;
	float moyenne_S1, moyenne_S2, moyenne_annuel;
	unsigned int compteur_acquis = 0;
	int UeAcquises[MAX_UE] = { 0,0,0,0,0,0 };
	scanf("%s", nom);
	numero_etudiant = Etuverif(nom, etu);
	if (numero_etudiant != -1) {
		if (verif_coef(&f->nbUE, &f->semestres[0]) == 0) {
			if (verif_coef(&f->nbUE, &f->semestres[1]) == 0) {
				/*dans ces variables je peux stocker dans l'une le nombre de note enregistrer à l'étudiant et dans l'autre le nombre d'épreuves (toutes matières confondues) pour ainsi
				vérifier si on a bien enregistré toutes les notes à l'étudiant (fonction crée tout d'abord pour la commande 6 mais qui a pu être réutilisé ici)*/
				note_etudiant_sem1 = Verif_nb_note_etudiant(&f->semestres[0], numero_etudiant);
				note_etudiant_sem2 = Verif_nb_note_etudiant(&f->semestres[1], numero_etudiant);
				nb_note_sem1 = Verif_nb_note(&f->semestres[0]);
				nb_note_sem2 = Verif_nb_note(&f->semestres[1]);
				if (note_etudiant_sem1 == nb_note_sem1 && note_etudiant_sem2 == nb_note_sem2) {
					printf("                     ");
					/*i = 1 car il de UE0 on commence direct à l'UE1, <= f->nbUE car on doit aller jusqu'au  nombre d'UE entrer par l'utilisateur*/
					for (unsigned int i = 1; i <= f->nbUE; ++i) {
						printf("UE%d          ", i);
					}
					printf("\nS1                   ");
					for (unsigned int b = 0; b < f->nbUE; ++b) {
						/*on réinitialise la note à 0, pour chaque UE pour y recalculer une nouvelle moyenne dans cette même variable*/
						note = 0;
						somme_coef = 0;
						for (unsigned int a = 0; a < f->semestres[0].nbMatieres; ++a) {
							for (unsigned int c = 0; c < f->semestres[0].matieres[a].nbEpreuves; ++c) {
								note += f->semestres[0].matieres[a].epreuve[c].note[numero_etudiant] * f->semestres[0].matieres[a].epreuve[c].coef[b];
								somme_coef += f->semestres[0].matieres[a].epreuve[c].coef[b];
							}
						}
						/*floor vient de la bibliothèque math.h elle permet le calcul à l'arrondi entier inférieur*/
						note = note / somme_coef;
						note = floor(note * 10) / 10;
						printf("%.1f         ", note);
					}
					printf("\n");
					printf("S2                   ");
					for (unsigned int b = 0; b < f->nbUE; ++b) {
						/*on réinitialise la note à 0 et la somme des coefs, pour chaque UE pour y recalculer une nouvelle moyenne cette même variable*/
						note = 0;
						somme_coef = 0;
						for (unsigned int a = 0; a < f->semestres[1].nbMatieres; a++) {
							for (unsigned int c = 0; c < f->semestres[1].matieres[a].nbEpreuves; ++c) {

								note += f->semestres[1].matieres[a].epreuve[c].note[numero_etudiant] * f->semestres[1].matieres[a].epreuve[c].coef[b];
								somme_coef += f->semestres[1].matieres[a].epreuve[c].coef[b];
							}
						}
						/*floor vient de la bibliothèque math.h elle permet le calcul à l'arrondi entier inférieur*/
						note = note / somme_coef;
						note = floor(note * 10) / 10;
						printf("%.1f         ", note);
					}
					printf("\n");
					printf("--\nMoyennes annuelles   ");
					for (unsigned int b = 0; b < f->nbUE; ++b) {
						/*on réinitialise la note à 0 et la somme des coefs, pour chaque UE pour y recalculer la moyenne du semestre de 1 de chaque UE*/
						note = 0;
						somme_coef = 0;
						for (unsigned int a = 0; a < f->semestres[0].nbMatieres; a++) {
							for (unsigned int c = 0; c < f->semestres[0].matieres[a].nbEpreuves; ++c) {
								note += f->semestres[0].matieres[a].epreuve[c].note[numero_etudiant] * f->semestres[0].matieres[a].epreuve[c].coef[b];
								somme_coef += f->semestres[0].matieres[a].epreuve[c].coef[b];
							}
						}
						moyenne_S1 = note / somme_coef;
						/*on réinitialise la note à 0 et la somme des coefs, pour chaque UE pour y recalculer la moyenne du semestre de 2 de chaque UE*/
						note = 0;
						somme_coef = 0;
						for (unsigned int a = 0; a < f->semestres[1].nbMatieres; a++) {
							for (unsigned int c = 0; c < f->semestres[1].matieres[a].nbEpreuves; ++c) {
								note += f->semestres[1].matieres[a].epreuve[c].note[numero_etudiant] * f->semestres[1].matieres[a].epreuve[c].coef[b];
								somme_coef += f->semestres[1].matieres[a].epreuve[c].coef[b];
							}
						}
						/*floor vient de la bibliothéque math.h elle permet le calcul à l'arrondi entier inférieur*/
						moyenne_S2 = note / somme_coef;
						moyenne_annuel = (moyenne_S1 + moyenne_S2) / 2;
						printf("%.1f  ", floor(moyenne_annuel * 10) / 10);
					}
					printf("\n");
					printf("Acquisition          ");
					for (unsigned int b = 0; b < f->nbUE; ++b) {
						/*on réinitialise la note ? 0 et la somme des coefs, pour chaque UE pour y recalculer la moyenne du semestre  de 1 de chaque UE*/
						note = 0;
						somme_coef = 0;
						for (unsigned int a = 0; a < f->semestres[0].nbMatieres; a++) {
							for (unsigned int c = 0; c < f->semestres[0].matieres[a].nbEpreuves; ++c) {
								note += f->semestres[0].matieres[a].epreuve[c].note[numero_etudiant] * f->semestres[0].matieres[a].epreuve[c].coef[b];
								somme_coef += f->semestres[0].matieres[a].epreuve[c].coef[b];
							}
						}
						moyenne_S1 = note / somme_coef;
						/*on réinitialise la note à 0 et la somme des coefs, pour chaque UE pour y recalculer la moyenne du semestre de 2 de chaque UE*/
						note = 0;
						somme_coef = 0;
						for (unsigned int a = 0; a < f->semestres[1].nbMatieres; a++) {
							for (unsigned int c = 0; c < f->semestres[1].matieres[a].nbEpreuves; ++c) {
								note += f->semestres[1].matieres[a].epreuve[c].note[numero_etudiant] * f->semestres[1].matieres[a].epreuve[c].coef[b];
								somme_coef += f->semestres[1].matieres[a].epreuve[c].coef[b];
							}
						}
						moyenne_S2 = note / somme_coef;
						moyenne_annuel = (moyenne_S1 + moyenne_S2) / 2;
						if (moyenne_annuel >= 10) {
							compteur_acquis += 1;
							UeAcquises[b] = 1;
						}

					}

					if (compteur_acquis == 0) {
						printf("Aucune");
					}
					else {
						/*la première boucle permet d'afficher si l'UE est valide à la position (i) de mon tableau UeAcquises mais comme il démarre à la position 0 on affiche
						UE d'i + 1 car il n'y a pas de UE0 */
						for (unsigned int i = 0; i < f->nbUE; ++i) {
							if (UeAcquises[i] == 1) {
								printf("UE%d", i + 1);
								/*ici on affiche une virgule seulement s'il y a une UE après, elle verifie si pour toutes les UE suivante l'UEi s'il y a au moins une acquise*/
								for (unsigned int j = i; j < f->nbUE; ++j) {
									if (UeAcquises[j + 1] == 1) {
										printf(", ");
										break;
									}
								}
							}
						}
					}

					printf("\nDevenir              ");

					if (compteur_acquis > f->nbUE / 2) {
						printf("Passage\n");
					}
					else {
						printf("Redoublement\n");
					}

				}
				else
					printf("Il manque au moins une note pour cet etudiant\n");
			}
			else
				printf("Les coefficients d'au moins un semestre sont incorrects\n");
		}
		else
			printf("Les coefficients d'au moins un semestre sont incorrects\n");
	}
	else
		printf("Etudiant inconnu\n");
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxFIN COMMANDE 8xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxDEBUT DU MAINxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


int main() {
	Formation f;
	Etudiants etu;
	Commande vient;
	f.nbUE = 0;

	/*Initialiser mon nombre de matières est important ainsi je peux utiliser mon nbMatieres comme l'indice de ma matière */
	f.semestres[0].nbMatieres = 0;
	f.semestres[1].nbMatieres = 0;

	/*boucle me permettant d'initialiser le nombre d'épreuve de chaque matière à 0, ainsi je peux utiliser mon nbEpreuves comme l'indice sur la position de l'épreuve que je vais enregistrer*/
	for (int i = 0; i < MAX_MATIERES; ++i) {
		f.semestres[0].matieres[i].nbEpreuves = 0;
		f.semestres[1].matieres[i].nbEpreuves = 0;
	}

	/*boucle pour initialiser toutes mes notes à - 1 (car ne peut pas etre une note car - 1 n'est pas compris ou égal à 0 et 20)
	cette initialisation est importante pour ma commande 5 pour savoir si la note est déjà définie ou non*/
	for (int i = 0; i < MAX_MATIERES; ++i) {
		for (int j = 0; j < MAX_EPREUVES; ++j) {
			for (int k = 0; k < MAX_ETUDIANTS; ++k) {
				f.semestres[0].matieres[i].epreuve[j].note[k] = -1;
				f.semestres[1].matieres[i].epreuve[j].note[k] = -1;
			}
		}
	}

	/*tout comme nbMatiere et nbEpreuve j'ai besoin d'initialiser mon nombre d'étudiant à 0 pour ainsi pouvoir incrémenter à chaque fois qu'un étudiant est enregistré
	ainsi l'enregistrer à la position nbEtudiant*/
	etu.nbEtudiant = 0;

	/*permets la saisit au clavier à l'utilisateur autant qu'il le souhaite jusqu'à ce qu'il entre exit */
	while (1) {
		scanf("%s", vient.commande);

		if (!strcmp(vient.commande, "exit")) {
			/*fonction provenant de la bibliothèque standard stdlib.h qui permet de provoquer la fin du programme*/
			exit(0);
		}

		if (!strcmp(vient.commande, "formation")) {
			if (f.nbUE == 0) {
				f.nbUE = formation();
			}
			else
				printf("Le nombre d'UE est deja defini\n");
			continue;
		}

		if (!strcmp(vient.commande, "epreuve")) {
			if (f.nbUE == 0) {
				printf("Le nombre d'UE n'est pas defini\n");
				continue;
			}
			epreuve(&f);
		}

		if (!strcmp(vient.commande, "coefficients")) {
			if (f.nbUE == 0) {
				printf("Le nombre d'UE n'est pas defini\n");
				continue;
			}
			coefficients(&f);
		}

		if (!strcmp(vient.commande, "note")) {
			if (f.nbUE == 0) {
				printf("Le nombre d'UE n'est pas defini\n");
				continue;
			}
			else
				note(&f, &etu);
		}

		if (!strcmp(vient.commande, "notes")) {
			if (f.nbUE == 0) {
				printf("Le nombre d'UE n'est pas defini\n");
				continue;
			}
			else
				notes(&f, &etu);
		}


		if (!strcmp(vient.commande, "releve")) {
			if (f.nbUE == 0) {
				printf("Le nombre d'UE n'est pas defini\n");
				continue;
			}
			else
				releve(&f, &etu);
		}

		if (!strcmp(vient.commande, "decision")) {
			if (f.nbUE == 0) {
				printf("Le nombre d'UE n'est pas defini\n");
				continue;
			}
			else
				decision(&f, &etu);
		}

	}
	return 0;
}

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxFIN DU MAINxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx