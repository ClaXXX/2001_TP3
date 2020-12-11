/**
 * \file block.cpp
 * \brief Implémentation d'un bloc.
 * \author ?
 * \version 0.1
 * \date nov-dec 2020
 *
 *  Travail pratique numéro 3
 *
 */

#include "block.h"
#include "disqueVirtuel.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP3
{
	// Ajouter votre code ici !

	/**
	 * Constructor use for boot block & super block
	 */
    Block::Block() : m_type_donnees(0), m_inode(nullptr), m_dirEntry(std::vector<dirEntry*>()) {

    }

    Block::Block(size_t td): m_type_donnees(td), m_inode(nullptr), m_dirEntry(std::vector<dirEntry*>()) {
      switch (td) {
        case S_IFBL: // initialise le bitmap des block libres
          m_bitmap = std::vector<bool>(N_BLOCK_ON_DISK, true); // par default, tous les blocks sont libres
          break;
        case S_IFIL: // initialise le bitmap des inodes libres
          m_bitmap = std::vector<bool>(N_INODE_ON_DISK, true); // par default, tous les inodes sont libres
          break;
      }
    }

    Block::~Block() {
      // TODO:
      //delete m_inode;
      //for (auto dir: m_dirEntry) delete dir;
      m_dirEntry.clear();
      m_inode = nullptr;
    }
}

//Fin du namespace




