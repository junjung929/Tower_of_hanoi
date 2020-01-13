/*
 * disk.cpp
 *
 * This file is a source file of disk.hh
 */

#include "disk.hh"

#include <QDebug>
#include <QDrag>

Disk::Disk(int id):
    id_(id) //, peg_(nullptr)
{
    is_top_ = false;
    brush_ = QBrush(Qt::red);

    this->setRect(0, 0, id*WIDTH_INCREASE + WIDTH, HEIGHT);
    this->setBrush(brush_);
    this->setFlag(QGraphicsItem::ItemIsMovable);
}

int Disk::getId() const
{
    return id_;
}

bool Disk::getIsTop() const
{
    return is_top_;
}

Peg *Disk::getPeg() const
{
    return peg_;
}

void Disk::setIsTop(bool isTop)
{
    is_top_ = isTop;
}

void Disk::setPeg(Peg *peg)
{
    peg_ = peg;
}

int Disk::type() const
{
    return Type;
}
