# 2001_TP3
Linux file system simple simulator.

## Consignes
Ce travail pratique a pour objectif d'implémenter un mini-système de fichiers en s'inspirant du système UFS (Unix File System) que nous avons vu dans le cours Système d'exploitation surtout qu'il est la base de nombreux systèmes de fichiers modernes (ext2, ext3 et ext4, par exemple). Dans ce travail nous devons nous familiariser entre autres avec les opérations nécessaires pour gérer les fichiers et les impacts sur les métadonnées, la traduction entre un nom de fichier et un numéro d'i-node et les patrons d'accès au disque résultants d'opérations simples comme ls, mkdir, rm, etc...

## Tests
Sortie des fichiers de test.
### Test 1
```shell
===== Commande format =====
UFS: saisir i-node 0
UFS: saisir i-node 1
UFS: saisir bloc 24

===== Commande ls / =====
/
d              . Size:   56 inode:    1 nlink:    2
d             .. Size:   56 inode:    1 nlink:    2

===== Commande mkdir /doc =====
UFS: saisir i-node 2
UFS: saisir bloc 25

===== Commande ls / =====
/
d              . Size:   84 inode:    1 nlink:    3
d             .. Size:   84 inode:    1 nlink:    3
d            doc Size:   56 inode:    2 nlink:    2

===== Commande mkdir /tmp =====
UFS: saisir i-node 3
UFS: saisir bloc 26

===== Commande ls / =====
/
d              . Size:  112 inode:    1 nlink:    4
d             .. Size:  112 inode:    1 nlink:    4
d            doc Size:   56 inode:    2 nlink:    2
d            tmp Size:   56 inode:    3 nlink:    2

===== Commande mkdir /tmp/lib =====
UFS: saisir i-node 4
UFS: saisir bloc 27

===== Commande ls / =====
/
d              . Size:  112 inode:    1 nlink:    4
d             .. Size:  112 inode:    1 nlink:    4
d            doc Size:   56 inode:    2 nlink:    2
d            tmp Size:   84 inode:    3 nlink:    3

===== Commande ls /tmp =====
/tmp
d              . Size:   84 inode:    3 nlink:    3
d             .. Size:  112 inode:    1 nlink:    4
d            lib Size:   56 inode:    4 nlink:    2

===== Commande mkdir /tmp/lib/deep =====
UFS: saisir i-node 5
UFS: saisir bloc 28

===== Commande ls /tmp/lib/deep =====
/tmp/lib/deep
d              . Size:   56 inode:    5 nlink:    2
d             .. Size:   84 inode:    4 nlink:    3

===== Commande rm /tmp/lib/deep =====
UFS: Relache i-node 5
UFS: Relache bloc 28

===== Commande rm /tmp/lib =====
UFS: Relache i-node 4
UFS: Relache bloc 27

===== Commande rm /tmp =====
UFS: Relache i-node 3
UFS: Relache bloc 26

===== Commande ls / =====
/
d              . Size:   84 inode:    1 nlink:    3
d             .. Size:   84 inode:    1 nlink:    3
d            doc Size:   56 inode:    2 nlink:    2

===== Commande create /a.txt =====
UFS: saisir i-node 3
UFS: saisir bloc 26

===== Commande ls / =====
/
d              . Size:  112 inode:    1 nlink:    3
d             .. Size:  112 inode:    1 nlink:    3
d            doc Size:   56 inode:    2 nlink:    2
-          a.txt Size:    0 inode:    3 nlink:    1

===== Commande rm /a.txt =====
UFS: Relache i-node 3
UFS: Relache bloc 26

===== Commande ls / =====
/
d              . Size:   84 inode:    1 nlink:    3
d             .. Size:   84 inode:    1 nlink:    3
d            doc Size:   56 inode:    2 nlink:    2

===== Commande format =====
UFS: saisir i-node 0
UFS: saisir i-node 1
UFS: saisir bloc 24

===== Commande ls / =====
/
d              . Size:   56 inode:    1 nlink:    2
d             .. Size:   56 inode:    1 nlink:    2
```

### Test 2
```shell
===== Commande format =====
UFS: saisir i-node 0
UFS: saisir i-node 1
UFS: saisir bloc 24

===== Commande ls / =====
/
d              . Size:   56 inode:    1 nlink:    2
d             .. Size:   56 inode:    1 nlink:    2

===== Commande ls /doc =====
doc: File not found.

===== Commande mkdir /new =====
UFS: saisir i-node 2
UFS: saisir bloc 25

===== Commande mkdir /empty =====
UFS: saisir i-node 3
UFS: saisir bloc 26

===== Commande mkdir /notempty =====
UFS: saisir i-node 4
UFS: saisir bloc 27

===== Commande mkdir /existepas/new =====
existepas: File not found.

===== Commande create /notempty/c.txt =====
UFS: saisir i-node 5
UFS: saisir bloc 28

===== Commande create /b.txt =====
UFS: saisir i-node 6
UFS: saisir bloc 29

===== Commande create /existepas/b.txt =====
existepas: File not found.

===== Commande ls / =====
/
d              . Size:  168 inode:    1 nlink:    5
d             .. Size:  168 inode:    1 nlink:    5
d            new Size:   56 inode:    2 nlink:    2
d          empty Size:   56 inode:    3 nlink:    2
d       notempty Size:   84 inode:    4 nlink:    2
-          b.txt Size:    0 inode:    6 nlink:    1

===== Commande ls /notempty =====
/notempty
d              . Size:   84 inode:    4 nlink:    2
d             .. Size:  168 inode:    1 nlink:    5
-          c.txt Size:    0 inode:    5 nlink:    1

===== Commande rm /notempty =====
/notempty: Not empty.

===== Commande ls / =====
/
d              . Size:  168 inode:    1 nlink:    5
d             .. Size:  168 inode:    1 nlink:    5
d            new Size:   56 inode:    2 nlink:    2
d          empty Size:   56 inode:    3 nlink:    2
d       notempty Size:   84 inode:    4 nlink:    2
-          b.txt Size:    0 inode:    6 nlink:    1

===== Commande rm /notempty/c.txt =====
UFS: Relache i-node 5
UFS: Relache bloc 28

===== Commande rm /notempty =====
UFS: Relache i-node 4
UFS: Relache bloc 27

===== Commande ls / =====
/
d              . Size:  140 inode:    1 nlink:    4
d             .. Size:  140 inode:    1 nlink:    4
d            new Size:   56 inode:    2 nlink:    2
d          empty Size:   56 inode:    3 nlink:    2
-          b.txt Size:    0 inode:    6 nlink:    1

===== Commande rm /a.txt =====
a.txt: File not found.

===== Commande ls / =====
/
d              . Size:  140 inode:    1 nlink:    4
d             .. Size:  140 inode:    1 nlink:    4
d            new Size:   56 inode:    2 nlink:    2
d          empty Size:   56 inode:    3 nlink:    2
-          b.txt Size:    0 inode:    6 nlink:    1

===== Commande format =====
UFS: saisir i-node 0
UFS: saisir i-node 1
UFS: saisir bloc 24

===== Commande ls / =====
/
d              . Size:   56 inode:    1 nlink:    2
d             .. Size:   56 inode:    1 nlink:    2
```