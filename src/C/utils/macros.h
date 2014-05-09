/** \file
 * Ce fichier contient des macros outils pour faciliter le développement.
 */
#ifndef MACROS_PERSO_H
#define MACROS_PERSO_H

/**
 * Indique si a est impair. si oui 1 est retourné, 0 sinon.
 */
#define IS_ODD(a) ((a) & 1)
/**
 * Indique si a est pair. si oui 1 est retourné, 0 sinon.
 */
#define IS_EVEN(a) (!((a) & 1))

#endif