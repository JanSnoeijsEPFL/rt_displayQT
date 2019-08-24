#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <stdlib.h>
extern "C" {
#include "server.h"
}

#define LENGTH 1000
#define CHANNELS 7
#define NBFILES 800
#define DATARANGE 100
#define NB_SEQ 800
#define FIRST_SEQ 180
#define CLASSES 3
#define SAMPLES_PER_FRAME 10
using namespace std;
double number[LENGTH][CHANNELS];
int test_class[NB_SEQ];
char res[CLASSES][10];
double confidence[NB_SEQ];
static int fileNb = 0;
enum ch_names{plot, plot1, plot2, plot3, plot4, plot5, plot6, plot7};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    double atof(const char*string);
    //QGridLayout* layout1 = new QGridLayout(parent);
    //layout1->addWidget(QCustomPlot);
    char * line;
    size_t len = 0;
    int j=0, k=0;
    ssize_t read;
    char fileName[120];
    char* res_message;
    QPen pen;
    pen.setWidth(4);
    pen.setColor(QColor(255,140,0));
    ui->setupUi(this);
    ui->plot->addGraph(); // blue line
    ui->plot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->plot1->addGraph(); // blue line
    ui->plot1->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->plot2->addGraph(); // blue line
    ui->plot2->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->plot3->addGraph(); // blue line
    ui->plot3->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    /*ui->plot4->addGraph(); // blue line
    ui->plot4->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->plot5->addGraph(); // blue line
    ui->plot5->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->plot6->addGraph(); // blue line
    ui->plot6->graph(0)->setPen(QPen(QColor(40, 110, 255)));*/

    ui->plot7->addGraph(); // greenline
    ui->plot7->graph(0)->setPen(QPen(pen)); // classification results
    //ui->plot7->graph(0)->setWidth(1); // classification results

    ui->plot7->addGraph();
    pen.setColor(QColor(220,20,60));

    ui->plot7->graph(1)->setPen(pen);

    ui->plot7->addGraph();
    pen.setColor(QColor(34,139,34));

    ui->plot7->graph(2)->setPen(pen);

    ui->plot8->addGraph();
    pen.setColor("black");
    pen.setWidth(2);
    ui->plot8->graph(0)->setPen(pen); // confidence

    res_message = send_receive_TCP(FIRST_SEQ); // send first sequence
    snprintf(fileName, sizeof(fileName), "/home/lsmpc1-linux/deliverables/De10_standard_files/NNESD/chb01_quantized_seq/RT_NormQuantdata_seq%d",FIRST_SEQ);
    FILE* inputFile = fopen(fileName, "r");

    for (k = 0; k < CHANNELS; k++)
    {
        for (j = 0; j < LENGTH; j++)
        {
            read = getline(&line, &len, inputFile);
            number[j][k] = atof(line);
            printf("number: %f %s\n", number[j][k], line);
        }
    }
    fclose(inputFile);
    parseTargetMessage(res_message);
    computeConfidence(0);//first index

    /*FILE* testFile = fopen("/home/jan/De10_standard_files/NNESD/chb01_quantized_seq/custom4fracbits_patient1.txt", "r");

    for (j = 0; j < NB_SEQ; j++)
    {
        read = getline(&line, &len, testFile);
        parseTargets(line);
        for (k = 0; k<CLASSES; k++)
        {
           test_targets[j][k]=atof(res[k]);
           //qDebug("line: %s ---, res: %s\n", line, res[k]);
        }
        test_max[j]=test_targets[j][0]; //assign to first class
        test_class[j]=0; //assign to first class (pre-ictal)

   for(k = 1; k<CLASSES; k++)
        {
            if (test_targets[j][k]>test_max[j])
            {
                test_max[j]=test_targets[j][k];
                test_class[j]=k;
            }
        }
        test_targets[j][test_class[j]]=0; // remove max
        test_max2[j]=test_targets[j][0]; // initizalize to first class
        for(k = 1; k<CLASSES; k++)
        {
            if (test_targets[j][k]>test_max2[j])
            {
                test_max2[j]=test_targets[j][k]; //2nd max
            }
        }
        confidence[j]=100*(test_max[j]-test_max2[j])/test_max[j];
    }
    fclose(testFile);*/

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plot->xAxis->setTicker(timeTicker);
    //ui->plot->axisRect()->setupFullAxesBox();
    ui->plot->yAxis->setRange(-1.2, 1.2);

    ui->plot1->xAxis->setTicker(timeTicker);
    //ui->plot1->axisRect()->setupFullAxesBox();
    ui->plot1->yAxis->setRange(-1.2, 1.2);

    ui->plot2->xAxis->setTicker(timeTicker);
    //ui->plot2->axisRect()->setupFullAxesBox();
    ui->plot2->yAxis->setRange(-1.2, 1.2);

    ui->plot3->xAxis->setTicker(timeTicker);
    //ui->plot3->axisRect()->setupFullAxesBox();
    ui->plot3->yAxis->setRange(-1.2, 1.2);

   /* ui->plot4->xAxis->setTicker(timeTicker);
    //ui->plot4->axisRect()->setupFullAxesBox();
    ui->plot4->yAxis->setRange(-1.2, 1.2);

    ui->plot5->xAxis->setTicker(timeTicker);
    //ui->plot5->axisRect()->setupFullAxesBox();
    ui->plot5->yAxis->setRange(-1.2, 1.2);

    ui->plot6->xAxis->setTicker(timeTicker);
    //ui->plot6->axisRect()->setupFullAxesBox();
    /ui->plot6->yAxis->setRange(-1.2, 1.2);*/

    ui->plot7->xAxis->setTicker(timeTicker);
    //ui->plot7->axisRect()->setupFullAxesBox();
    ui->plot7->yAxis->setRange(-0.2,2.2);

    ui->plot8->xAxis->setTicker(timeTicker);
    //ui->plot8->axisRect()->setupFullAxesBox();
    ui->plot8->yAxis->setRange(-5,105);

    QTimer *dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer->start(0); // Interval 0 means to refresh as fast as possible

}

void MainWindow::computeConfidence(int sequence)
{
    double test_targets[NB_SEQ][CLASSES];
    double test_max[NB_SEQ];
    double test_max2[NB_SEQ];
    int k;
    double test;
    double atof(const char*string);

    for (k = 0; k<CLASSES; k++)
    {
       test_targets[sequence][k]=atof(res[k]);
       test=atof("1,233335");
       printf("test %f, string : 1,233335 \n", test);
       test=atof("1.233335");
       printf("test %f, string : 1.233335 \n", test);
       printf("Test results %s %f\n", res[k], test_targets[sequence][k]);
    }
    printf("%d\n", sequence);
    //printf("test target: %d ",test_targets[sequence][0]);
   // printf("%d ",test_targets[sequence][1]);
    //printf("%d\n",test_targets[sequence][2]);

    test_max[sequence]=test_targets[sequence][0]; //assign to first class
    test_class[sequence]=0; //assign to first class (pre-ictal)
    for(k = 1; k<CLASSES; k++)
    {
        if (test_targets[sequence][k]>test_max[sequence])
        {
            test_max[sequence]=test_targets[sequence][k];
            test_class[sequence]=k;
        }
    }
    test_targets[sequence][test_class[sequence]]=0; // remove max
    test_max2[sequence]=test_targets[sequence][0]; // initizalize to first class
    for(k = 1; k<CLASSES; k++)
    {
        if (test_targets[sequence][k]>test_max2[sequence])
        {
            test_max2[sequence]=test_targets[sequence][k]; //2nd max
        }
    }
    confidence[sequence]=100*(test_max[sequence]-test_max2[sequence])/test_max[sequence];
    printf("testclass %d\n", test_class[fileNb]);
    printf("confidence %f\n", confidence[fileNb]);
}

void MainWindow::parseTargetMessage(char* message)
{
    int i=0;
    int char_nb = 0;
    int cl_nb = 0;
    for (i=29;i<(int)strlen(message);i++)
    {
        if (message[i] ==',')
        {
            cl_nb ++;
        }
        else
        {
            char_nb=(i-29)%9;
            res[cl_nb][char_nb]=message[i];
            //printf("res: %s\n", res[cl_nb]);
            //qDebug("cl_nb:%d res[cl_nb]: %s ", cl_nb, res[cl_nb]);

        }
    }
}
void MainWindow::parseTargets(char* line)
{
    int i,k;
    int cl_nb=0;

    for (i=0;i<(int)strlen(line);i++)
    {
        if (line[i] ==',')
        {
            cl_nb ++;
        }
        else
        {
            k=i%9;
            res[cl_nb][k]=line[i];
            //qDebug("cl_nb:%d res[cl_nb]: %s ", cl_nb, res[cl_nb]);

        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::readNewFile(){
    static int i =1;
    char * line;
    size_t len = 0;
    ssize_t read;
    int j=0, k=0;
    int clearGraphs = 0;
    char fileName[120] ;
    char* res_message;
    double atof(const char*string);
    if (i < NBFILES){
        snprintf(fileName, sizeof(fileName), "/home/lsmpc1-linux/deliverables/De10_standard_files/NNESD/chb01_quantized_seq/RT_NormQuantdata_seq%d",i+FIRST_SEQ);
        printf("Reading from %s\n", fileName);
        res_message = send_receive_TCP(FIRST_SEQ+i);
        parseTargetMessage(res_message); //sequence i
        computeConfidence(i);
        FILE* inputFile = fopen(fileName, "r"); //1000 x 23 format

        for (k = 0; k < CHANNELS; k++)
        {
            for (j = 0; j < LENGTH; j++)
            {
                read = getline(&line, &len, inputFile);
                number[j][k] = atof(line);
            }
        }
        fclose(inputFile);
        fileNb = i;
        i++;
        //qDebug("FILE %d/n", i);
        if  (i%100 == 0)
        {
            clearGraphs = 1;
            qDebug("setting clearGraph");

        }
        else {
            clearGraphs = 0;
        }
    }
    return clearGraphs;
}
void MainWindow::realtimeDataSlot(){
    static QTime time(QTime::currentTime());
     // calculate two new data points
    double key = time.elapsed()/1000.0; // time elapsed since start of demo in sec
    int j;
    int clearGraphs;
    static int i;
    char disp_var[30];
    static double lastPointKey = 0;
    static double my_nan = 0.0 / 0.0;
    if (key-lastPointKey > 0.017) // at most add point every 1 ms
    {
      // add data to lines:
      //printf("%d\n",(int)(key/(key-lastPointKey)));
        if (i < LENGTH){
            for (j = 0; j <SAMPLES_PER_FRAME; j++)
            {
                ui->plot ->graph(0)->addData(key+1/SAMPLES_PER_FRAME*j, number[i+j][0]);
                ui->plot1->graph(0)->addData(key+1/SAMPLES_PER_FRAME*j, number[i+j][1]);
                ui->plot2->graph(0)->addData(key+1/SAMPLES_PER_FRAME*j, number[i+j][2]);
                ui->plot3->graph(0)->addData(key+1/SAMPLES_PER_FRAME*j, number[i+j][3]);
                /*ui->plot4->graph(0)->addData(key+1/SAMPLES_PER_FRAME*j, number[i+j][4]);
                ui->plot5->graph(0)->addData(key+1/SAMPLES_PER_FRAME*j, number[i+j][5]);
                ui->plot6->graph(0)->addData(key+1/SAMPLES_PER_FRAME*j, number[i+j][6]);*/
                i++;
            }
            //if(key-lastPointKey > 0.005)

                if (test_class[fileNb]==0)
                {
                    ui->plot7->graph(0)->addData(key,1);
                    ui->plot7->graph(1)->addData(key,my_nan); // outside graph range
                    ui->plot7->graph(2)->addData(key,my_nan); // outside graph range
                }
                else if(test_class[fileNb]==1)
                {
                    ui->plot7->graph(0)->addData(key,my_nan);
                    ui->plot7->graph(1)->addData(key,1);
                    ui->plot7->graph(2)->addData(key,my_nan);
                }
                else {
                    ui->plot7->graph(0)->addData(key,my_nan);
                    ui->plot7->graph(1)->addData(key,my_nan);
                    ui->plot7->graph(2)->addData(key,1);
                }
                ui->plot8->graph(0)->addData(key,confidence[fileNb]);
            //}


            if (i == LENGTH)
            {
              i = 0;
              clearGraphs=readNewFile();
              if (clearGraphs == 1)
              {
                  qDebug("clearing graphs\n");
                  ui->plot ->graph(0)->data()->removeBefore(key-100);
                  ui->plot1->graph(0)->data()->removeBefore(key-100);
                  ui->plot2->graph(0)->data()->removeBefore(key-100);
                  ui->plot3->graph(0)->data()->removeBefore(key-100);
                 /* ui->plot4->graph(0)->data()->removeBefore(key-100);
                  ui->plot5->graph(0)->data()->removeBefore(key-100);
                  ui->plot6->graph(0)->data()->removeBefore(key-100);*/
                  ui->plot7->graph(0)->data()->removeBefore(key-100);
                  ui->plot7->graph(1)->data()->removeBefore(key-100);
                  ui->plot7->graph(2)->data()->removeBefore(key-100);
                  ui->plot8->graph(0)->data()->removeBefore(key-100);
              }
            }

        }
      //printf("i:%d", i);
      // ui->plot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
     lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plot->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot->replot();

    ui->plot1->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot1->replot();

    ui->plot2->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot2->replot();

    ui->plot3->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot3->replot();

    /*ui->plot4->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot4->replot();

    ui->plot5->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot5->replot();

    ui->plot6->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot6->replot();*/

    ui->plot7->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot7->replot();

    ui->plot8->xAxis->setRange(key, DATARANGE, Qt::AlignRight);
    ui->plot8->replot();
    // calculate frames per second:
    static double lastFpsKey=0;
    static int frameCount=0;
    ++frameCount;
    if (key-lastFpsKey>0.5){
        ui->textEdit_3->clear();
        snprintf(disp_var,sizeof(disp_var), "Sequences processed %d", fileNb);
        ui->textEdit_3->append(QString::fromStdString(disp_var));
        snprintf(disp_var,sizeof(disp_var), "File Number in database %d", 21+int(fileNb/100));
        ui->textEdit_3->append(QString::fromStdString(disp_var));
    }
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plot->graph(0)->data()->size())
             //for (ch = 0; ch < CHANNELS; ch++)
                //.arg(ui->plot->graph(0)->data()->size())
            , 0);

      lastFpsKey = key;
      frameCount = 0;
    }

}
