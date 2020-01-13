/*
 * peg.hh
 *
 * Author: Jun Jung
 * Student Number: 292961
 * Email: jun.jung@tuni.fi
 *
 * This file is subclass of QGraphicsRectItem.
 * Each peg contains information of disks only located at the peg.
 *
 * The disk vector could be implemented as linked list, but I faced memory leaking,
 * and couldn't figure it out.
 *
 */
#ifndef PEG_HH
#define PEG_HH

#include "disk.hh"

#include <QGraphicsRectItem>
#include <vector>

using namespace std;

class Peg: public QGraphicsRectItem
{
public:
    Peg(char name);

    char getName() const;

    // Disk operators
    bool addDisk        (Disk *diskToAdd);
    bool removeDisk     (Disk *diskToRemove);
    bool moveDiskTo     (Peg  *moveTo);
    bool moveDiskFrom   (Disk *diskToMove, Peg *moveFrom);

    // Estra functions
    void drawDisks  ();
    bool checkWin   (vector<Disk *> disks);

    // Custom type
    enum {Type = ItemType::TypePeg};
    int type() const override;

private:
    // Peg scale
    const int WIDTH     = 20;
    const int HEIGHT    = 200;

    char name_;

    // Disk
    Disk            *top_;
    vector<Disk *>  disks_;

};

#endif // PEG_HH
