#ifndef MEMORYTEST_H
#define MEMORYTEST_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;


QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);

private slots:
    void browse();
    void getTestCase();
    void getInputParameters();

private:
    QComboBox *createComboBox(const QStringList &list = QStringList());
    QLineEdit *showLine();

    void findFilesInDirectory(QString pathToDirectory);
    void updateConfigBox(QComboBox *configComboBox, QString inputValue);
    void updateConfigBox(QComboBox *configComboBox, QStringList listValues);

    QComboBox *fileComboBox;
    QComboBox *configComboBox;
    QLineEdit *lineTestCase;
    QLineEdit *lineType;
    QLineEdit *lineCountStack;
    QLineEdit *lineConfig;
    QLineEdit *lineSamples;

};
#endif // MEMORYTEST_H
