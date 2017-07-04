#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>

/**
 * @brief set_image 将图像设置到label上,图像自动根据label的大小来缩放
 * @param label
 * #param image
 */
void set_image(QLabel *label, const QPixmap &image) {
    float ratio(0.);
    ratio = 1. * label->width() / image.width();
    ratio = fmin(1. * label->height() / image.height(), ratio );
    QPixmap m = image.scaled(static_cast<int>(image.width() * ratio), static_cast<int>(image.height() * ratio));
    label->setPixmap(m);
}

void set_image(QLabel *label, const std::string image_path) {
    QPixmap image(image_path.c_str());
    set_image(label, image);
}

/**
 * @brief MainWindow::display()
 * @brief 据系统中的所有的变量来设置当前界面中的各个部分的内容
 */

void MainWindow::display() {

    if (this->current_age_idx >= this->total_pair_num && this->current_gender_idx >= this->total_pair_num){
        QMessageBox::information(this, "annotation over", "Congratulations! You've finished all the job! Please save your work",QMessageBox::Ok);
        this->current_age_idx = this->total_pair_num - 1;
        this->current_gender_idx = this->total_pair_num - 1;
    }
    if (this->current_age_idx < 0 || this->current_gender_idx < 0) {
        QMessageBox::information(this, "annotation warning","You must start at 0",QMessageBox::Ok);
        this->current_age_idx = 0;
        this->current_gender_idx = 0;
    }

    this->ui->horizontalSlider_progress->setRange(0, this->total_pair_num - 1);
    this->ui->horizontalSlider_progress->setValue(this->current_age_idx);
    //文件名
    std::string image_name_1 = this->image_list_1[this->current_age_idx];
    std::string image_base_name_1 = image_name_1.substr(image_name_1.find_last_of("/") + 1);
    this->ui->label_5->setText(image_base_name_1.c_str());

    //显示图像
    set_image(this->ui->label_3, image_name_1);
    //显示标注状态
    std::string show_image_age = "None";
    switch (this->age_list[this->current_age_idx]) {
    case Age::age_annotate:
        show_image_age = "None";
        break;
    case Age::age0:
        show_image_age = "0-2";
        break;
    case Age::age1:
        show_image_age = "4-6";
        break;
    case Age::age2:
        show_image_age = "8-12";
        break;
    case Age::age3:
        show_image_age = "15-20";
        break;
    case Age::age4:
        show_image_age = "25-32";
        break;
    case Age::age5:
        show_image_age = "38-43";
        break;
    case Age::age6:
        show_image_age = "48-53";
        break;
    case Age::age7:
        show_image_age = "60-100";
        break;

    }
    this->ui->label_7->setText(show_image_age.c_str());

    std::string show_image_gender = "None";
    switch (this->gender_list[current_gender_idx]) {
    case Gender::gender_annotate:
        show_image_gender = "None";
        break;
    case Gender::Male:
        show_image_gender = "Male";
        break;
    case Gender::FeMale:
        show_image_gender = "Female";
        break;
    }
    this->ui->label_6->setText(show_image_gender.c_str());

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 选择输入文件
    while(1) {
        QString file_name = QFileDialog::getOpenFileName(this, "choose a file to annotate", ".");
        if (file_name.isEmpty()) {
            int ok = QMessageBox::information(this, "choose a file to annotate", "Don't want to work now?", QMessageBox::Ok | QMessageBox::Cancel);
            if (ok == QMessageBox::Ok) {
                exit(0);
            }
            continue;
        }
        std::ifstream is(file_name.toStdString());
        std::string image_name;
        while(is >> image_name) {
            this->image_list_1.push_back(image_name);
        }
        is.close();
        if (0 == image_list_1.size()){
            QMessageBox::information(this, "choose a file to annotate", "this image list is empty", QMessageBox::Ok);
            continue;
        }
        break;
    }
    // 初始化
    this->total_pair_num = image_list_1.size();
    this->current_age_idx = 0;
    this->current_gender_idx = 0;
    std::vector<Age> age_list(this->total_pair_num, Age::age_annotate);
    std::vector<Gender> gender_list(this->total_pair_num, Gender::gender_annotate);
    this->age_list.swap(age_list);
    this->gender_list.swap(gender_list);
    display();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_pushButton_prev_clicked()
 * 进入上一张图片
 */

void MainWindow::on_pushButton_prev_clicked()
{
        -- this->current_age_idx;
        -- this->current_gender_idx;
        display();

}

/**
 * @brief MainWindow::on_pushButton_next_clicked()
 * 进入下一张图片
 */
void MainWindow::on_pushButton_next_clicked()
{
    if (this->age_list[this->current_age_idx] == -1 || this->gender_list[this->current_gender_idx] == -1) {
        QMessageBox::critical(this, "critical", "You must forget to annotate age or gender", QMessageBox::Ok);
        display();
    }
    if (this->age_list[this->current_age_idx] != -1 && this->gender_list[this->current_gender_idx] != -1){
        ++ this->current_age_idx;
        ++ this->current_gender_idx;
        display();

    }
}

/**
 * @brief MainWindow::on_pushButton_save_clicked()
 * 存储标注的结果
 */

void MainWindow::on_pushButton_save_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this, "choose a file to save", ".");
    if (file_name.isEmpty()) {
        QMessageBox::information(this, "choose a file to save", "please enter a legal file name", QMessageBox::Ok);
        return;
    }
    std::ofstream os(file_name.toStdString());
    for (int idx = 0; idx < static_cast<int>(this->age_list.size()); ++ idx) {
        os << this->image_list_1[idx] << " " << this->gender_list[idx] << " " << this->age_list[idx] << "\n";
    }
    os.close();
    QMessageBox::information(this, "save", "save result success", QMessageBox::Ok);
}

/**
 * @brief MainWindow::on_pushButton_02_clicked()
 * 标注年龄段在0-2
 */
void MainWindow::on_pushButton_02_clicked()
{
    this->age_list[this->current_age_idx] = MainWindow::Age::age0;
    display();
}

/**
 * @brief MainWindow::on_pushButton_46_clicked()
 * 标注年龄段在4-6
 */
void MainWindow::on_pushButton_46_clicked()
{
    this->age_list[this->current_age_idx] = MainWindow::Age::age1;
    display();
}

/**
 * @brief MainWindow::on_pushButton_812_clicked()
 * 标注年龄段在8-12
 */
void MainWindow::on_pushButton_812_clicked()
{
    this->age_list[this->current_age_idx] = MainWindow::Age::age2;
    display();
}


/**
 * @brief MainWindow::on_pushButton_1520_clicked()
 * 标注年龄段在15-20
 */
void MainWindow::on_pushButton_1520_clicked()
{
    this->age_list[this->current_age_idx] = MainWindow::Age::age3;
    display();
}

/**
 * @brief MainWindow::on_pushButton_2532_clicked()
 * 标注年龄段在25-32
 */
void MainWindow::on_pushButton_2532_clicked()
{
    this->age_list[this->current_age_idx] = MainWindow::Age::age4;
    display();
}

/**
 * @brief MainWindow::on_pushButton_3843_clicked()
 * 标注年龄段在38-43
 */
void MainWindow::on_pushButton_3843_clicked()
{
    this->age_list[this->current_age_idx] = MainWindow::Age::age5;
    display();
}

/**
 * @brief MainWindow::on_pushButton_4853_clicked()
 * 标注年龄段在48-53
 */
void MainWindow::on_pushButton_4853_clicked()
{
    this->age_list[this->current_age_idx] = MainWindow::Age::age6;
    display();
}

/**
 * @brief MainWindow::on_pushButton_60100_clicked()
 * 标注年龄段在60-100
 */
void MainWindow::on_pushButton_60100_clicked()
{
    this->age_list[this->current_age_idx] = MainWindow::Age::age7;
    display();
}
/**
 * @brief MainWindow::on_pushButton_female_clicked()
 * 标注性别为女性
 */
void MainWindow::on_pushButton_female_clicked()
{
    this->gender_list[this->current_gender_idx] = MainWindow::Gender::FeMale;
    display();
}

/**
 * @brief MainWindow::on_pushButton_male_clicked()
 * 标注性别为男性
 */
void MainWindow::on_pushButton_male_clicked()
{
    this->gender_list[this->current_gender_idx] = MainWindow::Gender::Male;
    display();
}


/**
 * @brief MainWindow::on_horizontalSlider_progress_sliderReleased
 * 拖放进度条，控制进度
 */
void MainWindow::on_horizontalSlider_progress_sliderReleased()
{
    int pos = this->ui->horizontalSlider_progress->value();
    this->current_age_idx = pos;
    this->current_gender_idx = pos;
    this->display();

}

