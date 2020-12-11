/**
 * \file disqueVirtuel.cpp
 * \brief Implémentation d'un disque virtuel.
 * \author ?
 * \version 0.1
 * \date nov-dec 2020
 *
 *  Travail pratique numéro 3
 *
 */

#include "disqueVirtuel.h"
#include <iostream>
#include <string>
//vous pouvez inclure d'autres librairies si c'est nécessaire
#include <sstream>      // std::istringstream
#include <algorithm>    // std::find
#include <iomanip>

namespace TP3
{
    /**
     * Utils
     **/

    /**
     * find first element for which function argument doesn't return true
     * @tparam T element type
     * @param vector
     * @param function lambda function to test when passing on each element of the vector
     * @return index of the element found, the size of the vector otherwise
     */
    template<typename T>
    size_t find_first_of(const std::vector<T> &vector, T (*function)(T)) {
      size_t i = 0;

      for (; i < vector.size() && function(vector.at(i)); i++);
      return i;
    }

    /**
     * return the value of error and print the problem
     * @tparam T: type of the return result
     * @param return_value: return value
     * @param message: message to print on the error out
     * @return return_value
     */
    template <typename T>
    T error_print(const T return_value, const std::string& message) {
      std::cerr << message << std::endl;
      return return_value;
    }

	// Ajouter votre code ici !

	/**
	 * Constructor & Destructor
	 */
    DisqueVirtuel::DisqueVirtuel() : m_blockDisque(std::vector<Block>()) {
    }

    DisqueVirtuel::~DisqueVirtuel() = default;

    /**
     * Inode management
     */

    /**
     * Create an empty inode and emplace it on the disk
     * @param st_ino: iNode number
     * @return 1 in Success, 0 otherwise
     */
    int DisqueVirtuel::inode_create(size_t st_ino) {
      auto *i_node = new iNode(st_ino, 0, 0, 0, 0);

      m_blockDisque.emplace_back(S_IFIN);
      m_blockDisque.back().m_inode = i_node;
      return 1;
    }

    /**
     * Take inode
     * @param st_ino number of target inode
     * @return 1 after the inode has been taken, 0 if an error occurs
     */
    int DisqueVirtuel::inode_take(size_t st_ino) {
      if (m_blockDisque.at(FREE_INODE_BITMAP).m_bitmap.size() <= st_ino
      || !m_blockDisque.at(FREE_INODE_BITMAP).m_bitmap[st_ino]) { // If the inode was already taken, raise an error
        return error_print(0, "UFS: saisir i-node " + std::to_string(st_ino) + " impossible");
      }
      m_blockDisque.at(FREE_INODE_BITMAP).m_bitmap[st_ino] = false;
      std::cout << "UFS: saisir i-node " << st_ino << std::endl;
      return 1;
    }

    /**
     * Free inode
     * @param st_ino number of the inode to free
     * @return 1 after the inode has been free, 0 if an error occurs
     */
    int DisqueVirtuel::inode_free(size_t st_ino) {
      if (m_blockDisque.at(FREE_INODE_BITMAP).m_bitmap.size() <= st_ino
      || m_blockDisque.at(FREE_INODE_BITMAP).m_bitmap[st_ino]) { // If the inode was already free, raise an error
        return error_print(0, "UFS: Relache i-node " + std::to_string(st_ino) + " impossible");
      }
      m_blockDisque.at(FREE_INODE_BITMAP).m_bitmap[st_ino] = true;
      std::cout << "UFS: Relache i-node " << st_ino << std::endl;
      return 1;
    }

    /**
     * Méthode permettant de trouver le premier i-node libre
     * @throw out of range
     * @return le premier i-node libre
     */
    iNode* DisqueVirtuel::inode_findFirtFree() {
      return m_blockDisque.at(BASE_BLOCK_INODE
          + find_first_of<bool>(m_blockDisque.at(FREE_INODE_BITMAP).m_bitmap,
                                [](bool value)->bool { return !(value); })
          ).m_inode;
    }

    /**
     * Block management
     */

    /**
     * Take block
     * @param st_ino numéro du bloc
     * @return 1 si le bloc a pu être pris sans erreur, 0 autrement
     */
    int DisqueVirtuel::block_take(size_t block_index) {
      if (m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap.size() <= block_index
      || !m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap[block_index]) { // If the inode was already free, raise an error
        return error_print(0, "UFS: saisir bloc " + std::to_string(block_index) + " impossible");
      }
      m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap[block_index] = false;
      std::cout << "UFS: saisir bloc " << block_index << std::endl;
      return 1;
    }

    /**
     * Free block
     * @param st_ino numéro du bloc
     * @return 1 si le bloc a pu être libéré sans erreur, 0 autrement
     */
    int DisqueVirtuel::block_free(size_t block_index) {
      if (m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap.size() <= block_index
      || m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap[block_index]) { // If the inode was already free, raise an error
        return error_print(0, "UFS: Relache bloc " + std::to_string(block_index) + " impossible");
      }
      m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap[block_index] = true;
      std::cout << "UFS: Relache bloc " << block_index << std::endl;
      return 1;
    }

    /**
     * Méthode permettant de trouver le premier bloc de données libre
     * @throw out_of_range
     * @return l'index du premier bloc de données libre
     */
    size_t DisqueVirtuel::block_findFirstFree() const {
      return find_first_of<bool>(m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap,
                                 [](bool value)->bool { return !(value); });
    }

    /**
     * Repository management
     */

    /**
     * Ajoute un lien vers un fichier dans un repository (directory)
     * @param ino : lien vers cet i-node
     * @param index : index du bloc du dossier à modifier
     * @param fileName : nom du lien
     */
    void DisqueVirtuel::repository_addLink(iNode *ino, const size_t index, const std::string& fileName) {
      m_blockDisque.at(index).m_dirEntry.emplace_back(new dirEntry(ino->st_ino, fileName));
      ino->st_nlink += 1;
    }

    /**
     * Supprime un lien vers un fichier dans un repository (directory)
     * @param dirEntrys : vecteur des liens du directory
     * @param index : index du lien à supprimer dans le vecteur dirEntry
     */
    void DisqueVirtuel::repository_removeLink(std::vector<dirEntry*>& dirEntrys, size_t index) {
      iNode *ino = m_blockDisque.at(BASE_BLOCK_INODE + dirEntrys.at(index)->m_iNode).m_inode;

      ino->st_nlink-=1;
      delete dirEntrys.at(index);
      dirEntrys.erase(dirEntrys.cbegin() + index, dirEntrys.cbegin() + index + 1);
      if (!ino->st_nlink) { // Si plus aucun lien ne pointe sur un repository, alors libère les données qui y sont liées
        inode_free(ino->st_ino);
        block_free(ino->st_block);
      }
    }

    /**
     * Ajoute le lien au dossier parent d'un fichier
     * @param parent : i-node parent
     * @param ino : i-node du fichier vers lequel faire le lien
     * @param fileName : nom du fichier vers lequel faire le lien
     */
    void DisqueVirtuel::repository_addParentLink(iNode* parent, iNode* ino, const std::string& fileName) {
      if (parent) { // si jamais on ne crée pas la root parent
        repository_addLink(ino, parent->st_block, fileName); // ajoute le lien vers le nouveau repo pour le parent
        parent->st_size += 28;
      } else {
        parent = ino;
      }
      if (ino->st_mode == S_IFDIR) { // si le repo créé est un dossier, alors créer ses liens par défault
        repository_addLink(ino, ino->st_block, ".");
        repository_addLink(parent, ino->st_block, "..");
        ino->st_size += 28*2;
      }
    }

    /**
     * Supprime le lien du dossier parent d'un fichier
     * @param parentIno : numéro d'i-node du parent où se situe le lien
     * @param childIno : numéro d'i-node du fichier vers lequel pointait le lien
     */
    void DisqueVirtuel::repository_removeParentLink(size_t parentIno, size_t childIno) {
      iNode* parent = m_blockDisque.at(BASE_BLOCK_INODE + parentIno).m_inode;
      Block& block = m_blockDisque.at(parent->st_block);
      size_t i = 0;

      for (; i < block.m_dirEntry.size() && block.m_dirEntry.at(i)->m_iNode != childIno; i++);
      repository_removeLink(block.m_dirEntry, i);
      parent->st_size-=28;
    }

    /**
     * Méthode créant un répertoire vide
     * @param parent: Inode du dossier parent
     * @param fileName: nom du repertoire à creer
     * @param mode: dossier ou fichier
     * @return l'iNode du repertoire créé
     */
    const iNode *DisqueVirtuel::repository_createEmpty(iNode* parent, const std::string& fileName, const size_t mode) {
      iNode *ino = inode_findFirtFree(); // Créer une erreur en cas de problème sur la mémoire
      const size_t block_index = block_findFirstFree();

      if (m_blockDisque.size() < block_index || block_index > N_BLOCK_ON_DISK) // Cas d'erreur
        return nullptr;
      if (m_blockDisque.size() == block_index)
        m_blockDisque.emplace_back(S_IFDE);
      ino->st_block = block_index;
      ino->st_mode = mode;
      repository_addParentLink(parent, ino, fileName);
      inode_take(ino->st_ino);
      block_take(block_index);
      return ino;
    }

    /**
     * à partir d'un path, trouve un fichier
     * @param filename: path du fichier à trouver
     * @return l'i-node du fichier
     */
    iNode* DisqueVirtuel::repository_get(const std::string& filename) {
      std::stringstream ss (filename);
      std::string file;
      iNode* ino = m_blockDisque.at(BASE_BLOCK_INODE + ROOT_INODE).m_inode;
      Block parent = m_blockDisque.at(ino->st_block);
      size_t i;

      std::getline(ss, file, '/'); // root file
      while (std::getline(ss, file, '/')) {
        for (i = 0; i < parent.m_dirEntry.size() && parent.m_dirEntry.at(i)->m_filename != file; i++);
        if (i == parent.m_dirEntry.size())
          return error_print(nullptr, file + ": File not found.");
        ino = m_blockDisque.at(BASE_BLOCK_INODE + parent.m_dirEntry.at(i)->m_iNode).m_inode;
        parent = m_blockDisque.at(ino->st_block);
      }
      return ino;
    }

    int DisqueVirtuel::repository_create(const std::string& filename, const size_t mode) {
      iNode* dir = repository_get(filename.substr(0, filename.find_last_of('/')));

      if (!dir)
        return 0;
      for (auto files: m_blockDisque.at(dir->st_block).m_dirEntry) {
        if (files->m_filename == filename.substr(filename.find_last_of('/'))) {
          return error_print(0, "File already exist.");
        }
      }
      return !!repository_createEmpty(dir, filename.substr(filename.find_last_of('/') + 1), mode);
    }


    int DisqueVirtuel::repository_remove(const std::string& filename) {
      iNode* ino = repository_get(filename);
      iNode* parent = repository_get(filename.substr(0, filename.find_last_of('/')));

      if (!ino)
        return 0;
      if (ino->st_mode == S_IFDIR && m_blockDisque.at(ino->st_block).m_dirEntry.size() > 2)
        return error_print(0, filename + ": Not empty.");
      repository_removeParentLink(parent->st_ino, ino->st_ino);
      for (int i = m_blockDisque.at(ino->st_block).m_dirEntry.size() - 1; i >= 0; i--) {
        repository_removeLink(m_blockDisque.at(ino->st_block).m_dirEntry, i);
        ino->st_size -= 28;
      }
      return 1;
    }

    int DisqueVirtuel::bd_FormatDisk() {
      m_blockDisque.clear(); // supprime tous les blocks qui pouvaient exister
      m_blockDisque.assign(2, Block()); // Creer les blocks boot et superboot (0 et 1)
      m_blockDisque.emplace_back(S_IFBL); // Bitmap des blocks libres
      m_blockDisque.emplace_back(S_IFIL); // Bitmap des inodes libres

      // Creer tous les inodes
      for (size_t i = 0; i < N_INODE_ON_DISK && inode_create(i); i++);
      // Prend tous les block jusqu'au bloc root (laisse le bloc root libre pour l'instant)
      for (size_t i = 0; i < 24; i++) m_blockDisque.at(FREE_BLOCK_BITMAP).m_bitmap[i] = false;
      if (!inode_take(0))
        return 0;
      return !!repository_createEmpty(nullptr, "", S_IFDIR); // creer le dossier root
    }

    int DisqueVirtuel::bd_create(const std::string &p_FileName) {
      return repository_create(p_FileName, S_IFREG);
    }

    int DisqueVirtuel::bd_mkdir(const std::string &p_DirName) {
      return repository_create(p_DirName, S_IFDIR);
    }

    std::string DisqueVirtuel::bd_ls(const std::string &p_DirLocation) {
      iNode* ino = repository_get(p_DirLocation);
      std::ostringstream ss;

      if (!ino)
        return std::string();
      ss << p_DirLocation << std::endl;
      for (auto dir : m_blockDisque.at(ino->st_block).m_dirEntry) {
        iNode* dirIno = m_blockDisque.at(BASE_BLOCK_INODE+dir->m_iNode).m_inode;
        ss << ((dirIno->st_mode == S_IFDIR) ? 'd' : '-')
          << std::setw(15) << dir->m_filename
          << " Size:" << std::setw(5) << dirIno->st_size
          << " inode:" << std::setw(5) << dirIno->st_ino
          << " nlink:" << std::setw(5) << dirIno->st_nlink
          << std::endl;
      }
      return ss.str();
    }

    int DisqueVirtuel::bd_rm(const std::string &p_Filename) {
      return repository_remove(p_Filename);
    }
}//Fin du namespace
