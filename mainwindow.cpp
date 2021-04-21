/*
 * mainwindow.cpp
 *
 * Source file of mainwindow.hh
 */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    
    scene_ = new Scene(this);
    is_started_ = false;
    turn_ = 0;

    // Timer
    second_ = 0;
    best_result_ = 3600*10;
    timer_ = new QTimer;

    connect(timer_, &QTimer::timeout, this, &MainWindow::countPlayTime);

    // Keyboard input wait timer
    kTimer_ = new QTimer;
    key_input_time_ = 0;
    departure_ = 0;
    destination_ = 0;

    connect(kTimer_, &QTimer::timeout, this, &MainWindow::countKeyInputTime);

    // Set geometry of the graphicsView
    int left_margin = 10; // x coordinate
    int top_margin = 270; // y coordinate
    QGraphicsView* view = ui_->graphicsView;
    view->setGeometry(left_margin, top_margin,
                      BORDER_RIGHT + 2, BORDER_DOWN + 2);
    view->setScene(scene_);
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    // Connect mouse events from Scene
    connect(scene_, &Scene::movedDisk, this, &MainWindow::onChangedDiskByMouse);

    // Disable move buttons
    enableMoveButtons(false);
    ui_->resetButton->setEnabled(false);

    // Draw pegs
    initializePegs();;
    
    // Set difficulty by user
    // Default is easy
    ui_->easyRadioButton->setChecked(true);
}

MainWindow::~MainWindow()
{
    for(Disk* disk : disks_) {
        delete disk;
    }
    delete kTimer_;
    delete timer_;
    delete ui_;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if(!is_started_){
        return;
    }
    // Set departure
    if(departure_ == 0) {
        destination_ = 0;
        if(event->key() == Qt::Key_A){
            departure_ = 'A';
        } else if(event->key() == Qt::Key_B){
            departure_ = 'B';
        } else if(event->key() == Qt::Key_C){
            departure_ = 'C';
        }
        // Commands not support
        else { return; }

        kTimer_->start(1000);
        ui_->labelKeyInput->setText(QString("Pressed %1, wait for next ... %2")
                                    .arg(departure_)
                                    .arg(KEY_WAIT_TIME - key_input_time_));
    }
    // Set destination
    else {
        if(event->key() == Qt::Key_A){
            destination_ = 'A';
        } else if(event->key() == Qt::Key_B){
            destination_ = 'B';
        } else if(event->key() == Qt::Key_C){
            destination_ = 'C';
        }
        // Commands not support
        else { return; }

        ui_->labelKeyInput->setText(QString(""));
        kTimer_->stop();
        key_input_time_ = 0;
    }
    if(departure_ && destination_){
        // Disk is already at the position.
        if(departure_ == destination_){
            ui_->labelKeyInput->setText(
                QString("Invalid movement."));
        } else {
            if(!moveDisk(departure_, destination_)){
                // Move failed
                ui_->labelKeyInput->setText(
                    QString("Invalid movement."));
            }
        }
        departure_ = 0;
    }
}

void MainWindow::onChangedDiskByMouse(char from, char to)
{
    printHistory(from, to);
    checkWin();
}

void MainWindow:: countPlayTime() {
    ++second_;
    int minutes = second_/60;
    int seconds = second_%60;
    ui_->lcdNumberMin->display(minutes);
    ui_->lcdNumberSec->display(seconds);
}

void MainWindow::countKeyInputTime()
{
    ++key_input_time_;
    ui_->labelKeyInput->setText(QString("Pressed %1, wait for next ... %2")
                                .arg(departure_)
                                .arg(KEY_WAIT_TIME - key_input_time_));
    if(key_input_time_ > KEY_WAIT_TIME) {
        ui_->labelKeyInput->setText("Key input time over.");
        kTimer_->stop();
        key_input_time_ = 0;
        departure_ = 0;
    }
}

void MainWindow::initializePegs()
{
    vector<QBrush> colors = {QBrush(QColor(200, 200, 0)),
                             QBrush(QColor(0, 200, 200)),
                             QBrush(QColor(200, 0, 200))};

    int leftRightMargin = 130;
    int itemInterval = (BORDER_RIGHT - leftRightMargin*2)/(PEG_SIZE-1);

    // Generate pegs
    for(int i = 0; i < PEG_SIZE; ++i){
        // Check if the peg already exists.
        bool isExist = false;
        for(Peg* peg : pegs_) {
            if(peg->getName() == 'A'+i) {
                isExist = true;
                break;
            }
        }
        // Skip to next round, if exists
        if(isExist) {continue;}

        // Configure information of new peg
        Peg* new_peg = new Peg('A'+i);
        int w = new_peg->rect().size().width();
        int h = new_peg->rect().size().height();
        // Set x cordinate for pegs have equal amount of margin between each other.
        int x = leftRightMargin + i*itemInterval - w/2 - 1;
        int y = BORDER_DOWN-h-1;

        new_peg->setPos(x, y);
        new_peg->setBrush(colors.at(i));

        QGraphicsTextItem *text = new QGraphicsTextItem(QString('A'+QChar(i)));
        text->setPos(x, 0);
        scene_->addItem(text);

        scene_->addItem(new_peg);
        pegs_.push_back(new_peg);
    }
}

void MainWindow::initializeDisks(int difficulty)
{
    if(difficulty < disk_size_){
        // If new difficulty level is easier than current,
        for(Disk* disk : disks_) {
            if(disk->getId() >= difficulty) {
                // Remove unnecessary disks.
                disks_.erase(disks_.end()-1);
                pegs_.at(0)->removeDisk(disk);

                // !Deallocation:
                // Deallocating the disk component only at mainwindow
                delete disk;
            }
        }
    } else {
        // Create new disks for the difficulty.
        for(int i = 0; i < difficulty; ++i) {
            bool isExist = false;
            // Check if the disk already exists.
            for(Disk* disk : disks_) {
                if(disk->getId() == i) {
                    isExist = true;
                    break;
                }
            }
            // Skip if exists
            if(isExist) {continue;}

            Disk* new_disk = new Disk(i);
            disks_.push_back(new_disk);
        }
    }

    // Place all disks to the first peg.
    Peg* source = pegs_.at(0);
    for(vector<Disk*>::iterator it = disks_.begin(); it != disks_.end(); ++it) {
        if(source->addDisk(*it)){
            scene_->addItem(*it);
        }
    }
    source->drawDisks();
    disk_size_ = difficulty;
}

void MainWindow::enableMoveButtons(bool enabled)
{
    ui_->atobButton->setEnabled(enabled);
    ui_->atocButton->setEnabled(enabled);
    ui_->btoaButton->setEnabled(enabled);
    ui_->btocButton->setEnabled(enabled);
    ui_->ctoaButton->setEnabled(enabled);
    ui_->ctobButton->setEnabled(enabled);
}

bool MainWindow::moveDisk(char from, char to)
{
    Peg* pegFrom = nullptr;
    Peg* pegTo = nullptr;

    // Find the peg which the disk moves from.
    foreach (Peg *peg, pegs_) {
        if(peg->getName() == from) {
            pegFrom = peg;
            break;
        }
    }
    // Find the peg which the disk moves to.
    foreach (Peg *peg, pegs_) {
        if(peg->getName() == to) {
            pegTo = peg;
            break;
        }
    }

    if(pegFrom->moveDiskTo(pegTo)){
        printHistory(from, to);
        checkWin();
        // Move succeeded
        return true;
    }
    // Move failed
    return false;

}

void MainWindow::checkWin()
{
    // Set target peg
    Peg* target = pegs_.at(2);

    if(target->checkWin(disks_)) {
        // If wins,
        timer_->stop();

        // Pop up window for notifying
        QMessageBox msgBox;
        bool newRecord = false;
        int minute = second_/60;
        int second = second_%60;

        // Set best record
        if(best_result_ > second_) {
            newRecord = true;
            best_result_ = second_;
            minute = best_result_/60;
            second = best_result_%60;
            ui_->lcdNumberBestMin->display(minute);
            ui_->lcdNumberBestSec->display(second);
        }

        // Winning notification
        QString winMsg;
        msgBox.setWindowTitle("Congraturation!");
        winMsg += newRecord ? "<p align='center'><strong>NEW RECORD!</strong></p>" : "";
        winMsg += QString("<p align='center'>Time: %1:%2<br>You won the game with %3 movements!</p>")
                .arg(minute).arg(second).arg(turn_);
        msgBox.setText(winMsg);
        msgBox.exec();

        enableMoveButtons(false);
        ui_->startButton->setDisabled(true);
        ui_->startButton->setText("Start");
    }
}

// Print history of movements.
void MainWindow::printHistory(char from, char to)
{
    // Previous history
    QString history = ui_->textBrowser->toPlainText();
    // Update history
    history = QString("%1: Disk moved from %2 to %3.\n")
            .arg(++turn_, 3)    // Round
            .arg(from)          // Departure peg
            .arg(to) + history; // Destination peg
    ui_->textBrowser->setText(history);
}

void MainWindow::onStartClicked()
{
    if(!is_started_) {
        qDebug() << "Start game";
        is_started_ = true;
        scene_->setIsStart(true);
        // Enable necessary buttons
        enableMoveButtons(true);
        ui_->resetButton->setEnabled(true);

        // Disable unnecessary buttons
        ui_->easyRadioButton->setDisabled(true);
        ui_->middleRadioButton->setDisabled(true);
        ui_->difficultRadioButton->setDisabled(true);

        timer_->start(1000);

        ui_->startButton->setText("Pause");
    } else {
        qDebug() << "Pause game";
        is_started_ = false;
        scene_->setIsStart(false);

        // Disable move buttons
        enableMoveButtons(false);

        timer_->stop();
        ui_->startButton->setText("Continue");
    }
}

void MainWindow::onResetClicked()
{
    qDebug() << "Reset game";
    foreach(Disk *disk, disks_) {
        disk->getPeg()->removeDisk(disk);
        delete disk;
    }
    disks_.clear();
    initializeDisks(disk_size_);

    second_ = 0;
    turn_ = 0;
    ui_->lcdNumberMin->display(0);
    ui_->lcdNumberSec->display(0);
    timer_->stop();
    ui_->startButton->setDisabled(false);
    ui_->startButton->setText("Start");
    ui_->textBrowser->setText("");
    is_started_ = false;
    scene_->setIsStart(false);

    // Disable move buttons
    enableMoveButtons(false);
    ui_->resetButton->setEnabled(false);
    ui_->easyRadioButton->setDisabled(false);
    ui_->middleRadioButton->setDisabled(false);
    ui_->difficultRadioButton->setDisabled(false);

}

void MainWindow::onEasyToggled(bool checked)
{
    ui_->easyRadioButton->setChecked(checked);
    if(checked){
        initializeDisks(EASY);
    }
}

void MainWindow::onMiddleToggled(bool checked)
{  
    ui_->middleRadioButton->setChecked(checked);
    if(checked){
        initializeDisks(MIDDLE);
    }
}

void MainWindow::onDifficultToggled(bool checked)
{
    ui_->difficultRadioButton->setChecked(checked);
    if(checked){
        initializeDisks(DIFFICULT);
    }
}

void MainWindow::onAtoBClicked()
{
    moveDisk('A', 'B');
}

void MainWindow::onAtoCClicked()
{
    moveDisk('A', 'C');
}

void MainWindow::onBtoAClicked()
{
    moveDisk('B', 'A');
}

void MainWindow::onBtoCClicked()
{
    moveDisk('B', 'C');
}

void MainWindow::onCtoAClicked()
{
    moveDisk('C', 'A');
}

void MainWindow::onCtoBClicked()
{
    moveDisk('C', 'B');
}
