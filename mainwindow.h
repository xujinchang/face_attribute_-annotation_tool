#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <string>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    enum Gender{
        Male  = 0,
        FeMale = 1,
        gender_annotate = -1
    };

    enum Age{
        age0 = 0,
        age1 = 1,
        age2 = 2,
        age3 = 3,
        age4 = 4,
        age5 = 5,
        age6 = 6,
        age7 = 7,
        age_annotate = -1
    };
     Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void display();

private slots:

    void on_pushButton_prev_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_1520_clicked();

    void on_pushButton_4853_clicked();

    void on_pushButton_812_clicked();

    void on_pushButton_3843_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_2532_clicked();

    void on_pushButton_02_clicked();

    void on_pushButton_female_clicked();

    void on_pushButton_male_clicked();

    void on_pushButton_60100_clicked();

    void on_horizontalSlider_progress_sliderReleased();

private:
    Ui::MainWindow *ui;
    std::vector<std::string> image_list_1;
    int current_age_idx;
    int current_gender_idx;
    int total_pair_num;
    std::vector<Gender> gender_list;
    std::vector<Age> age_list;

};

#endif // MAINWINDOW_H
