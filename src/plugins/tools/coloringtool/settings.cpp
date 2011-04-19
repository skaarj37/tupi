/***************************************************************************
 *   Project TUPI: Magia 2D                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez / xtingray                                          *
 *                                                                         *
 *   KTooN's versions:                                                     * 
 *                                                                         *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       *
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Gustav Gonzalez - http://www.maefloresta.com       *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "settings.h"
#include "kdebug.h"
#include "kradiobuttongroup.h"
#include "ktitemtweener.h"
#include "kttweenerstep.h"
#include "kimagebutton.h"
#include "kseparator.h"
#include "kosd.h"

#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QColorDialog>

struct Settings::Private
{
    QWidget *innerPanel;
    QBoxLayout *layout;
    Mode mode;

    QLineEdit *input;
    QComboBox *comboInit;
    QComboBox *comboEnd;
    KRadioButtonGroup *options;

    QPushButton *initColorButton;
    QColor initialColor;
    QPushButton *endColorButton;
    QColor endingColor;
    QComboBox *comboIterations;
    QCheckBox *loopBox;
    QCheckBox *reverseLoopBox;

    QLabel *totalLabel;
    int totalSteps;

    bool selectionDone;
    bool propertiesDone;

    KImageButton *apply;
    KImageButton *remove;
};

Settings::Settings(QWidget *parent) : QWidget(parent), k(new Private)
{
    k->selectionDone = false;
    k->totalSteps = 0;

    k->layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    k->layout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    setFont(QFont("Arial", 8, QFont::Normal, false));

    QLabel *nameLabel = new QLabel(tr("Name") + ": ");
    k->input = new QLineEdit;

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    nameLayout->setMargin(0);
    nameLayout->setSpacing(0);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(k->input);

    k->options = new KRadioButtonGroup(tr("Options"), Qt::Vertical);
    k->options->addItem(tr("Select object"), 0);
    k->options->addItem(tr("Set Properties"), 1);
    connect(k->options, SIGNAL(clicked(int)), this, SLOT(emitOptionChanged(int)));

    k->apply = new KImageButton(QPixmap(THEME_DIR + "icons/save.png"), 22);
    connect(k->apply, SIGNAL(clicked()), this, SLOT(applyTween()));

    k->remove = new KImageButton(QPixmap(THEME_DIR + "icons/close.png"), 22);
    connect(k->remove, SIGNAL(clicked()), this, SIGNAL(clickedResetTween()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    buttonsLayout->setMargin(0);
    buttonsLayout->setSpacing(10);
    buttonsLayout->addWidget(k->apply);
    buttonsLayout->addWidget(k->remove);

    k->layout->addLayout(nameLayout);
    k->layout->addWidget(k->options);

    setInnerForm();

    k->layout->addSpacing(10);
    k->layout->addLayout(buttonsLayout);
    k->layout->setSpacing(5);

    activatePropertiesMode(Settings::Selection);
}

Settings::~Settings()
{
    delete k;
}

void Settings::setInnerForm()
{
    k->innerPanel = new QWidget;

    QBoxLayout *innerLayout = new QBoxLayout(QBoxLayout::TopToBottom, k->innerPanel);
    innerLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    QLabel *startingLabel = new QLabel(tr("Starting at frame") + ": ");
    startingLabel->setAlignment(Qt::AlignVCenter);
    k->comboInit = new QComboBox();
    k->comboInit->setMaximumWidth(50);
    k->comboInit->setEditable(false);
    k->comboInit->setValidator(new QIntValidator(k->comboInit));

    connect(k->comboInit, SIGNAL(currentIndexChanged(int)), this, SLOT(checkBottomLimit(int)));

    QLabel *endingLabel = new QLabel(tr("Ending at frame") + ": ");
    endingLabel->setAlignment(Qt::AlignVCenter);
    k->comboEnd = new QComboBox();
    k->comboEnd->setFixedWidth(60);
    k->comboEnd->setEditable(true);
    k->comboEnd->addItem(QString::number(1));
    k->comboEnd->setValidator(new QIntValidator(k->comboEnd));

    connect(k->comboEnd, SIGNAL(currentIndexChanged(int)), this, SLOT(checkTopLimit(int)));

    QHBoxLayout *startLayout = new QHBoxLayout;
    startLayout->setAlignment(Qt::AlignHCenter);
    startLayout->setMargin(0);
    startLayout->setSpacing(0);
    startLayout->addWidget(startingLabel);
    startLayout->addWidget(k->comboInit);

    QHBoxLayout *endLayout = new QHBoxLayout;
    endLayout->setAlignment(Qt::AlignHCenter);
    endLayout->setMargin(0);
    endLayout->setSpacing(0);
    endLayout->addWidget(endingLabel);
    endLayout->addWidget(k->comboEnd);

    k->totalLabel = new QLabel(tr("Frames Total") + ": 1");
    k->totalLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    QHBoxLayout *totalLayout = new QHBoxLayout;
    totalLayout->setAlignment(Qt::AlignHCenter);
    totalLayout->setMargin(0);
    totalLayout->setSpacing(0);
    totalLayout->addWidget(k->totalLabel);

    kFatal() << "Settings::setInnerForm() - Setting color white!";
    k->initialColor = QColor("#fff");
    k->initColorButton = new QPushButton();
    k->initColorButton->setText(tr("White"));
    k->initColorButton->setPalette(QPalette(k->initialColor));
    k->initColorButton->setAutoFillBackground(true);
    connect(k->initColorButton, SIGNAL(clicked()), this, SLOT(setInitialColor()));

    QLabel *coloringInitLabel = new QLabel(tr("Initial Color") + ": ");
    coloringInitLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QHBoxLayout *coloringInitLayout = new QHBoxLayout;
    coloringInitLayout->setAlignment(Qt::AlignHCenter);
    coloringInitLayout->setMargin(0);
    coloringInitLayout->setSpacing(0);
    coloringInitLayout->addWidget(coloringInitLabel);
    coloringInitLayout->addWidget(k->initColorButton);

    k->endingColor = QColor("#fff");
    k->endColorButton = new QPushButton();
    k->endColorButton->setText(tr("White"));
    k->endColorButton->setPalette(QPalette(k->endingColor));
    k->endColorButton->setAutoFillBackground(true);
    connect(k->endColorButton, SIGNAL(clicked()), this, SLOT(setEndingColor()));

    QLabel *coloringEndLabel = new QLabel(tr("Ending Color") + ": ");
    coloringEndLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QHBoxLayout *coloringEndLayout = new QHBoxLayout;
    coloringEndLayout->setAlignment(Qt::AlignHCenter);
    coloringEndLayout->setMargin(0);
    coloringEndLayout->setSpacing(0);
    coloringEndLayout->addWidget(coloringEndLabel);
    coloringEndLayout->addWidget(k->endColorButton);

    k->comboIterations = new QComboBox();
    k->comboIterations->setEditable(true);
    k->comboIterations->setValidator(new QIntValidator(k->comboIterations));
    for (int i=1; i<=100; i++)
         k->comboIterations->addItem(QString::number(i));

    QLabel *iterationsLabel = new QLabel(tr("Iterations") + ": ");
    iterationsLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QHBoxLayout *iterationsLayout = new QHBoxLayout;
    iterationsLayout->setAlignment(Qt::AlignHCenter);
    iterationsLayout->setMargin(0);
    iterationsLayout->setSpacing(0);
    iterationsLayout->addWidget(iterationsLabel);
    iterationsLayout->addWidget(k->comboIterations);

    k->loopBox = new QCheckBox(tr("Loop"), k->innerPanel);
    // k->loopBox->setChecked(true);
    connect(k->loopBox, SIGNAL(stateChanged(int)), this, SLOT(updateReverseCheckbox(int)));

    QVBoxLayout *loopLayout = new QVBoxLayout;
    loopLayout->setAlignment(Qt::AlignHCenter);
    loopLayout->setMargin(0);
    loopLayout->setSpacing(0);
    loopLayout->addWidget(k->loopBox);

    k->reverseLoopBox = new QCheckBox(tr("Loop with Reverse"), k->innerPanel);
    connect(k->reverseLoopBox, SIGNAL(stateChanged(int)), this, SLOT(updateLoopCheckbox(int)));

    QVBoxLayout *reverseLayout = new QVBoxLayout;
    reverseLayout->setAlignment(Qt::AlignHCenter);
    reverseLayout->setMargin(0);
    reverseLayout->setSpacing(0);
    reverseLayout->addWidget(k->reverseLoopBox);

    innerLayout->addLayout(startLayout);
    innerLayout->addLayout(endLayout);
    innerLayout->addLayout(totalLayout);

    innerLayout->addSpacing(10);
    innerLayout->addWidget(new KSeparator(Qt::Horizontal));

    innerLayout->addLayout(coloringInitLayout);
    innerLayout->addLayout(coloringEndLayout);

    innerLayout->addLayout(iterationsLayout);
    innerLayout->addLayout(loopLayout);
    innerLayout->addLayout(reverseLayout);

    innerLayout->addWidget(new KSeparator(Qt::Horizontal));

    k->layout->addWidget(k->innerPanel);

    activeInnerForm(false);
}

void Settings::activeInnerForm(bool enable)
{
    if (enable && !k->innerPanel->isVisible()) {
        k->propertiesDone = true;
        k->innerPanel->show();
    } else {
        k->propertiesDone = false;
        k->innerPanel->hide();
    }
}

void Settings::setParameters(const QString &name, int framesTotal, int startFrame)
{
    k->mode = Add;
    k->input->setText(name);

    activatePropertiesMode(Settings::Selection);
    k->apply->setToolTip(tr("Save Tween"));
    k->remove->setIcon(QPixmap(THEME_DIR + "icons/close.png"));
    k->remove->setToolTip(tr("Cancel Tween"));

    k->comboInit->setCurrentIndex(startFrame);
    k->comboInit->setEditable(false);
    k->comboInit->setEnabled(false);
}

void Settings::setParameters(KTItemTweener *currentTween)
{
    setEditMode();
    activatePropertiesMode(Settings::Properties);

    k->input->setText(currentTween->name());

    k->comboInit->setEnabled(true);
    k->comboInit->setEditable(true);
    k->comboInit->setCurrentIndex(currentTween->startFrame());
    int lastFrame = currentTween->startFrame() + currentTween->frames();
    k->comboEnd->setItemText(0, QString::number(lastFrame));
    k->comboEnd->setCurrentIndex(0);

    checkFramesRange();

    updateColor(currentTween->tweenInitialColor(), k->initColorButton);
    updateColor(currentTween->tweenEndingColor(), k->endColorButton);

    int iterations = currentTween->tweenColorIterations();
    k->comboIterations->setCurrentIndex(0);
    k->comboIterations->setItemText(0, QString::number(iterations));
    k->loopBox->setChecked(currentTween->tweenColorLoop());
    k->reverseLoopBox->setChecked(currentTween->tweenColorReverseLoop());
}

void Settings::initStartCombo(int framesTotal, int currentIndex)
{
    k->comboInit->clear();
    k->comboEnd->clear();

    for (int i=1; i<=framesTotal; i++) {
         k->comboInit->addItem(QString::number(i));
         k->comboEnd->addItem(QString::number(i));
    }

    k->comboInit->setCurrentIndex(currentIndex);
    k->comboEnd->setCurrentIndex(framesTotal - 1);
}

void Settings::setStartFrame(int currentIndex)
{
    k->comboInit->setCurrentIndex(currentIndex);
    int end = k->comboEnd->currentText().toInt();
    if (end < currentIndex+1)
        k->comboEnd->setItemText(0, QString::number(currentIndex + 1));
}

int Settings::totalSteps()
{
    return k->comboEnd->currentText().toInt() - k->comboInit->currentIndex();
}

void Settings::setEditMode()
{
    k->mode = Edit;
    k->apply->setToolTip(tr("Update Tween"));
    k->remove->setIcon(QPixmap(THEME_DIR + "icons/close_properties.png"));
    k->remove->setToolTip(tr("Close Tween properties"));
}

void Settings::applyTween()
{
    if (!k->selectionDone) {
        KOsd::self()->display(tr("Info"), tr("You must select at least one object!"), KOsd::Info);
        return;
    }

    if (!k->propertiesDone) {
        KOsd::self()->display(tr("Info"), tr("You must set Tween properties first!"), KOsd::Info);
        return;
    }

    // SQA: Verify Tween is really well applied before call setEditMode!
    setEditMode();
    emit clickedApplyTween();
}

void Settings::notifySelection(bool flag)
{
    k->selectionDone = flag;
}

void Settings::setInitialColor()
{
     k->initialColor = QColorDialog::getColor(k->initialColor, this);
     updateColor(k->initialColor, k->initColorButton);
}

void Settings::setInitialColor(QColor color) {
    k->initialColor = color;
    k->endingColor = QColor("#fff");
    updateColor(k->initialColor, k->initColorButton);
    updateColor(k->endingColor, k->endColorButton);
}

QString Settings::currentTweenName() const
{
    QString tweenName = k->input->text();
    if (tweenName.length() > 0)
        k->input->setFocus();

    return tweenName;
}

void Settings::emitOptionChanged(int option)
{
    switch (option) {
            case 0:
             {
                 activeInnerForm(false);
                 emit clickedSelect();
             }
            break;
            case 1:
             {
                 if (k->selectionDone) {
                     activeInnerForm(true);
                     emit clickedDefineProperties();
                 } else {
                     k->options->setCurrentIndex(0);
                     KOsd::self()->display(tr("Info"), tr("Select objects for Tweening first!"), KOsd::Info);
                 }
             }
    }
}

QString Settings::tweenToXml(int currentFrame)
{
    QDomDocument doc;

    QDomElement root = doc.createElement("tweening");
    root.setAttribute("name", currentTweenName());
    root.setAttribute("type", KTItemTweener::Coloring);
    root.setAttribute("init", currentFrame);
  
    checkFramesRange();
    root.setAttribute("frames", k->totalSteps);
    root.setAttribute("origin", "0,0");

    int initialRed = k->initialColor.red();
    int initialGreen = k->initialColor.green();
    int initialBlue = k->initialColor.blue();

    QString colorText = QString::number(initialRed) + "," + QString::number(initialGreen)
                        + "," + QString::number(initialBlue);
    root.setAttribute("initialColor", colorText);

    int endingRed = k->endingColor.red();
    int endingGreen = k->endingColor.green();
    int endingBlue = k->endingColor.blue();

    colorText = QString::number(endingRed) + "," + QString::number(endingGreen)
                        + "," + QString::number(endingBlue);
    root.setAttribute("endingColor", colorText);

    int iterations = k->comboIterations->currentText().toInt();
    if (iterations == 0) {
        iterations = 1;
        k->comboIterations->setCurrentIndex(0);
        k->comboIterations->setItemText(0, QString::number(iterations));
    }
    root.setAttribute("colorIterations", iterations);

    bool loop = k->loopBox->isChecked();
    if (loop)
        root.setAttribute("colorLoop", "1");
    else
        root.setAttribute("colorLoop", "0");

    bool reverse = k->reverseLoopBox->isChecked();
    if (reverse)
        root.setAttribute("colorReverseLoop", "1");
    else
        root.setAttribute("colorReverseLoop", "0");

    KTTweenerStep *step = new KTTweenerStep(0);
    step->setColor(k->initialColor);
    root.appendChild(step->toXml(doc));

    if (k->totalSteps > 1 && iterations > 1) {

        iterations -= 1;

        if (iterations == 0)
            iterations = 1;

        double redDelta = (double)(initialRed - endingRed)/(double)iterations;
        double greenDelta = (double)(initialGreen - endingGreen)/(double)iterations;
        double blueDelta = (double)(initialBlue - endingBlue)/(double)iterations; 

        kFatal() << "Settings::tweenToXml() - redDelta: " << redDelta;
        kFatal() << "Settings::tweenToXml() - greenDelta : " <<  greenDelta;
        kFatal() << "Settings::tweenToXml() - blueDelta : " <<  blueDelta;

        double redReference = initialRed - redDelta;
        double greenReference = initialGreen - greenDelta;
        double blueReference = initialBlue - greenDelta;

        kFatal() << "Settings::tweenToXml() - redReference: " << redReference;
        kFatal() << "Settings::tweenToXml() - greenReference : " <<  greenReference;
        kFatal() << "Settings::tweenToXml() - blueReference : " <<  blueReference;

        int cycle = 2;
        int reverseTop = (iterations*2)-2;

        for (int i=1; i < k->totalSteps; i++) {
             KTTweenerStep *step = new KTTweenerStep(i);
             QColor color = QColor(redReference, greenReference, blueReference);
             step->setColor(color);
             root.appendChild(step->toXml(doc));

             if (cycle <= iterations) {
                 if (cycle == iterations) {
                     redReference = endingRed;
                     greenReference = endingGreen;
                     blueReference = endingBlue;
                 } else {
                     redReference -= redDelta;
                     greenReference -= greenDelta;
                     blueReference -= blueDelta;
                 }
                 cycle++;
             } else {
                 // if repeat option is enabled
                 if (loop) { 
                     cycle = 1;
                     redReference = initialRed;
                     greenReference = initialGreen;
                     blueReference = initialBlue;
                 } else if (reverse) { // if reverse option is enabled
                            // if reverse cycle must start again
                            if (cycle <= reverseTop) {
                                redReference += redDelta;
                                greenReference += greenDelta;
                                blueReference += blueDelta;
                                cycle++;
                            } else {
                                cycle = 1;
                                redReference = initialRed;
                                greenReference = initialGreen;
                                blueReference = initialBlue;
                            }
                 } else { // If cycle is done and no loop and no reverse 
                     redReference = initialRed;
                     greenReference = initialGreen;
                     blueReference = initialBlue;
                 }
             }
        }
    }

    doc.appendChild(root);

    kFatal() << "Settings::tweenToXml() - xml:";
    kFatal() << doc.toString();

    return doc.toString();
}

void Settings::activatePropertiesMode(Settings::EditMode mode)
{
    k->options->setCurrentIndex(mode);
}

void Settings::checkBottomLimit(int index)
{
    emit startingPointChanged(index);
    checkFramesRange();
}

void Settings::checkTopLimit(int index)
{
    Q_UNUSED(index);
    checkFramesRange();
}

void Settings::checkFramesRange()
{
    int begin = k->comboInit->currentText().toInt();
    int end = k->comboEnd->currentText().toInt();

    if (begin > end) {
        k->comboEnd->setCurrentIndex(k->comboEnd->count()-1);
        end = k->comboEnd->currentText().toInt();
    }

    k->totalSteps = end - begin + 1;
    k->totalLabel->setText(tr("Frames Total") + ": " + QString::number(k->totalSteps));
}

void Settings::updateLoopCheckbox(int state)
{
    if (k->reverseLoopBox->isChecked() && k->loopBox->isChecked())
        k->loopBox->setChecked(false);
}

void Settings::updateReverseCheckbox(int state)
{
    if (k->reverseLoopBox->isChecked() && k->loopBox->isChecked())
        k->reverseLoopBox->setChecked(false);
}

void Settings::setEndingColor()
{
     k->endingColor = QColorDialog::getColor(k->endingColor, this);
     updateColor(k->endingColor, k->endColorButton);
}

void Settings::updateColor(QColor color, QPushButton *colorButton)
{
     if (color.isValid()) {
         kFatal() << "Settings::udpateColor() - Updating color: " << color.name();
         colorButton->setText(color.name());
         colorButton->setPalette(QPalette(color));
         colorButton->setAutoFillBackground(true);
     }
}
