/*
 * peg.cpp
 *
 * This file is a source file of peg.cpp
 */

#include "peg.hh"

#include <algorithm>
#include <QDebug>

Peg::Peg(char name): name_(name), top_(nullptr)
{
    this->setRect(0, 0, WIDTH, HEIGHT);
    this->setAcceptDrops(true);
}

char Peg::getName() const
{
    return name_;
}

bool Peg::addDisk(Disk *diskToAdd)
{
    // Check if the disk already exists.
    // Returns false if exists.
    if(find(disks_.begin(), disks_.end(), diskToAdd) != disks_.end()){ return false; }
    // Set the top disk if the peg is empty
    if(disks_.empty()) {
        top_ = diskToAdd;
        top_->setIsTop(true);
    } else {
        // If the new disk is smaller than the top disk,
        // set the new disk as the top of the peg.
        if(top_->getId() > diskToAdd->getId()) {
            top_->setIsTop(false);
            top_ = diskToAdd;
            top_->setIsTop(true);
        }
    }
    // Set added disk with new configurations.
    diskToAdd->setPeg(this);
    diskToAdd->setBrush(this->brush());

    disks_.push_back(diskToAdd);
    return true;
}

bool Peg::removeDisk(Disk *diskToRemove)
{
    // Get the iterator for item to remove
    vector<Disk*>::iterator iter = find(disks_.begin(), disks_.end(), diskToRemove);
    // Return false if the disk doesn't exist.
    if(iter == disks_.end()){ return false; }
    // Remove the disk from the peg, but we don't deallocate it from the game.
    disks_.erase(iter);
    // If disk is empty after the removal, set top_ to nullptr
    if(disks_.empty()) {
        top_ = nullptr;
    } else {
        // If the removed item was the top of the peg,
        // set the next item as the new top.
        if(diskToRemove == top_){
            top_ = *iter;
            top_->setIsTop(true);
        }
    }
    return true;
}

bool Peg::moveDiskTo(Peg *moveTo)
{
    if(top_ == nullptr) {
        return false;
    }
    if(moveTo->moveDiskFrom(top_, this)){
        // Move succeeded.
        return true;
    }
    // Move failed.
    return false;
}

bool Peg::moveDiskFrom(Disk *diskToMove, Peg *moveFrom)
{
    // Check the disk is smaller than the top disk of the peg
    if(top_ != nullptr && top_->getId() < diskToMove->getId()) {
        // Return false, the disk is begger than the top disk.
        return false;
    }
    if(this->addDisk(diskToMove) && moveFrom->removeDisk(diskToMove)){
        // Move succeeded.
        this->drawDisks();
        return true;
    }
    // Move failed.
    return false;
}
bool compare(Disk *a, Disk *b) {
    return a->getId() < b->getId();
}
void Peg::drawDisks()
{
    sort(disks_.begin(), disks_.end(), compare);
    int i = 0;
    for(Disk* disk : disks_) {
        // Get disk size.
        QSizeF size = disk->rect().size();
        // Set new position.
        // Set x cordinate of the disk in the middle of the peg.
        int x = this->x() + WIDTH/2 - size.width()/2;
        // Stack bigger disk from the bottom.
        int y = this->y() + HEIGHT - size.height() * ((int)disks_.size()- i);
        disk->setPos(x, y);
        ++i;
    }
}

bool Peg::checkWin(vector<Disk *> disks)
{
    if(top_ == nullptr || disks_.size() != disks.size()) {
        return false;
    }
    sort(disks_.begin(), disks_.end(), compare);
    // Check the target peg has all disks in right order.
    for(size_t i = 0; i < disks.size(); ++i) {
        if(disks.at(i)->getId() != disks_.at(i)->getId()) {
            return false;
        }
    }
    return true;
}

int Peg::type() const
{
    return Type;
}
