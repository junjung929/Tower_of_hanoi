/*
 * scene.hh
 *
 * Author: Jun Jung
 * Student Number: 292961
 * Email: jun.jung@tuni.fi
 *
 * This file is subclass of QGraphicsScene.
 * This file handles mouse click for dragging and dropping.
 */

#ifndef VIEW_HH
#define VIEW_HH

#include "disk.hh"
#include "peg.hh"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(QObject *parent=nullptr);

    void setIsStart(bool isStart);

signals:
    void movedDisk(char from, char to);

protected:
    void mousePressEvent    (QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent  (QGraphicsSceneMouseEvent *event) override;

private:
    bool        is_start;
    Peg         *peg_;
    Disk        *selected_disk_;
    QPointF     original_pos_;
};

#endif // VIEW_HH
