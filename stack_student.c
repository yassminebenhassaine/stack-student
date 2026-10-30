#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Etudiant {
    int cne;
    char nom[50];
    char prenom[50];
    struct Etudiant *suivant;
} Etudiant;

typedef struct {
    Etudiant *sommet;
    int taille;
} Pile;

void initialiser(Pile *P) {
    P->sommet = NULL;
    P->taille = 0;
}

int pile_vide(Pile *P) {
    return(P->sommet==0);
    }

int empiler(Pile *P, int cne, char *nom, char *prenom) {
    Etudiant *nouveau;
    nouveau = (Etudiant *)malloc(sizeof(Etudiant));
    if (nouveau == NULL)
        return -1;
    nouveau->cne = cne;

    strcpy(nouveau->nom, nom);
    strcpy(nouveau->prenom, prenom);

    nouveau->suivant = P->sommet;
    P->sommet = nouveau;

    P->taille++;
    return 0;
}

int depiler(Pile *P) {
    Etudiant *temp;
    if (pile_vide(P))
        return -1;
    temp = P->sommet;/* sauvegarder le sommet */
    P->sommet = P->sommet->suivant;/* le sommet devient le suivant element de la pile */
    free(temp);
    P->taille--;
    return 0;
}

void afficher(Pile *P) {
    Etudiant *courant;
    int i;
    if (pile_vide(P)) {
        printf("la pile est vide.\n");
        return;
    }
    courant = P->sommet;//on va deplacer d'un etudiant a l'autre
    for (i = 0; i < P->taille; i++) {
        printf("cne: %d - nom: %s - prenom: %s\n",
               courant->cne, courant->nom, courant->prenom);
        courant = courant->suivant;
    }
}

int rechercher_cne(Pile *P, int cne) {
    Etudiant *courant;
    courant = P->sommet;
    while (courant != NULL) {
        if (courant->cne == cne) {
            printf("trouve -> cne: %d - nom: %s - prenom: %s\n",
                   courant->cne, courant->nom, courant->prenom);
            return 1;
        }
        courant = courant->suivant;
    }
    printf("etudiant non trouve.\n");
    return 0;
}

int sommet(Pile *P) {
    if (pile_vide(P)) {
        printf("La pile est vide.\n");
        return -1;
    }
    printf("sommet -> cne: %d - nom: %s - prenom: %s\n",
           P->sommet->cne, P->sommet->nom, P->sommet->prenom);
    return 0;
}

int inserer_base(Pile *P, int cne, char *nom, char *prenom) {
    Pile temp;//nv pile temp
    initialiser(&temp);//vide
    while (!pile_vide(P)) {//si pile pas vide
        empiler(&temp, P->sommet->cne, P->sommet->nom, P->sommet->prenom);
        depiler(P);
    }
    empiler(P, cne, nom, prenom);//ajouter le nv dans pile
    while (!pile_vide(&temp)) {
        empiler(P, temp.sommet->cne, temp.sommet->nom, temp.sommet->prenom);
        depiler(&temp);
    }
    return 0;
}

void inverser_pile(Pile *P) {
    Pile temp;
    initialiser(&temp);
    while (!pile_vide(P)) {
        empiler(&temp, P->sommet->cne, P->sommet->nom, P->sommet->prenom);
        depiler(P);
    }
    while (!pile_vide(&temp)) {
        empiler(P, temp.sommet->cne, temp.sommet->nom, temp.sommet->prenom);
        depiler(&temp);
    }
}

int supprimer_par_cne(Pile *P, int cne, Pile *historique) {
    Pile temp;
    int trouve;
    initialiser(&temp);
    trouve = 0;
    while (!pile_vide(P)) {
        if (P->sommet->cne == cne && trouve == 0) {
            empiler(historique, P->sommet->cne, P->sommet->nom, P->sommet->prenom);
            depiler(P);
            trouve = 1;
        } else {
            empiler(&temp, P->sommet->cne, P->sommet->nom, P->sommet->prenom);
            depiler(P);
        }
    }
    while (!pile_vide(&temp)) {
        empiler(P, temp.sommet->cne, temp.sommet->nom, temp.sommet->prenom);
        depiler(&temp);
    }
    if (trouve == 0)
        printf("cne non trouve.\n");
    return trouve;
}

int annuler_suppression(Pile *P, Pile *historique) {
    if (pile_vide(historique)) {
        printf("aucune suppression a annuler.\n");
        return -1;
    }
    empiler(P, historique->sommet->cne,
               historique->sommet->nom,
               historique->sommet->prenom);
    depiler(historique);
    printf("suppression annulee.\n");
    return 0;
}

int fusionner_piles(Pile *p1, Pile *p2, Pile *p3) {
    Etudiant *courant;
    initialiser(p3);
    courant = p1->sommet;
    while (courant != NULL) {
        inserer_base(p3, courant->cne, courant->nom, courant->prenom);
        courant = courant->suivant;
    }
    courant = p2->sommet;
    while (courant != NULL) {
        inserer_base(p3, courant->cne, courant->nom, courant->prenom);
        courant = courant->suivant;
    }
    return 0;
}

int taille_pile(Pile *P) {
    return P->taille;
}

void afficher_etudiant_nom_long(Pile *P, int n) {
    Etudiant *courant;
    int trouve;
    courant = P->sommet;
    trouve = 0;
    printf("etudiants avec un nom de %d caractere(s) :\n", n);
    while (courant != NULL) {
        if (strlen(courant->nom) == n) {
            printf("cne: %d - nom: %s - prenom: %s\n",
                   courant->cne, courant->nom, courant->prenom);
            trouve = 1;
        }
        courant = courant->suivant;
    }
    if (trouve == 0)
        printf("aucun etudiant trouve.\n");
}

int compter_prenom(Pile *P, char *lettre) {
    Etudiant *courant;
    int compteur;
    courant = P->sommet;
    compteur = 0;
    while (courant != NULL) {
        if (courant->prenom[0] == lettre[0])
            compteur++;
        courant = courant->suivant;
    }
    printf("nombre d'etudiants dont le prenom commence par '%c' : %d\n",
           lettre[0], compteur);
    return compteur;
}

void trier_temporairement(Pile *P) {
    int tab[100];
    Etudiant *courant;
    int i, j, temp, n;
    if (pile_vide(P)) {
        printf("la pile vide.\n");
        return;
    }
    n = P->taille;
    courant = P->sommet;
    i = 0;
    while (courant != NULL) {
        tab[i] = courant->cne;
        i++;
        courant = courant->suivant;
    }
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (tab[j] > tab[j+1]) {
                temp = tab[j];
                tab[j] = tab[j+1];
                tab[j+1] = temp;
            }
        }
    }
    printf("etudiants tries par cne croissant :\n");
    for (i = 0; i < n; i++) {
        courant = P->sommet;
        while (courant != NULL) {
            if (courant->cne == tab[i]) {
                printf("cne: %d - nom: %s - prenom: %s\n",
                       courant->cne, courant->nom, courant->prenom);
                break;
            }
            courant = courant->suivant;
        }
    }
}

void menu(Pile *P, Pile *historique) {
    int choix, cne, n;
    char nom[50], prenom[50], lettre[2];

    do {
        printf("1.  Initialiser la pile\n");
        printf("2.  Empiler un etudiant\n");
        printf("3.  Depiler\n");
        printf("4.  Afficher la pile\n");
        printf("5.  Recherche par CNE\n");
        printf("6.  Afficher le sommet\n");
        printf("7.  Inserer a la base\n");
        printf("8.  Inverser la pile\n");
        printf("9.  Supprimer par CNE\n");
        printf("10. Annuler la derniere suppression\n");
        printf("11. Afficher etudiants par longueur nom\n");
        printf("12. Compter prenoms commencant par une lettre\n");
        printf("13. Afficher la taille\n");
        printf("0.  Quitter\n");
        printf("votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                initialiser(P);
                printf("Pile initialisee.\n");
                break;
            case 2:
                printf("cne : ");
                scanf("%d", &cne);
                getchar();
                printf("nom : ");
                fgets(nom, 50, stdin);
                nom[strcspn(nom, "\n")] = '\0';
                printf("prenom : ");
                fgets(prenom, 50, stdin);
                prenom[strcspn(prenom, "\n")] = '\0';
                if (empiler(P, cne, nom, prenom) == 0)
                    printf("empile avec succes.\n");
                else
                    printf("echec.\n");
                break;
            case 3:
                if (depiler(P) == 0)
                    printf("depile avec succes.\n");
                else
                    printf("pile vide.\n");
                break;
            case 4:
                afficher(P);
                break;
            case 5:
                printf("cne a rechercher : "); scanf("%d", &cne);
                rechercher_cne(P, cne);
                break;
            case 6:
                sommet(P);
                break;
            case 7:
                printf("cne : "); scanf("%d", &cne);
                printf("nom : "); scanf("%s", nom);
                printf("prenom : "); scanf("%s", prenom);
                inserer_base(P, cne, nom, prenom);
                printf("insere a la base.\n");
                break;
            case 8:
                inverser_pile(P);
                printf("pile inversee.\n");
                break;
            case 9:
                printf("cne a supprimer : "); scanf("%d", &cne);
                if (supprimer_par_cne(P, cne, historique) == 1)
                    printf("supprime et sauvegarde.\n");
                break;
            case 10:
                annuler_suppression(P, historique);
                break;
            case 11:
                printf("longueur du nom : "); scanf("%d", &n);
                afficher_etudiant_nom_long(P, n);
                break;
            case 12:
                printf("lettre : "); scanf("%s", lettre);
                compter_prenom(P, lettre);
                break;
            case 13:
                printf("taille de la pile : %d\n", taille_pile(P));
                break;
            case 0:
                printf("quitter avec succee. \n");
                break;
            default:
                printf("choix invalide.\n");
        }
    } while (choix != 0);
}

int main() {
    Pile p, historique;
    initialiser(&p);
    initialiser(&historique);
    menu(&p, &historique);
    return 0;
}
