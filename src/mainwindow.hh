/*
 * mainwindow.hh
 *
 * Author: Jun Jung
 * Student Number: 292961
 * Eamil: jun.jung@tuni.fi
 *
 * This program is a tower of hanoi game implemented with graphic user interface.
 *
 * Peg vector contains information of each peg.
 * Each peg contains disks stacked on it in order of disk size.
 *
 * Disk vector constains information of each disk for the entire game.
 *
 * There are three different methods for moving disks; buttons, keyboard commands and drag and drop.
 *
 */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "peg.hh"
#include "disk.hh"
#include "scene.hh"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Keyboard command
    void keyPressEvent(QKeyEvent *event) override;

private slots:

    /**
     * Listen mouse event from Scene class
     * when a disk is successfully moved.
     *
     * Check win condition and print history.
     *
     * @param from Peg which the disk moved from
     * @param to   Peg which the disk moved to
     */
    void onChangedDiskByMouse(char from, char to);

    // Difficulty toggle buttons
    void onEasyToggled      (bool checked);
    void onMiddleToggled    (bool checked);
    void onDifficultToggled (bool checked);

    // Game start/reset buttons
    void onStartClicked();
    void onResetClicked();

    // Control buttons
    void onAtoBClicked();
    void onAtoCClicked();
    void onBtoAClicked();
    void onBtoCClicked();
    void onCtoAClicked();
    void onCtoBClicked();

private:
    Ui::MainWindow *ui_;

    // Graphic scene cordinates
    const int BORDER_UP     = 0;
    const int BORDER_DOWN   = 260;
    const int BORDER_LEFT   = 0;
    const int BORDER_RIGHT  = 680;

    // Game difficulties (the number of disks)
    const int EASY          = 4;
    const int MIDDLE        = 6;
    const int DIFFICULT     = 8;

    // Game timer attributes
    bool    is_started_;
    QTimer  *timer_;
    int     second_;
    int     best_result_;

    // Keyboard command attributes
    QTimer  *kTimer_;        // Checking time after first key inputs
    int     KEY_WAIT_TIME = 3;
    int     key_input_time_;
    char    departure_;
    char    destination_;

    Scene   *scene_;
    int     turn_;

    // Total pegs in game
    int             PEG_SIZE = 3;
    vector<Peg *>    pegs_;

    // Total disks in game
    // Disk size can be seleted in game.
    int             disk_size_ = EASY;
    vector<Disk *>  disks_;

    // Timer functions
    void countPlayTime      ();
    void countKeyInputTime  ();

    // Initialize graphic items
    void initializePegs     ();
    void initializeDisks    (int difficulty);

    // Extra functions
    void enableMoveButtons  (bool enabled);
    bool moveDisk           (char from, char to);
    void checkWin           ();
    void printHistory       (char from, char to);
};

#endif // MAINWINDOW_HH
