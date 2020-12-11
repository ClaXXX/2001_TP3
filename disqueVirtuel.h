/**
 * \file disqueVirtuel.h
 * \brief Gestion d'un disque virtuel.
 * \author IFT-2001
 * \version 0.1
 * \date nov-dec 2020
 *
 *  Travail pratique numéro 3
 *
 */

#include "block.h"

#ifndef _DISQUEVIRTUEL__H
#define _DISQUEVIRTUEL__H

namespace TP3
{

#define N_INODE_ON_DISK   20	// nombre maximal d'i-nodes (donc de fichiers) sur votre disque
#define N_BLOCK_ON_DISK 128	// nombre de blocs sur le disque au complet
#define FREE_BLOCK_BITMAP 2	// numero du bloc contenant le bitmap des block libres
#define FREE_INODE_BITMAP 3	// numero du bloc contenant le bitmap des i-nodes libres
#define BASE_BLOCK_INODE  4     // bloc de depart ou les i-nodes sont stockes sur disque
#define ROOT_INODE        1     // numero du i-node correspondant au repertoire racine

class DisqueVirtuel {
public:
	DisqueVirtuel();
	~DisqueVirtuel();

	// Méthodes demandées
	int bd_FormatDisk();
	std::string bd_ls(const std::string& p_DirLocation);
	int bd_mkdir(const std::string& p_DirName);
	int bd_create(const std::string& p_FileName);
	int bd_rm(const std::string& p_Filename);

	// Vous pouvez ajouter ici d'autres méthodes publiques

private:
	// Il est interdit de modifier ce modèle d'implémentation (i.e le type de m_blockDisque)!
    std::vector<Block> m_blockDisque; // Un vecteur de blocs représentant le disque virtuel

    // Vous pouvez ajouter ici des méthodes privées

    int inode_create(size_t st_ino);
    int inode_take(size_t st_ino);
    int inode_free(size_t st_ino);
    iNode *inode_findFirtFree();

    int block_take(size_t block_index);
    int block_free(size_t block_index);
    size_t block_findFirstFree() const;

    void repository_addLink(iNode *ino, const size_t index, const std::string &fileName);
    void repository_removeLink(std::vector<dirEntry*>& dirEntrys, size_t index);
    void repository_addParentLink(iNode *parent, iNode *ino, const std::string &fileName);
    void repository_removeParentLink(size_t parentIno, size_t childIno);
    const iNode *repository_createEmpty(iNode *parent, const std::string &fileName, const size_t mode);
    iNode *repository_get(const std::string &filename);
    int repository_create(const std::string &filename, const size_t mode);
    int repository_remove(const std::string &filename);
};
}//Fin du namespace

#endif
