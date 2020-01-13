/*
 * scene.cpp
 *
 * This file is a source file of scene.hh
 */

#include "scene.hh"
#include <QDebug>
#include <QGraphicsItem>

Scene::Scene(QObject *parent): QGraphicsScene(parent), peg_(nullptr), selected_disk_(nullptr)
{
    is_start = false;
}

void Scene::setIsStart(bool isStart)
{
    is_start = isStart;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && is_start) {
        QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
        if(item == nullptr){
            // If there is no item,
            return;
        }
        if(item->type() != ItemType::TypeDisk) {
            // If item is not disk type,
            return;
        }
        Disk* selectedDisk = qgraphicsitem_cast<Disk *>(item);
        if(selectedDisk == nullptr || !selectedDisk->getIsTop()) {
            // If selected disk is not the top element,
            return;
        }
        selected_disk_ = selectedDisk;
        selected_disk_->setBrush(QColor(Qt::red));
        original_pos_ = selected_disk_->scenePos();
        update();
        QGraphicsScene::mousePressEvent(event);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && selected_disk_ != nullptr && is_start) {
        Peg *originalPeg = selected_disk_->getPeg();
        Peg *peg = nullptr;
        // Find a peg under mouse position.
        foreach(QGraphicsItem *item, items(event->scenePos())) {
            if(item->type() == ItemType::TypePeg) {
                // If the item is peg type,
                peg = qgraphicsitem_cast<Peg *>(item);
                peg_ = peg;
                break;
            }
        }
        if(peg_ != nullptr) {
            if(peg_->moveDiskFrom(selected_disk_, originalPeg)){
                // Emit signal
                emit movedDisk(originalPeg->getName(), peg_->getName());
                // Skipped to end.
                goto end;
            }
        }

        // If there move cannot happen, place the disk back to original position.
        selected_disk_->setBrush(originalPeg->brush());
        selected_disk_->setPos(original_pos_);

        end:
            update();
            QGraphicsScene::mouseReleaseEvent(event);
            selected_disk_ = nullptr;
            peg_ = nullptr;
    }
}
