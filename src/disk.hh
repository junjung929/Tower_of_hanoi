/*
 * disk.hh
 *
 * Author: Jun Jung
 * Student Number: 292961
 * Email: jun.jung@tuni.fi
 *
 * This file contains disk item of tower of hanoi game.
 */
#ifndef DISK_HH
#define DISK_HH

#include <QBrush>
#include <QGraphicsRectItem>

class Peg;      // implicit class

// Custom item type
enum ItemType { TypePeg = QGraphicsRectItem::UserType + 1,
                TypeDisk = QGraphicsRectItem::UserType + 2 };

class Disk : public QGraphicsRectItem
{
public:
    Disk(int id);

    int  getId()    const;
    bool getIsTop() const;
    Peg  *getPeg()  const;

    void setIsTop   (bool isTop);
    void setPeg     (Peg  *peg);

    // Custom type
    enum {Type = ItemType::TypeDisk};
    int type() const override;

private:
    // the smaller disk size, the smaller id
    int     id_;
    bool    is_top_;    // Whether disk is located on top or not
    Peg     *peg_;      // Where disk is located

    // Disk scale
    const int WIDTH             = 40;
    const int WIDTH_INCREASE    = 20;
    const int HEIGHT            = 20;

    QBrush brush_;
};

#endif // DISK_HH
